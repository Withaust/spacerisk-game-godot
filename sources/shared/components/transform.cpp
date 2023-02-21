#include <shared/components/transform.hpp>

using namespace godot;

namespace spacerisk
{
    void Transform::_bind_methods()
    {
        BIND_PROPERTY(Transform, Variant::FLOAT, tps);
        BIND_METHOD(Transform, send_transform);
    }

    void Transform::_ready()
    {
        RUNTIME_ONLY()

        generator.instantiate();

        Dictionary config;
        config["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
        config["transfer_mode"] = MultiplayerPeer::TransferMode::TRANSFER_MODE_UNRELIABLE;
        config["call_local"] = false;
        config["channel"] = 0;
        rpc_config("send_transform", config);
    }

    void Transform::_physics_process(double delta)
    {
        RUNTIME_ONLY()
        if (is_multiplayer_authority())
        {
            // Local movement.
            // Sync position.
            last_send += delta;
            if (last_send >= tps)
            {
                rpc("send_transform", last_send, get_position(), get_velocity(), get_rotation());
                last_send = 0.0f;
            }
        }
        else
        {
            // Proxy movement.

            // Avoid warnings if syncBuffer is empty.
            if (generator->has_frames())
            {
                // Update playback and apply new position.
                generator->update_playback(delta);
                set_position(generator->get_position());
                set_rotation(generator->get_rotation().get_euler());
            }
        }
    }

    void Transform::send_transform(float p_interpolation_time, Vector3 p_position, Vector3 p_velocity, Vector3 p_rotation)
    {
        generator->add_frame(p_interpolation_time, p_position, p_velocity, p_rotation);
    }
}
