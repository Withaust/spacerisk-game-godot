#include <system/launch_args.hpp>

using namespace godot;

namespace spacerisk
{
    LaunchArgs *LaunchArgs::singleton = nullptr;

    void LaunchArgs::_bind_methods()
    {
    }

    String LaunchArgs::get_next_arg(int p_current_index, const PackedStringArray &p_args)
    {
        String result;
        int next_index = p_current_index + 1;
        if (next_index < 0 || next_index > p_args.size() - 1)
        {
            return result;
        }
        return p_args[next_index];
    }

    godot::String LaunchArgs::get_host()
    {
        return _target_host;
    }

    bool LaunchArgs::is_host()
    {
        return _is_host;
    }

    bool LaunchArgs::is_steam()
    {
        return _steam;
    }

    void LaunchArgs::_ready()
    {
        singleton = this;

        _target_host = "localhost";
        _is_host = true;
        _steam = true;

        PackedStringArray args = OS::get_singleton()->get_cmdline_args();

        for (int i = 0; i < args.size(); ++i)
        {
            String target = args[i];
            if (target == "--client")
            {
                String new_host = get_next_arg(i, args);
                if (new_host.is_valid_ip_address() || new_host.is_valid_int() || new_host == "localhost")
                {
                    _target_host = new_host;
                    _is_host = false;
                }
            }
            else if (target == "--nosteam")
            {
                _steam = false;
            }
        }
    }

}
