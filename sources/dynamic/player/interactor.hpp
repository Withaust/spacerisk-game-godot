#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/static_body3d.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <system/ui.hpp>
#include <ui/interact_menu.hpp>

#include <dynamic/player/transform.hpp>

#include <shared/components/component.hpp>
#include <shared/components/interactable.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class PlayerInteractor : public Component
    {
        UGDCLASS(PlayerInteractor, Component)

    private:
        godot::RayCast3D *raycast = nullptr;
        Interactable *collision_target = nullptr;
        Transform *transform = nullptr;
        InteractMenu *interact_menu = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::RayCast3D *, raycast)

        void on_interact(godot::String option);
        virtual void _ready() override;
        virtual void _process(double delta) override;
    };
}
