#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/expression.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Commander : public godot::Node
    {
        UGDCLASS(Commander, Node)

    private:
        godot::Ref<godot::Expression> expression;

    protected:
        static void _bind_methods();

    public:
        godot::Object *sys(godot::String p_system);

        virtual void _ready() override;
        godot::Variant execute(godot::String command);
    };
}
