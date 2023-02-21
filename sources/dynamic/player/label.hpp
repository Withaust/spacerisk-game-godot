#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/label3d.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerLabel : public Component
    {
        UGDCLASS(PlayerLabel, Component)

    private:
        godot::Label3D *label = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::Label3D *, label)

        virtual void _ready() override;
    };
}
