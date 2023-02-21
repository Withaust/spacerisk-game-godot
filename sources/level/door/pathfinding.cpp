#include <level/door/pathfinding.hpp>

using namespace godot;

namespace spacerisk
{
    void DoorPathfinding::_bind_methods()
    {
        BIND_PROPERTY_PATH(DoorPathfinding, CollisionShape3D, collider);
    }

    void DoorPathfinding::_ready()
    {
        RUNTIME_ONLY()

        collider->set_disabled(false);
    }
}
