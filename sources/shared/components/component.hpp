#pragma once

#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/templates/hash_set.hpp>

#include <shared/entity.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Entity;

    class Component : public godot::Node
    {
        UGDCLASS(Component, Node)

    private:
        float process_span = 0.0f;
        float process_timeout = 0.0f;
        float process_timer = 0.0f;
        bool process_sleeping = false;

        float physics_span = 0.0f;
        float physics_timeout = 0.0f;
        float physics_timer = 0.0f;
        bool physics_sleeping = false;

        Entity *entity = nullptr;

    protected:
        static void _bind_methods();

        // You should probably only call set_ methods below only for one process/physics method in mind
        // and not change sleep type back and forward since it would be quite confusing

        // p_tps - how many times per second should the sleeping function execute
        // 2.0f - 2 times per second
        // 0.5f - 1 time every 2 seconds
        void set_process_sleep(float p_tps = 1.0f);
        // p_sleep_seconds - for how many seconds should the function sleep
        // p_active_seconds - for how many seconds should the function run
        // For example, if we want to have a trigger that would enable itself every 3 second, and
        // would be working for 0.5 seconds, params would be set_process_sleep_range(3.0f, 0.5f)
        void set_process_sleep_range(float p_sleep_seconds = 1.0f, float p_active_seconds = 1.0f);
        // p_tps - how many times per second should the sleeping function execute
        // 2.0f - 2 times per second
        // 0.5f - 1 time every 2 seconds
        void set_physics_sleep(float p_tps = 1.0f);
        // p_sleep_seconds - for how many seconds should the function sleep
        // p_active_seconds - for how many seconds should the function run
        // For example, if we want to have a trigger that would enable itself every 3 second, and
        // would be working for 0.5 seconds, params would be set_physics_sleep_range(3.0f, 0.5f)
        void set_physics_sleep_range(float p_sleep_seconds = 1.0f, float p_active_seconds = 1.0f);

        bool process_sleep(double p_delta);
        bool physics_sleep(double p_delta);

        virtual void on_process_active(){};
        virtual void on_process_sleep(){};

        virtual void on_physics_active(){};
        virtual void on_physics_sleep(){};

        Entity *get_entity();
        Entity *get_entity(godot::Node *p_target);
    };
}
