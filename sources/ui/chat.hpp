#pragma once

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/input.hpp>

#include <system/commander.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Chat : public godot::VBoxContainer
    {
        UGDCLASS(Chat, VBoxContainer)

    private:
        godot::VBoxContainer *chat_entries = nullptr;
        godot::LineEdit *input = nullptr;
        int max_message_count = 8;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(godot::VBoxContainer *, chat_entries)
        IMPL_PROPERTY(godot::LineEdit *, input)
        IMPL_PROPERTY(int, max_message_count)

        void on_parse_fail(godot::String p_error);
        void on_execute_fail();

        void player_joined(int id);
        void player_left(int id);

        virtual void _ready() override;
        void input_message(godot::String p_message);
        void print_message(godot::String p_message);
        void send_message(godot::String p_message);
    };
}
