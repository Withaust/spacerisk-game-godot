#include <level/trigger_multiple.hpp>

using namespace godot;

namespace spacerisk
{
    void TriggerMultiple::_bind_methods()
    {
        BIND_PROPERTY_PATH(TriggerMultiple, Area3D, trigger);
        BIND_PROPERTY(TriggerMultiple, Variant::FLOAT, sleep_seconds);
        BIND_PROPERTY(TriggerMultiple, Variant::FLOAT, active_seconds);
    }

    void TriggerMultiple::_ready()
    {
        RUNTIME_ONLY()

        set_process_sleep_range(sleep_seconds, active_seconds);

        NOT_AUTHORITY_ONLY()
        queue_free();
    }

    void TriggerMultiple::on_process_active()
    {
        trigger->set_monitoring(true);
    }

    void TriggerMultiple::on_process_sleep()
    {
        trigger->set_monitoring(false);
    }

    void TriggerMultiple::_process(double delta)
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()
        if (!trigger->is_monitoring() || !trigger->has_overlapping_bodies())
        {
            PROCESS_SLEEP()
        }
    }
}
