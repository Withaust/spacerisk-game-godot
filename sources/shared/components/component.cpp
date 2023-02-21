#include <shared/components/component.hpp>

using namespace godot;

namespace spacerisk
{
    void Component::_bind_methods()
    {
        BIND_VIRTUAL_METHOD(Component, on_process_active)
        BIND_VIRTUAL_METHOD(Component, on_process_sleep)
        BIND_VIRTUAL_METHOD(Component, on_physics_active)
        BIND_VIRTUAL_METHOD(Component, on_physics_sleep)
    }

    void Component::set_process_sleep(float p_tps)
    {
        process_timeout = 1.0f / p_tps;
        process_timer = process_timeout + 1.0f;
    }

    void Component::set_process_sleep_range(float p_sleep_seconds, float p_active_seconds)
    {
        process_span = p_active_seconds;
        process_timeout = p_sleep_seconds;
    }

    void Component::set_physics_sleep(float p_tps)
    {
        physics_timeout = 1.0f / p_tps;
        physics_timer = physics_timeout + 1.0f;
    }

    void Component::set_physics_sleep_range(float p_sleep_seconds, float p_active_seconds)
    {
        physics_span = p_active_seconds;
        physics_timeout = p_sleep_seconds;
    }

    bool Component::process_sleep(double p_delta)
    {
        // Acting as sleep with range
        if (process_span != 0.0f)
        {
            process_timer += p_delta;
            if (process_sleeping)
            {
                if (process_timer >= process_timeout)
                {
                    process_timer = 0.0f;
                    on_process_active();
                    process_sleeping = false;
                    return true;
                }
            }
            else
            {
                if (process_timer >= process_span)
                {
                    process_timer = 0.0f;
                    on_process_sleep();
                    process_sleeping = true;
                }
            }
            return false;
        }
        // Acting as a singular sleep
        else
        {
            process_timer += p_delta;
            if (process_timer >= process_timeout)
            {
                process_timer = 0.0f;
                return true;
            }
            return false;
        }
    }

    bool Component::physics_sleep(double p_delta)
    {
        // Acting as sleep with range
        if (physics_span != 0.0f)
        {
            physics_timer += p_delta;
            if (physics_sleeping)
            {
                if (physics_timer >= physics_timeout)
                {
                    physics_timer = 0.0f;
                    on_physics_active();
                    physics_sleeping = false;
                    return true;
                }
            }
            else
            {
                if (physics_timer >= physics_span)
                {
                    physics_timer = 0.0f;
                    on_physics_sleep();
                    physics_sleeping = true;
                }
            }
            return false;
        }
        // Acting as a singular sleep
        else
        {
            physics_timer += p_delta;
            if (physics_timer >= physics_timeout)
            {
                physics_timer = 0.0f;
                return true;
            }
            return false;
        }
    }

    Entity *Component::get_entity()
    {
        if (entity == nullptr)
        {
            entity = get_entity(this);
        }
        return entity;
    }

    Entity *Component::get_entity(Node *p_target)
    {
        if (p_target == nullptr)
        {
            return nullptr;
        }

        String target_name = "Entity";

        if (p_target->is_class(target_name))
        {
            return cast_to<Entity>(p_target);
        }

        while (true)
        {
            Node *parent = p_target->get_parent();
            if (parent == nullptr)
            {
                return nullptr;
            }
            p_target = parent;
            if (p_target->is_class(target_name))
            {
                return cast_to<Entity>(p_target);
            }
        }

        return nullptr;
    }
}
