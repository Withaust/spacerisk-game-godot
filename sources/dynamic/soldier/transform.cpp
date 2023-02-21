#include <dynamic/soldier/transform.hpp>

using namespace godot;

namespace spacerisk
{
    void SoldierTransform::_bind_methods()
    {
        BIND_PROPERTY_PATH(SoldierTransform, CharacterBody3D, body);
        BIND_PROPERTY_PATH(SoldierTransform, Marker3D, head);

        BIND_PROPERTY(SoldierTransform, Variant::VECTOR3, position);
        BIND_PROPERTY(SoldierTransform, Variant::VECTOR3, rotation);
    }

    void SoldierTransform::_ready()
    {
        RUNTIME_ONLY()

        Super::_ready();
    }

    void SoldierTransform::_physics_process(double delta)
    {
        RUNTIME_ONLY()

        Super::_physics_process(delta);

        NOT_AUTHORITY_ONLY()

        if (body->get_velocity().length() > 0.01)
        {
            body->move_and_slide();
        }
    }

    godot::Vector3 SoldierTransform::get_position()
    {
        RUNTIME_ONLY(Vector3())

        return body->get_global_position();
    }

    void SoldierTransform::set_position(godot::Vector3 p_position)
    {
        RUNTIME_ONLY()

        body->set_global_position(p_position);
    }

    godot::Vector3 SoldierTransform::get_rotation()
    {
        RUNTIME_ONLY(Vector3())

        Vector3 rotation;
        rotation.x = head->get_rotation().x;
        rotation.y = body->get_rotation().y;
        return rotation;
    }

    void SoldierTransform::set_rotation(godot::Vector3 p_rotation)
    {
        RUNTIME_ONLY()

        Vector3 head_rotation;
        head_rotation.x = p_rotation.x;
        head->set_rotation(head_rotation);

        Vector3 player_rotation;
        player_rotation.y = p_rotation.y;
        body->set_rotation(player_rotation);
    }

    Vector3 SoldierTransform::get_velocity()
    {
        RUNTIME_ONLY(Vector3())
        return body->get_velocity();
    }

    void SoldierTransform::set_velocity(godot::Vector3 p_velocity)
    {
        RUNTIME_ONLY()
        body->set_velocity(p_velocity);
    }
}
