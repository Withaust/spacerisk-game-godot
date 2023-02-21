#include <shared/filesystem.hpp>

using namespace godot;

namespace spacerisk
{
    void Filesystem::_get_files_recursively(godot::String path, godot::String file_extension, godot::PackedStringArray &files)
    {
        // Remove trailing slash if present
        if (path.ends_with("/"))
        {
            path = path.left(path.length() - 1);
        }

        Ref<DirAccess> dir = DirAccess::open(path);

        if (dir == nullptr)
        {
            return;
        }

        dir->list_dir_begin();

        String file_name = dir->get_next();

        while (file_name != "")
        {
            if (dir->current_is_dir())
            {
                _get_files_recursively(dir->get_current_dir() + "/" + file_name, file_extension, files);
            }
            else
            {
                if (!file_extension.is_empty() && file_name.get_extension() != file_extension)
                {
                    file_name = dir->get_next();
                    continue;
                }
                files.append(dir->get_current_dir() + "/" + file_name);
            }

            file_name = dir->get_next();
        }
    }

    PackedStringArray Filesystem::get_files_recursively(String path, String file_extension)
    {
        PackedStringArray result;
        _get_files_recursively(path, file_extension, result);
        return result;
    }
}
