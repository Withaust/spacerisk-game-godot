#include <shared/units/motion_generator.hpp>

using namespace godot;

namespace spacerisk
{
    void MotionGeneratorFrame::_bind_methods()
    {
        BIND_PROPERTY(MotionGeneratorFrame, Variant::FLOAT, interpolation_time);
        BIND_PROPERTY(MotionGeneratorFrame, Variant::VECTOR3, position);
        BIND_PROPERTY(MotionGeneratorFrame, Variant::VECTOR3, velocity);
        BIND_PROPERTY(MotionGeneratorFrame, Variant::QUATERNION, rotation);
        BIND_PROPERTY(MotionGeneratorFrame, Variant::VECTOR3, angular_velocity);
    }

    void MotionGenerator::_bind_methods()
    {
    }

    bool MotionGenerator::is_extrapolating()
    {
        return frames.size() == 1;
    }

    bool MotionGenerator::has_frames()
    {
        return frames.size() != 0;
    }

    Vector3 MotionGenerator::get_position_no_error_correction()
    {
        if (!has_frames())
        {
            WARN_PRINT_ONCE("Trying to access position in an empty buffer. Zero vector returned.");
            return Vector3();
        }

        if (disable_extrapolation && is_extrapolating())
        {
            return frames[0]->get_position();
        }

        return frames[0]->get_position() + frames[0]->get_velocity() * playback_time;
    }

    Vector3 MotionGenerator::get_position()
    {
        return get_position_no_error_correction() + extrapolation_position_drift;
    }

    Vector3 MotionGenerator::get_velocity()
    {
        if (!has_frames())
        {
            WARN_PRINT_ONCE("Trying to access velocity in an empty buffer. Zero vector returned.");
            return Vector3();
        }

        return frames[0]->get_velocity();
    }

    Quaternion MotionGenerator::get_rotation_no_error_correction()
    {
        if (!has_frames())
        {
            WARN_PRINT_ONCE("Trying to access rotation in an empty buffer. Zero rotation returned.");
            return Quaternion();
        }

        if (disable_extrapolation && is_extrapolating())
        {
            return frames[0]->get_rotation();
        }

        return frames[0]->get_rotation() * Quaternion(frames[0]->get_angular_velocity() * playback_time);
    }

    Quaternion MotionGenerator::get_rotation()
    {
        return get_rotation_no_error_correction() * extrapolation_rotation_drift;
    }

    Vector3 MotionGenerator::get_angular_velocity()
    {
        if (!has_frames())
        {
            WARN_PRINT_ONCE("Trying to access angular velocity in an empty buffer. Zero vector returned.");
            return Vector3();
        }

        return frames[0]->get_angular_velocity();
    }

    Ref<MotionGeneratorFrame> MotionGenerator::get_last_received_frame()
    {
        if (!has_frames())
        {
            WARN_PRINT_ONCE("Trying to access last received frame in an empty buffer. Zero vector returned.");
            return nullptr;
        }
        return frames[frames.size() - 1];
    }

    Ref<MotionGeneratorFrame> MotionGenerator::get_current_frame()
    {
        if (!has_frames())
        {
            WARN_PRINT_ONCE("Trying to access last received frame in an empty buffer. Zero vector returned.");
            return nullptr;
        }
        return frames[0];
    }

    Quaternion MotionGenerator::get_rotation_difference(Quaternion p_from_rotation, Quaternion p_to_rotation)
    {
        Quaternion result(p_from_rotation);
        result = result.inverse();
        return result * p_to_rotation;
    }

    float MotionGenerator::format_euler_angle(float p_angle)
    {
        return p_angle > 180.0f ? p_angle - 360.0f : p_angle;
    }

    Vector3 MotionGenerator::format_euler_rotation(Vector3 p_euler_rotation)
    {
        return Vector3(format_euler_angle(p_euler_rotation.x), format_euler_angle(p_euler_rotation.y), format_euler_angle(p_euler_rotation.z));
    }

