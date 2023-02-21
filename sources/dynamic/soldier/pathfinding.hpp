#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#include <dynamic/soldier/transform.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class SoldierPathfinding : public Component
    {
        UGDCLASS(SoldierPathfinding, Component)

    private:
        godot::NavigationAgent3D *agent = nullptr;
        godot::Node3D *raycasts_parent = nullptr;
        float movement_speed = 6.0f;
        float lerp_factor = 0.25f;
        float movement_delta = 0.0f;
        godot::Array raycasts;
        SoldierTransform *transform = nullptr;
        double physics_delta = 0.0f;
        float gravity = 0.0f;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::NavigationAgent3D *, agent)
        IMPL_PROPERTY(godot::Node3D *, raycasts_parent)
        IMPL_PROPERTY(float, movement_speed)
        IMPL_PROPERTY(float, lerp_factor)

        void set_movement_target(godot::Vector3 p_movement_target);
        void on_velocity_computed(godot::Vector3 p_safe_velocity);

        virtual void _ready() override;
        virtual void _process(double delta) override;
        virtual void _physics_process(double delta) override;
    };
}
