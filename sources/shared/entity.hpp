#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include <shared/components/component.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Component;

    class Entity : public godot::Node3D
    {
        UGDCLASS(Entity, Node3D)

    private:
        bool got_components = false;
        godot::HashMap<godot::StringName, Component *> components;

    protected:
        static void _bind_methods() {}

    public:
        virtual void _ready() override;

        template <class T>
        T *get_component()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit Component to be used with get_component()");
            if (!got_components)
            {
                _ready();
            }
            if (!components.has(T::get_class_static()))
            {
                return nullptr;
            }
            return cast_to<T>(components.get(T::get_class_static()));
        }
    };
}