    void MotionGenerator::update_playback(double delta)
    {
        if (frames.size() < 1)
        {
            WARN_PRINT_ONCE("Trying to update playback in an empty buffer.");
            return;
        }

        if (delta > 0.0f)
        {
            // Smoothly correct time drift.
            float time_drift_correction = -Math::lerp(0.0f, time_drift, time_correction_speed * delta);
            // Add the time drift correction to delta time so it properly affects everything else.
            delta += time_drift_correction;
            time_drift += time_drift_correction;
            playback_time += delta;

            // Smoothly correct extrapolation errors.
            extrapolation_position_drift = extrapolation_position_drift.lerp(Vector3(), error_correction_speed * delta);
            extrapolation_rotation_drift = extrapolation_rotation_drift.slerp(Quaternion(), error_correction_speed * delta);
        }

        // Remove old keyframes.
        while (frames.size() > 1 && playback_time >= frames[1]->get_interpolation_time())
        {
            // If you're going through an instant keyframe, nullify drifting because it's a teleport keyframe.
            if (frames[1]->get_interpolation_time() == 0.0f)
            {
                extrapolation_position_drift = Vector3();
                extrapolation_rotation_drift = Quaternion();
            }

            playback_time -= frames[1]->get_interpolation_time();
            frames.erase(frames.begin());
        }
    }

    void MotionGenerator::add_frame_custom(float p_interpolation_time, Vector3 p_position, Vector3 p_velocity, Quaternion p_rotation, Vector3 p_angular_velocity)
    {
        // Prevent long first frame if some keyframes were skipped before the first frame.
        if (frames.size() < 1)
        {
            p_interpolation_time = Math::max(interpolation_latency, 0.01f);
        }

        // Calculate time drift.
        float time_till_frame_buffer_end = p_interpolation_time - playback_time;
        for (int i = 1; i < frames.size(); i++)
        {
            time_till_frame_buffer_end += frames[i]->get_interpolation_time();
        }

        time_drift = interpolation_latency - time_till_frame_buffer_end;

        // Inserting empty keyframe the buffer is empty (LastReceivedKeyframe doesn't exist).
        if (frames.size() < 1)
        {
            Ref<MotionGeneratorFrame> fake_frame;
            fake_frame.instantiate();
            fake_frame->set_interpolation_time(0.0f);
            fake_frame->set_position(p_position);
            fake_frame->set_velocity(Vector3());
            fake_frame->set_rotation(p_rotation);
            fake_frame->set_angular_velocity(Vector3());
            frames.push_back(fake_frame);
        }

        Vector3 position_before_new_frame = get_position();
        Quaternion rotation_before_new_frame = get_rotation();

        // Add the keyframe.
        Ref<MotionGeneratorFrame> last_received_frame = get_last_received_frame();
        Vector3 calculated_velocity = p_interpolation_time > 0.0f ? (p_position - last_received_frame->get_position()) / p_interpolation_time : Vector3();
        Quaternion calculated_rotation_difference = get_rotation_difference(last_received_frame->get_rotation(), p_rotation);
        Vector3 calculated_angular_velocity = p_interpolation_time > 0.0f ? format_euler_rotation(calculated_rotation_difference.get_euler()) / p_interpolation_time : Vector3();

        Ref<MotionGeneratorFrame> frame;
        frame.instantiate();
        frame->set_interpolation_time(p_interpolation_time);
        frame->set_position(p_position);
        if (p_velocity != Vector3())
        {
            frame->set_velocity(p_velocity);
        }
        else
        {
            frame->set_velocity(calculated_velocity);
        }
        frame->set_rotation(p_rotation);
        if (p_angular_velocity != Vector3())
        {
            frame->set_angular_velocity(p_angular_velocity);
        }
        else
        {
            frame->set_angular_velocity(calculated_angular_velocity);
        }
        frames.push_back(frame);

        // Set previous keyframe velocity to match the new position.
        last_received_frame->set_velocity(calculated_velocity);
        last_received_frame->set_angular_velocity(calculated_angular_velocity);
        frames[frames.size() - 2] = last_received_frame;

        // Get onto a new frame if needed.
        update_playback(0.0f);

        // Calculate drift.
        Vector3 position_after_new_frame = get_position_no_error_correction();
        Quaternion rotation_after_new_frame = get_rotation_no_error_correction();

        extrapolation_position_drift = position_before_new_frame - position_after_new_frame;
        extrapolation_rotation_drift = get_rotation_difference(rotation_after_new_frame, rotation_before_new_frame);
    }

    void MotionGenerator::add_frame(float p_interpolation_time, Vector3 p_position, Quaternion p_rotation)
    {
        add_frame_custom(p_interpolation_time, p_position, {}, p_rotation);
    }

    void MotionGenerator::add_frame(float p_interpolation_time, Vector3 p_position, Vector3 p_velocity, Quaternion p_rotation, Vector3 p_angular_velocity)
    {
        add_frame_custom(p_interpolation_time, p_position, p_velocity, p_rotation, p_angular_velocity);
    }
}
