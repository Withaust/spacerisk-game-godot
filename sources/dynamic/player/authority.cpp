#include <dynamic/player/autority.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerAuthority::_enter_tree()
    {
        get_parent()->set_multiplayer_authority(get_parent()->get_name().to_int(), true);
    }
}
