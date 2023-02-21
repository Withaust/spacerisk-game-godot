#include <ui/interact_menu.hpp>

using namespace godot;

namespace spacerisk
{
    void InteractMenu::_select(int index)
    {
        selected_option = index;
        get_child(selected_option)->get_node<ColorRect>("background")->set_modulate(Color(0.65f, 0.65f, 0.65f));
    }

    void InteractMenu::_select_higher()
    {
        get_child(selected_option)->get_node<ColorRect>("background")->set_modulate(Color(1.0f, 1.0f, 1.0f));
        selected_option--;
        if (selected_option < 0)
        {
            selected_option = get_child_count() - 1;
        }
        _select(selected_option);
    }

    void InteractMenu::_select_lower()
    {
        get_child(selected_option)->get_node<ColorRect>("background")->set_modulate(Color(1.0f, 1.0f, 1.0f));
        selected_option++;
        if (selected_option >= get_child_count())
        {
            selected_option = 0;
        }
        _select(selected_option);
    }

    void InteractMenu::_bind_methods()
    {
        BIND_PROPERTY_PATH(InteractMenu, Label, option);
        ADD_SIGNAL(MethodInfo("on_interact", PropertyInfo(Variant::STRING, "option")));
    }

    void InteractMenu::_ready()
    {
        RUNTIME_ONLY()

        remove_child(option);
    }

    void InteractMenu::_exit_tree()
    {
        RUNTIME_ONLY()

        option->queue_free();
    }

    void InteractMenu::_process(double delta)
    {
        RUNTIME_ONLY()

        if(get_child_count() == 0)
        {
            return;
        }

        if (Input::get_singleton()->is_action_just_released("ui_scroll_up"))
        {
            _select_higher();
        }
        else if (Input::get_singleton()->is_action_just_released("ui_scroll_down"))
        {
            _select_lower();
        }

        if (Input::get_singleton()->is_action_just_pressed("ui_interact"))
        {
            Label *target = cast_to<Label>(get_child(selected_option));
            if (target != nullptr)
            {
                emit_signal("on_interact", target->get_text());
            }
        }
    }

    void InteractMenu::show(const HashMap<String, Callable> &p_input)
    {
        hide();

        for (const auto &target : p_input)
        {
            Label *new_label = cast_to<Label>(option->duplicate());
            new_label->set_text(target.key);
            add_child(new_label);
        }

        _select(0);
    }

    void InteractMenu::hide()
    {
        for (int i = 0; i < get_child_count(); ++i)
        {
            get_child(i)->queue_free();
        }
    }
}
