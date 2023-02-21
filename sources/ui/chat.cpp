#include <ui/chat.hpp>

using namespace godot;

namespace spacerisk
{
    void Chat::_ready()
    {
        RUNTIME_ONLY()

        GET_SYSTEM(Commander)->connect("parse_fail", Callable(this, "on_parse_fail"));
        GET_SYSTEM(Commander)->connect("execute_fail", Callable(this, "on_execute_fail"));

        Dictionary config;
        config["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
        config["transfer_mode"] = MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE;
        config["call_local"] = true;
        config["channel"] = 0;
        rpc_config("send_message", config);

        input->connect("text_submitted", Callable(this, "input_message"));

        get_multiplayer()->connect("peer_connected", Callable(this, "player_joined"));
        get_multiplayer()->connect("peer_disconnected", Callable(this, "player_left"));
    }

    void Chat::_bind_methods()
    {
        BIND_PROPERTY_PATH(Chat, VBoxContainer, chat_entries);
        BIND_PROPERTY_PATH(Chat, LineEdit, input);
        BIND_PROPERTY(Chat, Variant::INT, max_message_count);

        BIND_METHOD(Chat, on_parse_fail);
        BIND_METHOD(Chat, on_execute_fail);

        BIND_METHOD(Chat, player_joined);
        BIND_METHOD(Chat, player_left);

        BIND_METHOD(Chat, input_message);
        BIND_METHOD(Chat, print_message);
        BIND_METHOD(Chat, send_message);
    }

    void Chat::on_parse_fail(godot::String p_error)
    {
        print_message("Failed to parse last expression with error: " + p_error);
    }

    void Chat::on_execute_fail()
    {
        print_message("Failed to execute last expression.");
    }

    void Chat::player_joined(int id)
    {
        print_message(String::num_int64(id) + " has joined the game");
    }

    void Chat::player_left(int id)
    {
        print_message(String::num_int64(id) + " has left the game");
    }

    void Chat::input_message(godot::String p_message)
    {
        RUNTIME_ONLY()

        input->clear();
        input->release_focus();
        Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);

        if (p_message.begins_with("/"))
        {
            print_message(p_message);
            String command_message = ">" + GET_SYSTEM(Commander)->execute(p_message.right(-1)).stringify();
            print_message(command_message);
        }
        else
        {
            rpc("send_message", p_message);
        }
    }

    void Chat::print_message(godot::String p_message)
    {
        RUNTIME_ONLY()

        if (chat_entries->get_child_count() > max_message_count)
        {
            chat_entries->get_child(0)->queue_free();
        }

        Label *new_message = new Label();
        new_message->set("theme_override_colors/font_outline_color", Color(0.0f, 0.0f, 0.0f, 1.0f));
        new_message->set("theme_override_constants/outline_size", 2);
        new_message->set_text(p_message);
        chat_entries->add_child(new_message);
    }

    void Chat::send_message(godot::String p_message)
    {
        RUNTIME_ONLY()

        print_message(String::num_int64(get_multiplayer()->get_remote_sender_id()) + ": " + p_message);
    }
}
