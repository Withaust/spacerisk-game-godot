#pragma once

#include <godot_cpp/classes/node.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Info : public godot::Node
    {
        UGDCLASS(Info, Node)

    private:
        godot::Dictionary player_info;

    protected:
        static void _bind_methods();

    public:
        void send_player_info(godot::Dictionary player_info);
    };
}
