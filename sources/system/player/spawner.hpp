#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_spawner.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerSpawner : public godot::Node
    {
        UGDCLASS(PlayerSpawner, Node)

    private:
        godot::Node *players = nullptr;
        godot::Ref<godot::PackedScene> player_scene;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::Node *, players)
        IMPL_PROPERTY(godot::Ref<godot::PackedScene>, player_scene)

        virtual void _ready() override;

        void create_player(int id);
        void destroy_player(int id);
    };
}
