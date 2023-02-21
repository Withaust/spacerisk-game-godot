#include <dynamic/player/transform.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerTransform::_bind_methods()
    {
        BIND_PROPERTY_PATH(PlayerTransform, CharacterBody3D, player);
        BIND_PROPERTY_PATH(PlayerTransform, Marker3D, player_head);

        BIND_PROPERTY(PlayerTransform, Variant::VECTOR3, position);
        BIND_PROPERTY(PlayerTransform, Variant::VECTOR3, rotation);
    }

    void PlayerTransform::_ready()
    {
        RUNTIME_ONLY()

        Super::_ready();
    }

    void PlayerTransform::_physics_process(double delta)
    {
        RUNTIME_ONLY()

        Super::_physics_process(delta);

        NOT_AUTHORITY_ONLY()

        if(player->get_velocity().length() > 0.01)
        {
            player->move_and_slide();
        }
    }

    Vector3 PlayerTransform::get_position()
    {
        RUNTIME_ONLY(Vector3())

        return player->get_position();
    }

    void PlayerTransform::set_position(Vector3 p_position)
    {
        RUNTIME_ONLY()

        player->set_position(p_position);
    }

    Vector3 PlayerTransform::get_rotation()
    {
        RUNTIME_ONLY(Vector3())

        Vector3 rotation;
        rotation.x = player_head->get_rotation().x;
        rotation.y = player->get_rotation().y;
        return rotation;
    }

    void PlayerTransform::set_rotation(Vector3 p_rotation)
    {
        RUNTIME_ONLY()

        Vector3 head_rotation;
        head_rotation.x = p_rotation.x;
        player_head->set_rotation(head_rotation);

        Vector3 player_rotation;
        player_rotation.y = p_rotation.y;
        player->set_rotation(player_rotation);
    }

    Vector3 PlayerTransform::get_velocity()
    {
        RUNTIME_ONLY(Vector3())
        return player->get_velocity();
    }

    void PlayerTransform::set_velocity(godot::Vector3 p_velocity)
    {
        RUNTIME_ONLY()
        player->set_velocity(p_velocity);
    }
}
