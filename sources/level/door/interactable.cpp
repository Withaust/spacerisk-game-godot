#include <level/door/interactable.hpp>

using namespace godot;

namespace spacerisk
{
    void DoorInteractable::_bind_methods()
    {
        BIND_ENUM_CONSTANT(None)
        BIND_ENUM_CONSTANT(Forward)
        BIND_ENUM_CONSTANT(Backward)

        BIND_METHOD(DoorInteractable, send_direction);
        BIND_METHOD(DoorInteractable, set_direction);
        BIND_METHOD(DoorInteractable, get_direction);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "direction", PROPERTY_HINT_ENUM, "None,Forward,Backward"), "set_direction", "get_direction");

        BIND_PROPERTY_PATH(DoorInteractable, Node3D, hinge);

        BIND_METHOD(DoorInteractable, try_open);

        BIND_PROPERTY(DoorInteractable, Variant::FLOAT, speed);
        BIND_PROPERTY(DoorInteractable, Variant::FLOAT, speed_up);
    }

    DoorInteractable::OpenDirection DoorInteractable::get_direction()
    {
        return direction;
    }

    void DoorInteractable::set_direction(DoorInteractable::OpenDirection p_direction)
    {
        direction = p_direction;

        RUNTIME_ONLY()

        weight = speed;

        if (direction == OpenDirection::Forward)
        {
            rotation = Vector3(0.0f, 90.0f, 0.0f);
        }
        else if (direction == OpenDirection::Backward)
        {
            rotation = Vector3(0.0f, -90.0f, 0.0f);
        }
        else
        {
            rotation = Vector3(0.0f, 0.0f, 0.0f);
        }
    }

    void DoorInteractable::send_direction(DoorInteractable::OpenDirection p_direction)
    {
        set_direction(p_direction);
    }

    void DoorInteractable::_ready()
    {
        RUNTIME_ONLY()

        add_option("open", Callable(this, "try_open"));

        Dictionary config;
        config["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
        config["transfer_mode"] = MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE;
        config["call_local"] = true;
        config["channel"] = 0;
        rpc_config("send_direction", config);

        if (is_multiplayer_authority())
        {
            rpc_id(MultiplayerPeer::TARGET_PEER_SERVER, "send_direction", direction);
        }
    }

    void DoorInteractable::_process(double delta)
    {
        RUNTIME_ONLY()

        if (!hinge->get_rotation_degrees().is_equal_approx(rotation))
        {
            weight += speed_up;
            hinge->set_rotation_degrees(hinge->get_rotation_degrees().lerp(rotation, delta * weight));
        }
    }

    void DoorInteractable::try_open(Entity *p_opener)
    {
        Vector3 position;
        Transform *transform = p_opener->get_component<Transform>();
        if (transform == nullptr)
        {
            position = p_opener->get_position();
        }
        else
        {
            position = transform->get_position();
        }

        if (direction == OpenDirection::None)
        {
            if (get_entity()->to_local(position).x > 0.0f)
            {
                rpc_id(MultiplayerPeer::TARGET_PEER_SERVER, "send_direction", OpenDirection::Backward);
            }
            else
            {
                rpc_id(MultiplayerPeer::TARGET_PEER_SERVER, "send_direction", OpenDirection::Forward);
            }
        }
        else
        {
            rpc_id(MultiplayerPeer::TARGET_PEER_SERVER, "send_direction", OpenDirection::None);
        }
    }
}
