#include <dynamic/player/movement.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerMovement::_bind_methods()
    {
        BIND_PROPERTY(PlayerMovement, Variant::FLOAT, sensitivity);
        BIND_PROPERTY(PlayerMovement, Variant::FLOAT, speed);
        BIND_PROPERTY(PlayerMovement, Variant::FLOAT, acceleration);
        BIND_PROPERTY(PlayerMovement, Variant::FLOAT, jump_velocity);
        BIND_PROPERTY(PlayerMovement, Variant::FLOAT, max_speed);
        BIND_PROPERTY(PlayerMovement, Variant::FLOAT, jump_height);

        BIND_PROPERTY_PATH(PlayerMovement, CharacterBody3D, body);
        BIND_PROPERTY_PATH(PlayerMovement, Marker3D, head);
        BIND_PROPERTY_PATH(PlayerMovement, AudioListener3D, listener);
        BIND_PROPERTY_PATH(PlayerMovement, Camera3D, camera);
        BIND_PROPERTY_PATH(PlayerMovement, CollisionShape3D, shape);
        BIND_PROPERTY_PATH(PlayerMovement, MeshInstance3D, mesh);
    }

    void PlayerMovement::_ready()
    {
        RUNTIME_ONLY()

        gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

        Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);

        if (is_multiplayer_authority())
        {
            camera->set_current(true);
            listener->make_current();
        }
        else
        {
            camera->set_current(false);
            shape->set_disabled(true);
            listener->clear_current();
        }
    }

    void PlayerMovement::_input(const Ref<InputEvent> &event)
    {
        RUNTIME_ONLY()

        AUTHORITY_ONLY()

        if (Input::get_singleton()->get_mouse_mode() != Input::MouseMode::MOUSE_MODE_CAPTURED)
        {
            return;
        }

        if (Input::get_singleton()->is_key_pressed(Key::KEY_ESCAPE))
        {
            Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
        }

        const InputEventMouseMotion *motion = cast_to<InputEventMouseMotion>(event.ptr());
        if (motion != nullptr)
        {
            body->rotate_y(-motion->get_relative().x * sensitivity);
            head->rotate_x(-motion->get_relative().y * sensitivity);

            Vector3 head_rot = head->get_rotation_degrees();
            head_rot.x = Math::clamp(head_rot.x, -89.9f, 89.9f);
            head->set_rotation_degrees(head_rot);
        }
    }

    void PlayerMovement::sprint()
    {
        if (Input::get_singleton()->is_action_just_pressed("move_sprint"))
        {
            speed = max_speed;
        }
    }

    void PlayerMovement::_physics_process(double delta)
    {
        RUNTIME_ONLY()

        AUTHORITY_ONLY()

        if (Input::get_singleton()->get_mouse_mode() != Input::MouseMode::MOUSE_MODE_CAPTURED)
        {
            return;
        }

        Vector3 velocity = body->get_velocity();

        if (!body->is_on_floor())
        {
            velocity.y -= gravity * delta;
        }

        if (Input::get_singleton()->is_action_pressed("move_jump") && body->is_on_floor())
        {
            velocity.y = jump_velocity;
        }

        Vector2 input_dir = Input::get_singleton()->get_vector("move_left", "move_right", "move_forward", "move_backward");
        input_dir = input_dir.normalized();

        Vector3 direction;

        if (input_dir != Vector2(0.0f, 0.0f))
        {
            direction.x = input_dir.x;
            direction.y = 0.0f;
            direction.z = input_dir.y;
            direction.rotate(Vector3(0.0f, 1.0f, 0.0f), body->get_rotation().y);
        }

        sprint();

        Vector3 motion = velocity;
        velocity = velocity.lerp(direction * speed, acceleration * delta);
        motion.x = velocity.x;
        motion.z = velocity.z;

        body->set_velocity(motion);
        body->move_and_slide();
    }
}
