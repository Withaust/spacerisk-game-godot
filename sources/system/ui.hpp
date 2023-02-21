#pragma once

#include <godot_cpp/classes/node.hpp>

#include <shared/util.hpp>
#include <shared/filesystem.hpp>

namespace spacerisk
{
    class UI : public godot::Node
    {
        UGDCLASS(UI, Node)

    private:
    protected:
        static void _bind_methods();

    public:
        virtual void _ready() override;
    };
}
