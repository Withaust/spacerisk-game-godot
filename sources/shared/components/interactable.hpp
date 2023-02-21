#pragma once

#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/templates/hash_map.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace spacerisk
{
    class Interactable : public Component
    {
        UGDCLASS(Interactable, Component)

    private:
        godot::HashMap<godot::String, godot::Callable> options;

    protected:
        static void _bind_methods() {}

        void add_option(const godot::String &p_option_name, const godot::Callable &p_callable);
        void clear_option();

    public:
        const godot::HashMap<godot::String, godot::Callable> get_options();
    };
}
