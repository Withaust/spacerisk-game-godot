#pragma once

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

#include <godot_cpp/templates/hash_map.hpp>

#include <system/commander.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class InteractMenu : public godot::VBoxContainer
    {
        UGDCLASS(InteractMenu, VBoxContainer)

    private:
        godot::Label *option = nullptr;
        int selected_option = 0;
        void _select(int index);
        void _select_higher();
        void _select_lower();

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::Label *, option)

        virtual void _ready() override;
        virtual void _exit_tree() override;
        virtual void _process(double delta) override;

        void show(const godot::HashMap<godot::String, godot::Callable> &p_input);
        void hide();
    };
}
