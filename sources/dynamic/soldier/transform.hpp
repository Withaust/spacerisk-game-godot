#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <shared/components/transform.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class SoldierTransform : public Transform
    {
        UGDCLASS(SoldierTransform, Transform)

    private:
        godot::CharacterBody3D *body = nullptr;
        godot::Marker3D *head = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::CharacterBody3D *, body)
        IMPL_PROPERTY(godot::Marker3D *, head)

        virtual void _ready() override;
        virtual void _physics_process(double delta) override;

        virtual godot::Vector3 get_position() override;
        virtual void set_position(godot::Vector3 p_position) override;

        virtual godot::Vector3 get_rotation() override;
        virtual void set_rotation(godot::Vector3 p_rotation) override;

        virtual godot::Vector3 get_velocity() override;
        virtual void set_velocity(godot::Vector3 p_velocity) override;
    };
}
