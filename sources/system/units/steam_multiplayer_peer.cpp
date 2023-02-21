#include <system/units/steam_multiplayer_peer.hpp>

using namespace godot;

namespace spacerisk
{
    void SteamMultiplayerPeer::steam_net_connection_status_changed(Ref<SteamCallback> p_callback)
    {
        GET_CALLBACK_PTR(p_callback, SteamNetConnectionStatusChangedCallback)
        if (_is_server())
        {
            if (callback->m_eOldState == k_ESteamNetworkingConnectionState_None &&
                callback->m_info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
            {
                steam->AcceptConnection(callback->m_hConn);
            }
            else if (callback->m_info.m_eState == k_ESteamNetworkingConnectionState_Connected)
            {
                HSteamNetConnection new_connection = callback->m_hConn;
                uint64_t steam_id = callback->m_info.m_identityRemote.GetSteamID64();
                int64_t peer_id = static_cast<int64_t>(steam_id);
                id_to_connections[peer_id] = new_connection;
                id_to_connections[new_connection] = peer_id;
                emit_signal("peer_connected", peer_id);
            }

            if (callback->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer ||
                callback->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
            {
                if (callback->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
                {
                    _disconnect_peer(static_cast<int64_t>(callback->m_info.m_identityRemote.GetSteamID64()));
                }
                else
                {
                    steam->CloseConnection(callback->m_hConn, 0, nullptr, false);
                }
            }
        }
        // Client
        else
        {
            if (callback->m_info.m_eState == k_ESteamNetworkingConnectionState_Connected)
            {
                HSteamNetConnection new_connection = callback->m_hConn;
                id_to_connections[1] = new_connection;
                id_to_connections[new_connection] = 1;
                active_mode = MODE_CLIENT;
                connection_status = ConnectionStatus::CONNECTION_CONNECTED;
            }
            else if (callback->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer ||
                     callback->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
            {
                connection_status = ConnectionStatus::CONNECTION_DISCONNECTED;
            }
        }
    }

    void SteamMultiplayerPeer::set(Steam *p_steam)
    {
        steam = p_steam;
        steam->connect("SteamNetConnectionStatusChangedCallback", Callable(this, "steam_net_connection_status_changed"));
    }

    void SteamMultiplayerPeer::_bind_methods()
    {
        BIND_METHOD(SteamMultiplayerPeer, steam_net_connection_status_changed);
    }

    Error SteamMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size)
    {
        RUNTIME_ONLY(OK)
        ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

        current_packet.packet->Release();
        current_packet = incoming_packets.front()->get();
        incoming_packets.pop_front();

        *r_buffer = static_cast<const uint8_t *>(current_packet.packet->m_pData);
        *r_buffer_size = static_cast<int32_t>(current_packet.packet->GetSize());

        return OK;
    }

    Error SteamMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size)
    {
        RUNTIME_ONLY(OK)
        ERR_FAIL_COND_V_MSG(!_is_active(), ERR_UNCONFIGURED, "The multiplayer instance isn't currently active.");
        ERR_FAIL_COND_V_MSG(connection_status != CONNECTION_CONNECTED, ERR_UNCONFIGURED, "The multiplayer instance isn't currently connected to any server or client.");
        ERR_FAIL_COND_V_MSG(target_peer != 0 && !id_to_connections.has(ABS(target_peer)), ERR_INVALID_PARAMETER, vformat("Invalid target peer: %d", target_peer));
        ERR_FAIL_COND_V(active_mode == MODE_CLIENT && !id_to_connections.has(1), ERR_BUG);

#ifdef DEBUG_ENABLED
        if ((transfer_mode & TransferMode::TRANSFER_MODE_UNRELIABLE) && p_buffer_size > HOST_DEFAULT_MTU)
        {
            WARN_PRINT_ONCE(vformat("Sending %d bytes unreliably which is above the MTU (%d), this will result in higher packet loss", p_buffer_size, HOST_DEFAULT_MTU));
        }
#endif

        int Flag = 0;

        switch (get_transfer_mode())
        {
        case TRANSFER_MODE_UNRELIABLE:
        case TRANSFER_MODE_UNRELIABLE_ORDERED:
        {
            Flag = k_nSteamNetworkingSend_UnreliableNoDelay;
        }
        default:
        case TRANSFER_MODE_RELIABLE:
        {
            Flag = k_nSteamNetworkingSend_Reliable;
        }
        }

        if (_is_server())
        {
            if (target_peer == 0)
            {
                for (const auto &connection : id_to_connections)
                {
                    steam->SendMessageToConnection(connection.value, p_buffer, p_buffer_size, Flag, nullptr);
                }
            }
            else if (target_peer < 0)
            {
                // Send to all but one and make copies for sending.
                int64_t exclude = -target_peer;
                for (const auto &connection : id_to_connections)
                {
                    if (connection.key == exclude)
                    {
                        continue;
                    }
                    steam->SendMessageToConnection(connection.value, p_buffer, p_buffer_size, Flag, nullptr);
                }
            }
            else
            {
                steam->SendMessageToConnection(id_to_connections[target_peer], p_buffer, p_buffer_size, Flag, nullptr);
            }
        }
        else if (active_mode == MODE_CLIENT)
        {
            steam->SendMessageToConnection(id_to_connections[target_peer], p_buffer, p_buffer_size, Flag, nullptr);
        }
        else
        {
            if (target_peer <= 0)
            {
                // Send to all but one and make copies for sending.
                int64_t exclude = -target_peer;
                for (const auto &connection : id_to_connections)
                {
                    if (connection.key == exclude)
                    {
                        continue;
                    }
                    steam->SendMessageToConnection(connection.value, p_buffer, p_buffer_size, Flag, nullptr);
                }
            }
            else
            {
                steam->SendMessageToConnection(id_to_connections[target_peer], p_buffer, p_buffer_size, Flag, nullptr);
            }
        }

        return OK;
    }

    int32_t SteamMultiplayerPeer::_get_available_packet_count() const
    {
        RUNTIME_ONLY(0)
        return incoming_packets.size();
    }

    int32_t SteamMultiplayerPeer::_get_max_packet_size() const
    {
        RUNTIME_ONLY(0)
        return 1 << 24; // Anything is good
    }

    PackedByteArray SteamMultiplayerPeer::_get_packet_script()
    {
        PackedByteArray result;
        RUNTIME_ONLY(result)

        ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, result, "No incoming packets available.");

        current_packet.packet->Release();
        current_packet = incoming_packets.front()->get();
        incoming_packets.pop_front();

        uint32_t size = current_packet.packet->GetSize();
        result.resize(size);
        mempcpy(result.ptrw(), static_cast<const uint8_t *>(current_packet.packet->m_pData), size);

        return result;
    }

    Error SteamMultiplayerPeer::_put_packet_script(const PackedByteArray &p_buffer)
    {
        RUNTIME_ONLY(OK)
        return _put_packet(p_buffer.ptr(), p_buffer.size());
    }

    void SteamMultiplayerPeer::_set_transfer_channel(int32_t p_channel)
    {
        RUNTIME_ONLY()
        ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");
        transfer_channel = p_channel;
    }

    int32_t SteamMultiplayerPeer::_get_transfer_channel() const
    {
        RUNTIME_ONLY(0)
        ERR_FAIL_COND_V_MSG(!_is_active(), 1, "The multiplayer instance isn't currently active.");
        ERR_FAIL_COND_V(incoming_packets.size() == 0, 1);
        return incoming_packets.front()->get().channel;
    }

    void SteamMultiplayerPeer::_set_transfer_mode(MultiplayerPeer::TransferMode p_mode)
    {
        RUNTIME_ONLY()
        ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");
        transfer_mode = p_mode;
    }

    MultiplayerPeer::TransferMode SteamMultiplayerPeer::_get_transfer_mode() const
    {
        RUNTIME_ONLY(MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE)
        ERR_FAIL_COND_V_MSG(!_is_active(), TRANSFER_MODE_RELIABLE, "The multiplayer instance isn't currently active.");
        ERR_FAIL_COND_V(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE);
        return incoming_packets.front()->get().transfer_mode;
    }

    void SteamMultiplayerPeer::_set_target_peer(int32_t p_peer)
    {
        RUNTIME_ONLY()
        target_peer = p_peer;
    }

    int32_t SteamMultiplayerPeer::_get_packet_peer() const
    {
        RUNTIME_ONLY(0)
        ERR_FAIL_COND_V_MSG(!_is_active(), 1, "The multiplayer instance isn't currently active.");
        ERR_FAIL_COND_V(incoming_packets.size() == 0, 1);

        return incoming_packets.front()->get().from;
    }

    bool SteamMultiplayerPeer::_is_server() const
    {
        RUNTIME_ONLY(false)
        return active_mode == MODE_SERVER;
    }

    void SteamMultiplayerPeer::_poll()
    {
        RUNTIME_ONLY()

        if (!_is_active())
        {
            return;
        }

        int message_count = 0;

        if (active_mode == MODE_SERVER)
        {
            message_count = steam->ReceiveMessagesOnPollGroup(poll_group, messages_buffer, max_messages_buffer);
        }
        else if (active_mode == MODE_CLIENT)
        {
            message_count = steam->ReceiveMessagesOnConnection(id_to_connections[target_peer], messages_buffer, max_messages_buffer);
        }

        for (int i = 0; i < message_count; ++i)
        {
            SteamNetworkingMessage_t *message = messages_buffer[i];
            Packet new_packet;
            new_packet.channel = 0;
            new_packet.from = connections_to_id[message->GetConnection()];
            new_packet.packet = message;
            switch (message->m_nFlags)
            {
            case k_nSteamNetworkingSend_UnreliableNoDelay:
            {
                new_packet.transfer_mode = TRANSFER_MODE_UNRELIABLE;
            }
            default:
            case k_nSteamNetworkingSend_Reliable:
            {
                new_packet.transfer_mode = TRANSFER_MODE_RELIABLE;
            }
            }
            incoming_packets.push_back(new_packet);
        }
    }

    void SteamMultiplayerPeer::_close()
    {
        RUNTIME_ONLY()
        if (!_is_active())
        {
            return;
        }

        for (int i = 0; i < incoming_packets.size(); ++i)
        {
            incoming_packets.front()->get().packet->Release();
            incoming_packets.pop_front();
        }

        incoming_packets.clear();
        for (const auto &connection : id_to_connections)
        {
            steam->CloseConnection(connection.value, 0, nullptr, false);
        }
        id_to_connections.clear();
        connections_to_id.clear();

        if (active_mode == MODE_SERVER)
        {
            steam->DestroyPollGroup(poll_group);
            steam->CloseListenSocket(listen_socket);
        }
        else if (active_mode == MODE_CLIENT)
        {
        }
        active_mode = MODE_NONE;

        unique_id = 0;
        connection_status = CONNECTION_DISCONNECTED;
    }

    void SteamMultiplayerPeer::_disconnect_peer(int32_t p_peer, bool p_force)
    {
        RUNTIME_ONLY()
        ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");
        // TODO fix id_to_connections since id is no longer counts as int64_t
        ERR_FAIL_COND_MSG(!id_to_connections.has(p_peer), "Peer is not connected.");
        steam->CloseConnection(id_to_connections[p_peer], 0, nullptr, !p_force);
        connections_to_id.erase(id_to_connections[p_peer]);
        id_to_connections.erase(p_peer);
        emit_signal("peer_disconnected", p_peer);
    }

    int32_t SteamMultiplayerPeer::_get_unique_id() const
    {
        RUNTIME_ONLY(0)
        ERR_FAIL_COND_V_MSG(!_is_active(), 0, "The multiplayer instance isn't currently active.");
        return unique_id;
    }

    void SteamMultiplayerPeer::_set_refuse_new_connections(bool p_enable)
    {
        RUNTIME_ONLY()
        refuse_new_connections = p_enable;
    }

    bool SteamMultiplayerPeer::_is_refusing_new_connections() const
    {
        RUNTIME_ONLY(true)
        return refuse_new_connections;
    }

    MultiplayerPeer::ConnectionStatus SteamMultiplayerPeer::_get_connection_status() const
    {
        RUNTIME_ONLY(MultiplayerPeer::ConnectionStatus::CONNECTION_DISCONNECTED)
        return connection_status;
    }

    SteamMultiplayerPeer::SteamMultiplayerPeer()
    {
        RUNTIME_ONLY()
        messages_buffer = new SteamNetworkingMessage_t *[max_messages_buffer];
    }

    SteamMultiplayerPeer::~SteamMultiplayerPeer()
    {
        RUNTIME_ONLY()
        if (_is_active())
        {
            _close();
        }

        delete[] messages_buffer;
    }

    Error SteamMultiplayerPeer::create_client(uint64_t p_host_steamid, int p_port)
    {
        RUNTIME_ONLY(OK)
        SteamNetworkingIdentity new_identity;
        new_identity.SetSteamID64(p_host_steamid);
        steam->ConnectP2P(new_identity, p_port, 0, nullptr);
        connection_status = ConnectionStatus::CONNECTION_CONNECTING;
        unique_id = generate_unique_id();
        return OK;
    }

    Error SteamMultiplayerPeer::create_server(int p_port, int p_max_clients)
    {
        RUNTIME_ONLY(OK)
        listen_socket = steam->CreateListenSocketP2P(p_port, 0, nullptr);
        poll_group = steam->CreatePollGroup();
        active_mode = MODE_SERVER;
        connection_status = ConnectionStatus::CONNECTION_CONNECTED;
        unique_id = 1;
        return OK;
    }

    SteamNetworkingIdentity SteamMultiplayerPeer::get_peer(int64_t p_peer)
    {
        SteamNetworkingIdentity result;
        result.Clear();
        RUNTIME_ONLY(result)

        ERR_FAIL_COND_V_MSG(!_is_active(), result, "The multiplayer instance isn't currently active.");
        ERR_FAIL_COND_V_MSG(!id_to_connections.has(p_peer), result, "Peer is not connected.");

        result.SetSteamID64(static_cast<uint64_t>(p_peer));
        return result;
    }
}
