#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerShooter : public Component
    {
        UGDCLASS(PlayerShooter, Component)

    private:
        godot::RayCast3D *raycast = nullptr;
        godot::Node3D *decal = nullptr;
        godot::Array decals;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::RayCast3D *, raycast)
        IMPL_PROPERTY(godot::Node3D *, decal)

        virtual void _ready() override;
        virtual void _exit_tree() override;
        virtual void _process(double delta) override;
    };
}
