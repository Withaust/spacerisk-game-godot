#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/os.hpp>

#include <shared/util.hpp>
#include <shared/filesystem.hpp>

namespace spacerisk
{
    class LaunchArgs : public godot::Node
    {
        GDCLASS(LaunchArgs, Node)

    private:
        static LaunchArgs *singleton;
        godot::String _target_host;
        bool _is_host;
        bool _steam;

        godot::String get_next_arg(int p_current_index, const godot::PackedStringArray &p_args);

    protected:
        static void _bind_methods();

    public:
        static LaunchArgs *get_singleton() { return singleton; }
        godot::String get_host();
        bool is_host();
        bool is_steam();

        virtual void _ready() override;
    };
}
