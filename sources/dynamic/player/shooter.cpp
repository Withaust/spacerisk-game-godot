#include <dynamic/player/shooter.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerShooter::_bind_methods()
    {
        BIND_PROPERTY_PATH(PlayerShooter, RayCast3D, raycast);
        BIND_PROPERTY_PATH(PlayerShooter, Node3D, decal);
    }

    void PlayerShooter::_ready()
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()

        remove_child(decal);
    }

    void PlayerShooter::_exit_tree()
    {
        RUNTIME_ONLY()
        decal->queue_free();
    }

    void PlayerShooter::_process(double delta)
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()

        if (Input::get_singleton()->is_action_just_pressed("shoot") && raycast->is_colliding())
        {
            if (decals.size() >= 4)
            {
                Node3D *target = cast_to<Node3D>(decals[0]);
                if (UtilityFunctions::is_instance_valid(target))
                {
                    target->queue_free();
                }
                decals.pop_front();
            }

            Node3D *collider = cast_to<Node3D>(raycast->get_collider());
            Node3D *new_decal = cast_to<Node3D>(decal->duplicate());
            new_decal->set_visible(true);
            new_decal->set_disable_scale(true);
            collider->add_child(new_decal);
            new_decal->set_global_position(raycast->get_collision_point());
            if (Math::abs(raycast->get_collision_normal().y) > 0.99f)
            {
                new_decal->look_at(raycast->get_collision_point() + raycast->get_collision_normal(), Vector3(0.0f, 0.0f, 1.0f));
            }
            else
            {
                new_decal->look_at(raycast->get_collision_point() + raycast->get_collision_normal());
            }
            new_decal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), -Math_PI / 2.0f);
            decals.push_back(new_decal);
        }
    }
}
