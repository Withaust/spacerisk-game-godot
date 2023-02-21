#pragma once

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

#include <shared/entity.hpp>
#include <shared/components/interactable.hpp>
#include <shared/components/transform.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class DoorInteractable : public Interactable
    {
        UGDCLASS(DoorInteractable, Interactable)

    public:
        enum OpenDirection
        {
            None,
            Forward,
            Backward
        };

    private:
        OpenDirection direction = OpenDirection::None;
        godot::Node3D *hinge = nullptr;
        godot::Vector3 rotation;
        float weight = 0.0f;
        float speed = 2.0f;
        float speed_up = 0.2f;

    protected:
        static void _bind_methods();

    public:
        OpenDirection get_direction();
        void set_direction(OpenDirection p_direction);
        void send_direction(OpenDirection p_direction);
        IMPL_PROPERTY(godot::Node3D *, hinge)
        IMPL_PROPERTY(float, speed)
        IMPL_PROPERTY(float, speed_up)

        virtual void _ready() override;
        virtual void _process(double delta) override;

        void try_open(Entity *p_caller);
    };
}

VARIANT_ENUM_CAST(spacerisk::DoorInteractable::OpenDirection)