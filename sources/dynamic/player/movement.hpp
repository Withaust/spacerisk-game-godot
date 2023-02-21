#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/audio_listener3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerMovement : public Component
    {
        UGDCLASS(PlayerMovement, Component)

    protected:
        static void _bind_methods();

    private:
        float sensitivity = 0.006f;
        float speed = 6.0f;
        float acceleration = 10.0f;
        float jump_velocity = 4.5f;
        float max_speed = 8.0f;
        float jump_height = 4.0f;

        float gravity = 0.0f;

        godot::CharacterBody3D *body = nullptr;
        godot::Marker3D *head = nullptr;
        godot::Camera3D *camera = nullptr;
        godot::AudioListener3D *listener = nullptr;
        godot::CollisionShape3D *shape = nullptr;
        godot::MeshInstance3D *mesh = nullptr;

    public:
        IMPL_PROPERTY(float, sensitivity)
        IMPL_PROPERTY(float, speed)
        IMPL_PROPERTY(float, acceleration)
        IMPL_PROPERTY(float, jump_velocity)
        IMPL_PROPERTY(float, max_speed)
        IMPL_PROPERTY(float, jump_height)

        IMPL_PROPERTY(godot::CharacterBody3D *, body)
        IMPL_PROPERTY(godot::Marker3D *, head)
        IMPL_PROPERTY(godot::Camera3D *, camera)
        IMPL_PROPERTY(godot::AudioListener3D *, listener)
        IMPL_PROPERTY(godot::CollisionShape3D *, shape)
        IMPL_PROPERTY(godot::MeshInstance3D *, mesh)

        virtual void _ready() override;
        virtual void _input(const godot::Ref<godot::InputEvent> &event) override;
        void sprint();
        virtual void _physics_process(double delta) override;
    };
}
