#include <dynamic/player/label.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerLabel::_bind_methods()
    {
        BIND_PROPERTY_PATH(PlayerLabel, Label3D, label);
    }

    void PlayerLabel::_ready()
    {
        RUNTIME_ONLY()

        if (!is_multiplayer_authority())
        {
            label->set_text(String::num_int64(get_multiplayer_authority()));
        }
    }
}
