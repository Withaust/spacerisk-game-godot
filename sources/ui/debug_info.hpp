#pragma once

#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <godot_cpp/classes/e_net_connection.hpp>
#include <godot_cpp/classes/e_net_packet_peer.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/performance.hpp>
#include <system/units/steam_multiplayer_peer.hpp>
#include <system/launch_args.hpp>
#include <system/network.hpp>

#include <vector>

#include <shared/util.hpp>

namespace spacerisk
{
    class DebugInfo : public godot::ColorRect
    {
        UGDCLASS(DebugInfo, ColorRect)
    private:
        enum ColoringType
        {
            HigherIsBetter = 0,
            LowerIsBetter,
        };

        struct GroupEntry
        {
            godot::String name;
            godot::String tip;
            ColoringType coloring;
        };

        struct ValueEntry
        {
            godot::String text;
            uint32_t value;
        };

        float timer = 0.0f;
        float timeout = 1.0f;
        int average_count = 8;

        LaunchArgs *args = nullptr;
        godot::Ref<godot::ENetMultiplayerPeer> enet_peer;
        godot::Ref<SteamMultiplayerPeer> steam_peer;
        godot::Ref<godot::MultiplayerAPI> multiplayer;
        godot::Performance *performance = nullptr;

        godot::VBoxContainer *contents = nullptr;
        godot::VBoxContainer *group = nullptr;
        godot::Label *group_entry_name = nullptr;
        godot::Label *group_entry_value = nullptr;

        void add_group(godot::String p_name, const std::vector<GroupEntry> &p_values);

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::VBoxContainer *, contents)
        IMPL_PROPERTY(godot::VBoxContainer *, group)
        IMPL_PROPERTY(godot::Label *, group_entry_name)
        IMPL_PROPERTY(godot::Label *, group_entry_value)

        void add_godot_metrics();
        void add_enet_metrics();
        void add_steam_metrics();

        void poll_godot_metrics();

        void poll();

        virtual void _ready() override;
        virtual void _exit_tree() override;
        virtual void _process(double delta) override;
    };
}
