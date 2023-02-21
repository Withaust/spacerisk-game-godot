#include <level/drill.hpp>

using namespace godot;

namespace spacerisk
{
    void Drill::_bind_methods()
    {
        BIND_PROPERTY(Drill, Variant::FLOAT, buffer);
        BIND_PROPERTY(Drill, Variant::FLOAT, collected);
        BIND_PROPERTY(Drill, Variant::FLOAT, step_generation);

        BIND_PROPERTY_PATH(Drill, Node3D, head);
        BIND_PROPERTY_PATH(Drill, Node3D, start_move);
        BIND_PROPERTY_PATH(Drill, Node3D, end_move);
        BIND_PROPERTY_PATH(Drill, Label3D, screen);
    }

    float Drill::get_collected()
    {
        return collected;
    }

    void Drill::set_collected(float p_collected)
    {
        screen->set_text(String::num_int64(UtilityFunctions::roundi(p_collected)));
    }

    void Drill::_ready()
    {
        RUNTIME_ONLY()

        AUTHORITY_ONLY()

        set_collected(collected);

        set_process_sleep(2.0f);
    }

    void Drill::_process(double delta)
    {
        RUNTIME_ONLY()

        if (start_to_end)
        {
            head->set_position(head->get_position().lerp(end_move->get_position(), 5.0f * delta));

            if (head->get_position().is_equal_approx(end_move->get_position()))
            {
                start_to_end = false;
            }
        }
        else
        {
            head->set_position(head->get_position().lerp(start_move->get_position(), 5.0f * delta));

            if (head->get_position().is_equal_approx(start_move->get_position()))
            {
                start_to_end = true;
            }
        }

        AUTHORITY_ONLY()

        PROCESS_SLEEP()

        collected += step_generation;
        set_collected(collected);
    }

}
