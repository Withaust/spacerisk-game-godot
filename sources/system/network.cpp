#include <system/network.hpp>

using namespace godot;

namespace spacerisk
{
    void Network::start_network(bool p_server, String p_host_name)
    {
        enet_peer.instantiate();

        if (p_server)
        {
            enet_peer->create_server(40404);
            UtilityFunctions::print("Server listening on localhost port ", 40404);
        }
        else
        {
            enet_peer->create_client(p_host_name, 40404);
        }

        get_multiplayer()->set_multiplayer_peer(enet_peer);
    }

    void Network::start_network_steam(bool p_server, uint64_t p_host_steamid)
    {
        steam_peer.instantiate();
        steam_peer->set(GET_SYSTEM(Steam));

        if (p_server)
        {
            steam_peer->create_server();
            UtilityFunctions::print("Steam P2P server listening on localhost port ", 44);
        }
        else
        {
            steam_peer->create_client(p_host_steamid);
        }

        get_multiplayer()->set_multiplayer_peer(steam_peer);
    }

    void Network::_ready()
    {
        RUNTIME_ONLY()

        args = GET_SYSTEM(LaunchArgs);

        if (args->is_steam())
        {
            if (args->is_host())
            {
                start_network_steam(true, 0);
            }
            else
            {
                uint64_t target_steamid = static_cast<uint64_t>(args->get_host().to_int());
                if (target_steamid == GET_SYSTEM(Steam)->GetSteamID64())
                {
                    UtilityFunctions::printerr("Tried to connecting to ourself!. Abort!");
                    get_tree()->quit(1);
                    return;
                }
                else if (target_steamid == 0)
                {
                    UtilityFunctions::printerr("Invalid target STEAM_ID! Are you sure you changed launch args in VSCode's launch.json?");
                    get_tree()->quit(1);
                    return;
                }
                start_network_steam(false, target_steamid);
            }
        }
        else
        {
            if (args->is_host())
            {
                start_network(true, args->get_host());
            }
            else
            {
                start_network(false, args->get_host());
            }
        }
    }

    godot::Ref<godot::MultiplayerPeer> Network::get_any_peer()
    {
        return get_multiplayer()->get_multiplayer_peer();
    }

    const godot::Ref<godot::ENetMultiplayerPeer> Network::get_enet_peer()
    {
        if (args->is_steam())
        {
            return godot::Ref<ENetMultiplayerPeer>();
        }
        return enet_peer;
    }

    const godot::Ref<SteamMultiplayerPeer> Network::get_steam_peer()
    {
        if (!args->is_steam())
        {
            return godot::Ref<SteamMultiplayerPeer>();
        }
        return steam_peer;
    }
}
