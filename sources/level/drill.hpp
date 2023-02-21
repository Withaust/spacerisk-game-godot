#pragma once

#include <godot_cpp/classes/label3d.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class Drill : public Component
    {
        UGDCLASS(Drill, Component)

    private:
        float buffer = 36000.0f;
        float collected = 1000.0f;
        float step_generation = 36.0f;
        godot::Node3D *head = nullptr;
        godot::Node3D *start_move = nullptr;
        godot::Node3D *end_move = nullptr;
        godot::Label3D *screen = nullptr;

        bool start_to_end = true;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(float, buffer)
        IMPL_PROPERTY(float, step_generation)

        float get_collected();
        void set_collected(float p_collected);

        IMPL_PROPERTY(godot::Node3D *, head)
        IMPL_PROPERTY(godot::Node3D *, start_move)
        IMPL_PROPERTY(godot::Node3D *, end_move)
        IMPL_PROPERTY(godot::Label3D *, screen)

        virtual void _ready() override;
        virtual void _process(double delta) override;
    };
}
