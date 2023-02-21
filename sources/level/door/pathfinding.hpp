#pragma once

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class DoorPathfinding : public Component
    {
        UGDCLASS(DoorPathfinding, Component)

    private:
        godot::CollisionShape3D *collider = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::CollisionShape3D *, collider)

        virtual void _ready() override;
    };
}
