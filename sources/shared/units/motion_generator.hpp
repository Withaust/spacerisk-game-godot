#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <shared/util.hpp>

namespace spacerisk
{
    class MotionGeneratorFrame : public godot::RefCounted
    {
        UGDCLASS(MotionGeneratorFrame, RefCounted)
    private:
        float interpolation_time = 0.0f;
        godot::Vector3 position;
        godot::Vector3 velocity;
        godot::Quaternion rotation;
        godot::Vector3 angular_velocity;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY(float, interpolation_time)
        IMPL_PROPERTY(godot::Vector3, position)
        IMPL_PROPERTY(godot::Vector3, velocity)
        IMPL_PROPERTY(godot::Quaternion, rotation)
        IMPL_PROPERTY(godot::Vector3, angular_velocity)
    };

    class MotionGenerator : public godot::RefCounted
    {
        UGDCLASS(MotionGenerator, RefCounted)

    private:
        // It's basically a frame buffer length defined in seconds.
        // Values above sync message send interval increase resistance to bad network conditions.
        // Values below sync message send interval compensate the latency by using extrapolation.
        float interpolation_latency = 0.04f;
        // Smoothly corrects position and rotation extrapolation errors.
        float error_correction_speed = 10.0f;
        // Smoothly corrects time drift back into correct time frame.
        float time_correction_speed = 0.75f;
        // Sometimes extrapolation may not be desired at all because of overshooting.
        // Make sure to use interpolation_latency way higher than send interval if you enable this setting.
        bool disable_extrapolation = true;
        // Gets playback time between current and the next frame.
        float playback_time = 0.0f;
        // Gets current time drift.
        float time_drift = 0.0f;
        std::vector<godot::Ref<MotionGeneratorFrame>> frames;
        // Gets current position drift.
        godot::Vector3 extrapolation_position_drift;
        // Gets current rotation drift.
        godot::Quaternion extrapolation_rotation_drift;

    protected:
        static void _bind_methods();

    public:
        // Used to identify if it's currently playing back through frames or predicting the future.
        bool is_extrapolating();
        // Indicates if the frame buffer is not empty (received any frames).
        bool has_frames();
        // Same as get_position(), but doesn't smoothly correct extrapolation drift when new frame arrives.
        godot::Vector3 get_position_no_error_correction();
        // Gets position at current playback time.
        godot::Vector3 get_position();
        // Gets velocity at current playback time.
        godot::Vector3 get_velocity();
        // Same as get_rotation(), but doesn't smoothly correct extrapolation drift when new frame arrives.
        godot::Quaternion get_rotation_no_error_correction();
        // Gets rotation at current playback time.
        godot::Quaternion get_rotation();
        // Gets angular velocity at current playback time.
        godot::Vector3 get_angular_velocity();
        // Gets last received frame.
        godot::Ref<MotionGeneratorFrame> get_last_received_frame();
        // Gets frame you're currently interpolating from.
        godot::Ref<MotionGeneratorFrame> get_current_frame();
        // Gets the rotation difference between two quaternions.
        // Basically output is "p_to_rotation - p_from_rotation", but in quaternion math.
        godot::Quaternion get_rotation_difference(godot::Quaternion p_from_rotation, godot::Quaternion p_to_rotation);
        // Converts angle values from 0 to 360 degree range into -180 to 180 degree range.
        float format_euler_angle(float p_angle);
        // Converts vector rotation values from 0 to 360 degree range into -180 to 180 degree range.
        godot::Vector3 format_euler_rotation(godot::Vector3 p_euler_rotation);
        // Progress the playback further by deltaTime.
        // delta - Time in seconds.
        void update_playback(double delta);
        // Adds a new keyframe to the buffer. Expert version where you can choose what to specify.
        void add_frame_custom(float p_interpolation_time, godot::Vector3 p_position = {}, godot::Vector3 p_velocity = {}, godot::Quaternion p_rotation = {}, godot::Vector3 p_angular_velocity = {});
        // Adds a new keyframe to the buffer. Basic version that automatically calculates the velocity.
        // Everything should be specified in global coordinates.
        // p_interpolation_time - Time (in seconds) to interpolate to this keyframe from the previous one.
        // p_position - Target position.
        // p_rotation - Target rotation. Optional parameter. Omit if you don't need to sync rotations.</param>
        void add_frame(float p_interpolation_time, godot::Vector3 p_position, godot::Quaternion p_rotation = {});
        // Adds a new keyframe to the buffer. Advanced version with explicitly specified velocity.
        // Greatly improves extrapolation results and helps to simplify code that saves traffic on idling objects.
        // Everything should be specified in global coordinates.
        // p_interpolation_time - Time (in seconds) to interpolate to this keyframe from the previous one.
        // p_position - Target position.
        // p_velocity - Target velocity. Improves extrapolation results and helps optimizing bandwidth for idling objects.
        // p_rotation - Target rotation. Optional parameter. Omit if you don't need to sync rotations.
        // p_angular_velocity - Target angular velocity. Optional parameter, specified in degrees. Calculated using previous keyframe when omitted.
        void add_frame(float p_interpolation_time, godot::Vector3 p_position, godot::Vector3 p_velocity, godot::Quaternion p_rotation = {}, godot::Vector3 p_angular_velocity = {});
    };
}
