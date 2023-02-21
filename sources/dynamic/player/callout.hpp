#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerCallout : public Component
    {
        UGDCLASS(PlayerCallout, Component)

    private:
        godot::RayCast3D *raycast = nullptr;
        godot::Node3D *callout = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::RayCast3D *, raycast)
        IMPL_PROPERTY(godot::Node3D *, callout)

        void set_new_callout(godot::Vector3 p_position);

        virtual void _ready() override;
        virtual void _process(double delta) override;
    };
}
