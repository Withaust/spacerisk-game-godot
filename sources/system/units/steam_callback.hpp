#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

#include <shared/util.hpp>

#define GET_CALLBACK_PTR(p_callback, p_callback_enum)                                                           \
    auto callback = p_callback->get_pointer<p_callback_enum##_t>(SteamCallback::CallbackType::p_callback_enum); \
    if (callback == nullptr)                                                                                    \
    {                                                                                                           \
        return;                                                                                                 \
    }

namespace spacerisk
{
    class SteamCallback : public godot::RefCounted
    {
        UGDCLASS(SteamCallback, RefCounted)
    public:
        enum CallbackType
        {
            None,
            SteamNetConnectionStatusChangedCallback,
        };

    private:
        CallbackType type = CallbackType::None;
        uint64_t *pointer = nullptr;

    protected:
        static void _bind_methods();

    public:
        void set(CallbackType p_type, uint64_t *p_pointer);
        CallbackType get_type();
        template <class T>
        T *get_pointer(CallbackType p_type)
        {
            if (type == p_type)
            {
                return reinterpret_cast<T *>(pointer);
            }
            return nullptr;
        }
    };
}
