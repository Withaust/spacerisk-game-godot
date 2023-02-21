#include <system/player/spawner.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerSpawner::_bind_methods()
    {
        BIND_PROPERTY_PATH(PlayerSpawner, Node, players);
        BIND_PROPERTY_PATH_RESOURCE(PlayerSpawner, PackedScene, player_scene);

        BIND_METHOD(PlayerSpawner, create_player);
        BIND_METHOD(PlayerSpawner, destroy_player);
    }

    void PlayerSpawner::_ready()
    {
        RUNTIME_ONLY()

        if (get_multiplayer()->is_server())
        {
            get_multiplayer()->connect("peer_connected", Callable(this, "create_player"));
            get_multiplayer()->connect("peer_disconnected", Callable(this, "destroy_player"));

            create_player(1);
        }
    }

    void PlayerSpawner::create_player(int id)
    {
        RUNTIME_ONLY()
        Node *NewPlayer = player_scene->instantiate();
        NewPlayer->set_name(String::num_int64(id));
        NewPlayer->set_multiplayer_authority(id, true);
        players->add_child(NewPlayer);
    }

    void PlayerSpawner::destroy_player(int id)
    {
        RUNTIME_ONLY()
        players->get_node<Node>(String::num_int64(id))->queue_free();
    }
}
