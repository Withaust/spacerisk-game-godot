#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/classes/multiplayer_synchronizer.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerAuthority : public Component
    {
        UGDCLASS(PlayerAuthority, Component)

    protected:
        static void _bind_methods() {}

    public:
        virtual void _enter_tree() override;
    };
}
