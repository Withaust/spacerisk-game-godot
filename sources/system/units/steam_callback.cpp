#include <system/units/steam_callback.hpp>

using namespace godot;

namespace spacerisk
{
    void SteamCallback::_bind_methods()
    {
    }

    void SteamCallback::set(CallbackType p_type, uint64_t *p_pointer)
    {
        type = p_type;
        pointer = p_pointer;
    }

    SteamCallback::CallbackType SteamCallback::get_type()
    {
        return type;
    }
}
