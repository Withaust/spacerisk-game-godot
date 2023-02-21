#pragma once

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>

#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/list.hpp>

#include <system/steam.hpp>
#include <system/units/steam_callback.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class SteamMultiplayerPeer : public godot::MultiplayerPeerExtension
    {
        UGDCLASS(SteamMultiplayerPeer, MultiplayerPeerExtension)

    private:
        Steam *steam = nullptr;

        enum Mode
        {
            MODE_NONE,
            MODE_SERVER,
            MODE_CLIENT,
        };

        Mode active_mode = MODE_NONE;

        int32_t unique_id = 0;
        int32_t target_peer = 0;

        godot::MultiplayerPeer::ConnectionStatus connection_status = CONNECTION_DISCONNECTED;

        godot::HashMap<int64_t, HSteamNetConnection> id_to_connections;
        godot::HashMap<HSteamNetConnection, int64_t> connections_to_id;

        HSteamListenSocket listen_socket;
        HSteamNetPollGroup poll_group;

        SteamNetworkingMessage_t **messages_buffer;
        int max_messages_buffer = 64;

        struct Packet
        {
            SteamNetworkingMessage_t *packet = nullptr;
            int32_t from = 0;
            int32_t channel = 0;
            godot::MultiplayerPeer::TransferMode transfer_mode = TRANSFER_MODE_RELIABLE;
        };

        godot::List<Packet> incoming_packets;

        Packet current_packet;

        bool refuse_new_connections = false;
        int transfer_channel = 0;
        godot::MultiplayerPeer::TransferMode transfer_mode = TRANSFER_MODE_RELIABLE;

        int HOST_DEFAULT_MTU = 1200;

        _FORCE_INLINE_ bool _is_active() const { return active_mode != MODE_NONE; }

    protected:
        static void _bind_methods();

    public:
        // Callbacks
        void steam_net_connection_status_changed(godot::Ref<SteamCallback> p_callback);
        void set(Steam *p_steam);
        // TODO Fix entire implementation to rely on the unique int32_t ids as handles instead of uint64_t SteamIDs
        virtual godot::Error _get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) override;
        virtual godot::Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) override;
        virtual int32_t _get_available_packet_count() const override;
        virtual int32_t _get_max_packet_size() const override;
        virtual godot::PackedByteArray _get_packet_script() override;
        virtual godot::Error _put_packet_script(const godot::PackedByteArray &p_buffer) override;
        virtual void _set_transfer_channel(int32_t p_channel) override;
        virtual int32_t _get_transfer_channel() const override;
        virtual void _set_transfer_mode(godot::MultiplayerPeer::TransferMode p_mode) override;
        virtual godot::MultiplayerPeer::TransferMode _get_transfer_mode() const override;
        virtual void _set_target_peer(int32_t p_peer) override;
        virtual int32_t _get_packet_peer() const override;
        virtual bool _is_server() const override;
        virtual void _poll() override;
        virtual void _close() override;
        virtual void _disconnect_peer(int32_t p_peer, bool p_force = true) override;
        virtual int32_t _get_unique_id() const override;
        virtual void _set_refuse_new_connections(bool p_enable) override;
        virtual bool _is_refusing_new_connections() const override;
        virtual godot::MultiplayerPeer::ConnectionStatus _get_connection_status() const override;
        godot::Error create_client(uint64_t p_host_steamid, int p_port = 44);
        godot::Error create_server(int p_port = 44, int p_max_clients = 32);
        SteamNetworkingIdentity get_peer(int64_t p_peer);

        SteamMultiplayerPeer();
        ~SteamMultiplayerPeer();
    };
}
