#include <shared/entity.hpp>

using namespace godot;

namespace spacerisk
{
    void Entity::_ready()
    {
        RUNTIME_ONLY()

        if (got_components)
        {
            return;
        }

        const auto classes = ClassDB::get_custom_classes();
        int child_count = get_child_count();
        for (int i = 0; i < child_count; ++i)
        {
            Component *component = cast_to<Component>(get_child(i));
            if (component == nullptr)
            {
                continue;
            }

            ClassDB::ClassInfo class_info;
            auto itterator = classes.find(component->get_class());
            if (itterator == classes.end())
            {
                continue;
            }

            class_info = classes.at(itterator->first);

            if (components.has(class_info.name))
            {
                UtilityFunctions::push_error("Tried re-registering " + class_info.name + " class as a component in a node named " + get_name());
                continue;
            }
            components[class_info.name] = component;

            while (class_info.parent_ptr != nullptr)
            {
                class_info = *class_info.parent_ptr;
                if (class_info.name == StringName("Component"))
                {
                    break;
                }
                if (components.has(class_info.name))
                {
                    UtilityFunctions::push_error("Tried re-registering " + class_info.name + " class as a component in a node named " + get_name());
                    break;
                }
                components[class_info.name] = component;
            }
        }

        got_components = true;
    }
}
