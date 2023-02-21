#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/units/motion_generator.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class Transform : public Component
    {
        UGDCLASS(Transform, Component)

    private:
        godot::Ref<MotionGenerator> generator;
        // 0.05f - 20 ticks per second
        // 0.1f - 10 ticks per second
        float tps = 0.05f;
        float last_send = 0.0f;

    protected:
        static void _bind_methods();

    public:
        virtual void _ready() override;
        virtual void _physics_process(double delta) override;

        IMPL_PROPERTY(float, tps)

        void send_transform(float p_interpolation_time, godot::Vector3 p_position, godot::Vector3 p_velocity, godot::Vector3 p_rotation);

        virtual godot::Vector3 get_position() { return godot::Vector3(); }
        virtual void set_position(godot::Vector3 p_position) {}

        virtual godot::Vector3 get_rotation() { return godot::Vector3(); }
        virtual void set_rotation(godot::Vector3 p_rotation) {}

        virtual godot::Vector3 get_velocity() { return godot::Vector3(); }
        virtual void set_velocity(godot::Vector3 p_velocity) {};
    };
}
