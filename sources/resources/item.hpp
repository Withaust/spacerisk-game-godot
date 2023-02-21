#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class Item : public godot::Resource
    {
        UGDCLASS(Item, Resource)
    private:
        float weight = 10.0f;
        float price = 10.0f;
        int size_x = 1;
        int size_y = 1;
        godot::Ref<godot::Texture> texture;

    protected:
        static void _bind_methods()
        {
            BIND_PROPERTY(Item, godot::Variant::FLOAT, weight);
            BIND_PROPERTY(Item, godot::Variant::FLOAT, price);
            BIND_PROPERTY(Item, godot::Variant::INT, size_x);
            BIND_PROPERTY(Item, godot::Variant::INT, size_y);
            BIND_PROPERTY_PATH_RESOURCE(Item, Texture, texture);
        };

    public:
        IMPL_PROPERTY(float, weight)
        IMPL_PROPERTY(float, price)
        IMPL_PROPERTY(int, size_x)
        IMPL_PROPERTY(int, size_y)
        IMPL_PROPERTY(godot::Ref<godot::Texture>, texture)
    };
}
