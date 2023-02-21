#include <system/commander.hpp>

using namespace godot;

namespace spacerisk
{
    void Commander::_bind_methods()
    {
        ADD_SIGNAL(MethodInfo("parse_fail", PropertyInfo(Variant::STRING, "error")));
        ADD_SIGNAL(MethodInfo("execute_fail"));

        BIND_METHOD(Commander, sys);
    }

    Object *Commander::sys(String p_system)
    {
        return get_node<Node>("/root/" + p_system + "System");
    }

    void Commander::_ready()
    {
        RUNTIME_ONLY()

        expression.instantiate();
    }

    Variant Commander::execute(godot::String command)
    {
        Error error = expression->parse(command);
        if (error != OK)
        {
            emit_signal("parse_fail", expression->get_error_text());
            return Variant();
        }

        Variant result = expression->execute(Array(), this, true, false);
        if (expression->has_execute_failed())
        {
            emit_signal("execute_fail");
            return Variant();
        }

        return result;
    }
}
