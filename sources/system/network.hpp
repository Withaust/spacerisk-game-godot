#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_spawner.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <godot_cpp/classes/e_net_connection.hpp>
#include <godot_cpp/classes/e_net_packet_peer.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <system/launch_args.hpp>
#include <system/units/steam_multiplayer_peer.hpp>

#include <system/steam.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Network : public godot::Node
    {
        UGDCLASS(Network, Node)
    private:
        LaunchArgs *args = nullptr;
        godot::Ref<godot::ENetMultiplayerPeer> enet_peer;
        godot::Ref<SteamMultiplayerPeer> steam_peer;

    protected:
        static void _bind_methods() {}

    public:
        void start_network(bool p_server, godot::String p_host_name);
        void start_network_steam(bool p_server, uint64_t p_host_steamid);

        virtual void _ready() override;

        godot::Ref<godot::MultiplayerPeer> get_any_peer();
        const godot::Ref<godot::ENetMultiplayerPeer> get_enet_peer();
        const godot::Ref<SteamMultiplayerPeer> get_steam_peer();
    };
}
