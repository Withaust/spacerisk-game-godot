#pragma once

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/engine_profiler.hpp>

#include <system/launch_args.hpp>

#define CONCAT2(x, y) #x "" #y
#define CONCAT3(x, y, z) #x "" #y "" #z
#define CONCAT4(x, y, z, w) #x "" #y "" #z "" #w

// Generic properties

#define IMPL_PROPERTY(Type, Name)  \
    void set_##Name(Type p_##Name) \
    {                              \
        Name = p_##Name;           \
    }                              \
    Type get_##Name()              \
    {                              \
        return Name;               \
    }

#define BIND_PROPERTY(Class, Type, Name)                                                   \
    godot::ClassDB::bind_method(godot::D_METHOD(CONCAT2(get_, Name)), &Class::get_##Name); \
    godot::ClassDB::bind_method(godot::D_METHOD(CONCAT2(set_, Name)), &Class::set_##Name); \
    ADD_PROPERTY(godot::PropertyInfo(Type, #Name), CONCAT2(set_, Name), CONCAT2(get_, Name))

#define BIND_PROPERTY_VIRTUAL(Class, Name)  \
    BIND_VIRTUAL_METHOD(Class, get_##Name); \
    BIND_VIRTUAL_METHOD(Class, set_##Name)

// Property path

#define BIND_PROPERTY_PATH(Class, Type, Name)                                              \
    godot::ClassDB::bind_method(godot::D_METHOD(CONCAT2(get_, Name)), &Class::get_##Name); \
    godot::ClassDB::bind_method(godot::D_METHOD(CONCAT2(set_, Name)), &Class::set_##Name); \
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, #Name, PROPERTY_HINT_NODE_TYPE, #Type), CONCAT2(set_, Name), CONCAT2(get_, Name))

#define BIND_PROPERTY_PATH_RESOURCE(Class, Type, Name)                                     \
    godot::ClassDB::bind_method(godot::D_METHOD(CONCAT2(get_, Name)), &Class::get_##Name); \
    godot::ClassDB::bind_method(godot::D_METHOD(CONCAT2(set_, Name)), &Class::set_##Name); \
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, #Name, PROPERTY_HINT_RESOURCE_TYPE, #Type), CONCAT2(set_, Name), CONCAT2(get_, Name))

// Debug only methods

#ifdef DEBUG_METHODS_ENABLED
#define RUNTIME_ONLY(v)                                                                                        \
    if (godot::Engine::get_singleton()->is_editor_hint() || spacerisk::LaunchArgs::get_singleton() == nullptr) \
    {                                                                                                          \
        return v;                                                                                              \
    }
// TODO: Add GODOT_PROFILING_FUNCTION support
#else
#define RUNTIME_ONLY(v)
#endif

// System access

#define GET_SYSTEM(Target) \
    get_node<Target>(godot::String("/root/") + #Target + "System")

// Method binding

#define BIND_METHOD(Class, Name) \
    godot::ClassDB::bind_method(godot::D_METHOD(#Name), &Class::Name)

#define UGDCLASS(m_class, m_inherits) \
    typedef m_inherits Super;         \
    GDCLASS(m_class, m_inherits)

// Shortcuts

#define AUTHORITY_ONLY(v)            \
    if (!is_multiplayer_authority()) \
    {                                \
        return v;                    \
    }

#define NOT_AUTHORITY_ONLY(v)       \
    if (is_multiplayer_authority()) \
    {                               \
        return v;                   \
    }

#define PROCESS_SLEEP()        \
    if (!process_sleep(delta)) \
    {                          \
        return;                \
    }

#define PHYSICS_SLEEP()        \
    if (!physics_sleep(delta)) \
    {                          \
        return;                \
    }
