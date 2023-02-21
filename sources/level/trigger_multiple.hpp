#pragma once

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <shared/entity.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class TriggerMultiple : public Component
    {
        UGDCLASS(TriggerMultiple, Component)

    private:
        godot::Area3D *trigger = nullptr;
        float sleep_seconds = 1.0f;
        float active_seconds = 1.0f;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::Area3D *, trigger)
        IMPL_PROPERTY(float, sleep_seconds)
        IMPL_PROPERTY(float, active_seconds)

        virtual void _ready() override;
        virtual void on_process_active() override;
        virtual void on_process_sleep() override;
        virtual void _process(double delta) override;
    };
}