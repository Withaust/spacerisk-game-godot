#include <ui/debug_info.hpp>

using namespace godot;

namespace spacerisk
{
    void DebugInfo::_bind_methods()
    {
        BIND_PROPERTY_PATH(DebugInfo, VBoxContainer, contents);
        BIND_PROPERTY_PATH(DebugInfo, VBoxContainer, group);
        BIND_PROPERTY_PATH(DebugInfo, Label, group_entry_name);
        BIND_PROPERTY_PATH(DebugInfo, Label, group_entry_value);
    }

    void DebugInfo::add_group(String p_name, const std::vector<GroupEntry> &p_values)
    {
        VBoxContainer *new_group = cast_to<VBoxContainer>(group->duplicate());
        new_group->get_node<Label>("name")->set_text(p_name);
        new_group->set_name(p_name);

        VBoxContainer *names = new_group->get_node<VBoxContainer>("variables/names");
        VBoxContainer *values = new_group->get_node<VBoxContainer>("variables/values");

        for (const auto &variable : p_values)
        {
            String tooltip = variable.tip;
            if (variable.coloring == ColoringType::HigherIsBetter)
            {
                tooltip += "\nHigher is better.";
            }
            else
            {
                tooltip += "\nLower is better.";
            }
            Label *new_name = cast_to<Label>(group_entry_name->duplicate());
            new_name->set_name(variable.name);
            new_name->set_text(String(variable.name) + ":");
            new_name->set_meta("coloring", variable.coloring);
            new_name->set_tooltip_text(tooltip);
            names->add_child(new_name);
            Label *new_value = cast_to<Label>(group_entry_value->duplicate());
            new_value->set_name(variable.name);
            new_value->set_text("0");
            new_value->set_tooltip_text(tooltip);
            values->add_child(new_value);
        }

        contents->add_child(new_group);
    }

    void DebugInfo::add_godot_metrics()
    {
        std::vector<GroupEntry> values;
        GroupEntry entry;

        entry.name = "FPS";
        entry.coloring = ColoringType::HigherIsBetter;
        entry.tip = "The number of frames rendered in the last second.";
        values.push_back(entry);

        entry.name = "Process Time";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Time it took to complete one frame, in milliseconds.";
        values.push_back(entry);

        entry.name = "Physics Time";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Time it took to complete one physics frame, in milliseconds.";
        values.push_back(entry);

        entry.name = "Navigation Time";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Time it took to complete one navigation step, in milliseconds.\n") +
                    "This includes navigation map updates as well as agent avoidance calculations.";
        values.push_back(entry);

#ifdef DEBUG_ENABLED
        entry.name = "Memory static";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Static memory currently used, in bytes.\n") +
                    "Not available in release builds.";
        values.push_back(entry);

        entry.name = "Memory static max";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Available static memory.\n") +
                    "Not available in release builds.";
        values.push_back(entry);
#endif

        entry.name = "Message buffer max";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Largest amount of memory the message queue buffer has used, in bytes.\n") +
                    "The message queue is used for deferred functions calls and notifications.";
        values.push_back(entry);

        entry.name = "Object count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of objects currently instantiated (including nodes).";
        values.push_back(entry);

        entry.name = "Resources count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of resources currently used.";
        values.push_back(entry);

        entry.name = "Node count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Number of nodes currently instantiated in the scene tree.\n") +
                    "This also includes the root node.";
        values.push_back(entry);

        entry.name = "Orphan node count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of orphan nodes, i.e. nodes which are not parented to a node of the scene tree.";
        values.push_back(entry);

        entry.name = "Total objects";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of objects in the last rendered frame.\n") +
                    "This metric doesn't include culled objects (either via hiding nodes, frustum culling or occlusion culling).";
        values.push_back(entry);

        entry.name = "Total primitives";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of vertices or indices rendered in the last rendered frame.\n") +
                    "This metric doesn't include primitives from culled objects (either via hiding nodes, frustum culling or occlusion culling).\n" +
                    "Due to the depth prepass and shadow passes, the number of primitives is always higher than the actual number of vertices in the scene (typically double or triple the original vertex count).";
        values.push_back(entry);

        entry.name = "Draw calls";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of draw calls performed in the last rendered frame.\n") +
                    "This metric doesn't include culled objects (either via hiding nodes, frustum culling or occlusion culling), since they do not result in draw calls.";
        values.push_back(entry);

        entry.name = "Video memory usage";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The amount of video memory used (texture and vertex memory combined, in bytes).\n") +
                    "Since this metric also includes miscellaneous allocations, this value is always greater than the sum of \"Texture memory\" and \"Buffer memory\".";
        values.push_back(entry);

        entry.name = "Texture memory usage";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "The amount of texture memory used (in bytes).";
        values.push_back(entry);

        entry.name = "Buffer memory usage";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "The amount of render buffer memory used (in bytes).";
        values.push_back(entry);

        entry.name = "Active objects 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active RigidBody2D nodes in the game.";
        values.push_back(entry);

        entry.name = "Collision pairs 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of collision pairs in the 2D physics engine.";
        values.push_back(entry);

        entry.name = "Island count 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of islands in the 2D physics engine.";
        values.push_back(entry);

        entry.name = "Active objects 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active RigidBody3D and VehicleBody3D nodes in the game.";
        values.push_back(entry);

        entry.name = "Collision pairs 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of collision pairs in the 3D physics engine.";
        values.push_back(entry);

        entry.name = "Island count 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of islands in the 3D physics engine.";
        values.push_back(entry);

        entry.name = "Audio latency";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Output latency of the AudioServer.";
        values.push_back(entry);

