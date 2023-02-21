#include <dynamic/player/callout.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerCallout::_bind_methods()
    {
        BIND_PROPERTY_PATH(PlayerCallout, RayCast3D, raycast);
        BIND_PROPERTY_PATH(PlayerCallout, Node3D, callout);

        BIND_METHOD(PlayerCallout, set_new_callout);
    }

    void PlayerCallout::set_new_callout(Vector3 p_position)
    {
        callout->set_visible(true);
        callout->set_position(p_position);
    }

    void PlayerCallout::_ready()
    {
        RUNTIME_ONLY()

        Dictionary rpc;
        rpc["rpc_mode"] = MultiplayerAPI::RPC_MODE_AUTHORITY;
        rpc["transfer_mode"] = MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE;
        rpc["call_local"] = true;
        rpc["channel"] = 0;
        rpc_config("set_new_callout", rpc);
    }

    void PlayerCallout::_process(double delta)
    {
        RUNTIME_ONLY()

        AUTHORITY_ONLY()

        if (!Input::get_singleton()->is_action_just_pressed("ui_callout"))
        {
            return;
        }

        if (!raycast->is_colliding())
        {
            return;
        }

        rpc("set_new_callout", raycast->get_collision_point());
    }
}
