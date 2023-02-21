#include <register_types.h>

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// Resources
#include <resources/item.hpp>
// Shared units
#include <shared/units/motion_generator.hpp>
// Shared code
#include <shared/entity.hpp>
#include <shared/components/component.hpp>
#include <shared/components/transform.hpp>
#include <shared/components/interactable.hpp>
// Level scenes
#include <level/door/interactable.hpp>
#include <level/door/pathfinding.hpp>
#include <level/trigger_multiple.hpp>
#include <level/drill.hpp>
// Dynamic scenes
#include <dynamic/player/autority.hpp>
#include <dynamic/player/movement.hpp>
#include <dynamic/player/transform.hpp>
#include <dynamic/player/label.hpp>
#include <dynamic/player/callout.hpp>
#include <dynamic/player/interactor.hpp>
#include <dynamic/player/shooter.hpp>
#include <dynamic/soldier/transform.hpp>
#include <dynamic/soldier/pathfinding.hpp>
// System units
#include <system/units/steam_multiplayer_peer.hpp>
#include <system/units/steam_callback.hpp>
// System scenes
#include <system/launch_args.hpp>
#include <system/steam.hpp>
#include <system/network.hpp>
#include <system/player/spawner.hpp>
#include <system/commander.hpp>
#include <system/ui.hpp>
// UI scenes
#include <ui/chat.hpp>
#include <ui/interact_menu.hpp>
#include <ui/debug_info.hpp>

using namespace godot;
using namespace spacerisk;

#define RegClass(Type) ClassDB::register_class<Type>()
#define RegVirtual(Type) ClassDB::register_class<Type>(true)
#define RegAbstract(Type) ClassDB::register_abstract_class<Type>()

void initialize_spacerisk(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
    // Resources
    RegClass(Item);
    // Shared units
    RegClass(MotionGeneratorFrame);
    RegClass(MotionGenerator);
    // Shared code
    RegClass(Entity);
    RegVirtual(Component);
    RegVirtual(Transform);
    RegClass(Interactable);
    // Level scenes
    RegClass(DoorInteractable);
    RegClass(DoorPathfinding);
    RegClass(TriggerMultiple);
    RegClass(Drill);
    // Dynamic scenes
    RegClass(PlayerAuthority);
    RegClass(PlayerMovement);
    RegClass(PlayerTransform);
    RegClass(PlayerLabel);
    RegClass(PlayerCallout);
    RegClass(PlayerInteractor);
    RegClass(PlayerShooter);
    RegClass(SoldierTransform);
    RegClass(SoldierPathfinding);
    // System units
    RegClass(SteamMultiplayerPeer);
    RegClass(SteamCallback);
    // System scenes
    RegClass(LaunchArgs);
    RegClass(Steam);
    RegClass(Network);
    RegClass(PlayerSpawner);
    RegClass(Commander);
    RegClass(UI);
    // UI scenes
    RegClass(Chat);
    RegClass(InteractMenu);
    RegClass(DebugInfo);
}

void uninitialize_spacerisk(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
}

extern "C"
{
    GDExtensionBool GDE_EXPORT spacerisk_init(const GDExtensionInterface *p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

        init_obj.register_initializer(initialize_spacerisk);
        init_obj.register_terminator(uninitialize_spacerisk);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