        entry.name = "Active maps count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Number of active navigation maps in the NavigationServer3D.\n") +
                    "This also includes the two empty default navigation maps created by World2D and World3D.";
        values.push_back(entry);

        entry.name = "Region count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active navigation regions in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Agent count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active navigation agents processing avoidance in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Link count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active navigation links in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Polygon count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of navigation mesh polygons in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of navigation mesh polygon edges in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge merge count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of navigation mesh polygon edges that were merged due to edge key overlap in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge connect count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of polygon edges that are considered connected by edge proximity NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge free count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Number of navigation mesh polygon edges that could not be merged in the NavigationServer3D.\n") +
                    "The edges still may be connected by edge proximity or with links.";
        values.push_back(entry);

        add_group("Engine metrics", values);
    }

    void DebugInfo::add_enet_metrics()
    {
    }

    void DebugInfo::add_steam_metrics()
    {
    }

    String format_ms_time(uint32_t p_time)
    {
        if (p_time <= 0)
        {
            return "< 1ms";
        }
        return String::num_uint64(p_time) + "ms";
    }

    void DebugInfo::poll_godot_metrics()
    {
        godot::HashMap<String, ValueEntry> values;
        ValueEntry entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::TIME_FPS));
        entry.text = String::num_uint64(entry.value);
        values["FPS"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::TIME_PROCESS) * 100.0f);
        entry.text = format_ms_time(entry.value);
        values["Process Time"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::TIME_PHYSICS_PROCESS) * 100.0f);
        entry.text = format_ms_time(entry.value);
        values["Physics Time"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::TIME_NAVIGATION_PROCESS) * 100.0f);
        entry.text = format_ms_time(entry.value);
        values["Navigation Time"] = entry;

#ifdef DEBUG_ENABLED
        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::MEMORY_STATIC));
        entry.text = String::humanize_size(entry.value);
        values["Memory static"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::MEMORY_STATIC_MAX));
        entry.text = String::humanize_size(entry.value);
        values["Memory static max"] = entry;
#endif

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::MEMORY_MESSAGE_BUFFER_MAX));
        entry.text = String::humanize_size(entry.value);
        values["Message buffer max"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::OBJECT_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Object count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::OBJECT_RESOURCE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Resources count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::OBJECT_NODE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Node count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::OBJECT_ORPHAN_NODE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Orphan node count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::RENDER_TOTAL_OBJECTS_IN_FRAME));
        entry.text = String::num_uint64(entry.value);
        values["Total objects"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::RENDER_TOTAL_PRIMITIVES_IN_FRAME));
        entry.text = String::num_uint64(entry.value);
        values["Total primitives"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::RENDER_TOTAL_DRAW_CALLS_IN_FRAME));
        entry.text = String::num_uint64(entry.value);
        values["Draw calls"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::RENDER_VIDEO_MEM_USED));
        entry.text = String::humanize_size(entry.value);
        values["Video memory usage"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::RENDER_TEXTURE_MEM_USED));
        entry.text = String::humanize_size(entry.value);
        values["Texture memory usage"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::RENDER_BUFFER_MEM_USED));
        entry.text = String::humanize_size(entry.value);
        values["Buffer memory usage"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::PHYSICS_2D_ACTIVE_OBJECTS));
        entry.text = String::num_uint64(entry.value);
        values["Active objects 2D"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::PHYSICS_2D_COLLISION_PAIRS));
        entry.text = String::num_uint64(entry.value);
        values["Collision pairs 2D"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::PHYSICS_2D_ISLAND_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Island count 2D"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::PHYSICS_3D_ACTIVE_OBJECTS));
        entry.text = String::num_uint64(entry.value);
        values["Active objects 3D"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::PHYSICS_3D_COLLISION_PAIRS));
        entry.text = String::num_uint64(entry.value);
        values["Collision pairs 3D"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::PHYSICS_3D_ISLAND_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Island count 3D"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::AUDIO_OUTPUT_LATENCY) * 100.0f);
        entry.text = format_ms_time(entry.value);
        values["Audio latency"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_ACTIVE_MAPS));
        entry.text = String::num_uint64(entry.value);
        values["Active maps count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_REGION_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Region count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_AGENT_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Agent count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_LINK_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Link count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_POLYGON_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Polygon count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_MERGE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge merge count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_CONNECTION_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge connect count"] = entry;

        entry.value = static_cast<uint32_t>(performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_FREE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge free count"] = entry;

        VBoxContainer *godot_group = contents->get_node<VBoxContainer>("Engine metrics");
        for (const auto &target : values)
        {
            godot_group->get_node<Label>("variables/values/" + target.key)->set_text(target.value.text);
        }
    }

    void DebugInfo::poll()
    {
        poll_godot_metrics();
    }

    void DebugInfo::_ready()
    {
        RUNTIME_ONLY()

        args = GET_SYSTEM(LaunchArgs);
        if (args->is_steam())
        {
            steam_peer = GET_SYSTEM(Network)->get_steam_peer();
        }
        else
        {
            enet_peer = GET_SYSTEM(Network)->get_enet_peer();
        }

        multiplayer = get_tree()->get_multiplayer();
        performance = Performance::get_singleton();

        group->get_node<Node>("variables/names")->remove_child(group_entry_name);
        group->get_node<Node>("variables/values")->remove_child(group_entry_value);
        contents->remove_child(group);

        add_godot_metrics();
    }

    void DebugInfo::_exit_tree()
    {
        RUNTIME_ONLY()

        group->queue_free();
    }

    void DebugInfo::_process(double delta)
    {
        RUNTIME_ONLY()

        if (Input::get_singleton()->is_action_pressed("ui_debug_info"))
        {
            if (is_visible())
            {
                hide();
            }
            else
            {
                show();
            }
        }

        timer += delta;
        if (timer >= timeout)
        {
            poll();
            timer = 0.0f;
        }
    }
}
