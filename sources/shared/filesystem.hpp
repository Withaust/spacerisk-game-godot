#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/dir_access.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Filesystem
    {
    private:
        static void _get_files_recursively(godot::String path, godot::String file_extension, godot::PackedStringArray &files);

    public:
        static godot::PackedStringArray get_files_recursively(godot::String path, godot::String file_extension = "");
    };
}
