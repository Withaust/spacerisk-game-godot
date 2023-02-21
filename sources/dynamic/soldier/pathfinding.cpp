#include <dynamic/soldier/pathfinding.hpp>

using namespace godot;

namespace spacerisk
{
    void SoldierPathfinding::_bind_methods()
    {
        BIND_PROPERTY_PATH(SoldierPathfinding, NavigationAgent3D, agent);
        BIND_PROPERTY_PATH(SoldierPathfinding, Node3D, raycasts_parent);
        BIND_PROPERTY(SoldierPathfinding, Variant::FLOAT, movement_speed);
        BIND_PROPERTY(SoldierPathfinding, Variant::FLOAT, lerp_factor);
        BIND_METHOD(SoldierPathfinding, on_velocity_computed);
    }

    void SoldierPathfinding::set_movement_target(Vector3 p_movement_target)
    {
        agent->set_target_position(p_movement_target);
    }

    void SoldierPathfinding::on_velocity_computed(godot::Vector3 p_safe_velocity)
    {
        CharacterBody3D *body = transform->get_body();
        body->set_velocity(body->get_velocity().move_toward(p_safe_velocity, lerp_factor));
        body->move_and_slide();
    }

    void SoldierPathfinding::_ready()
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()

        for (int i = 0; i < raycasts_parent->get_child_count(); ++i)
        {
            raycasts.push_back(cast_to<RayCast3D>(raycasts_parent->get_child(i)));
        }

        transform = get_entity()->get_component<SoldierTransform>();
        gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

        agent->connect("velocity_computed", Callable(this, "on_velocity_computed"));

        set_process_sleep(0.33f);
    }

    void SoldierPathfinding::_process(double delta)
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()
        PROCESS_SLEEP()

        int index = UtilityFunctions::randi_range(0, raycasts.size() - 1);
        RayCast3D *target = cast_to<RayCast3D>(raycasts[index]);
        if (!target->is_colliding())
        {
            return;
        }
        set_movement_target(target->get_collision_point());
    }

    void SoldierPathfinding::_physics_process(double delta)
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()

        Vector3 current_location = transform->get_position();
        Vector3 next_location = agent->get_next_path_position();
        Vector3 new_velocity = next_location - current_location;
        new_velocity.normalize();
        new_velocity *= movement_speed;
        agent->set_velocity(new_velocity);
    }
}
