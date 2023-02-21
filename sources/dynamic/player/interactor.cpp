#include <dynamic/player/interactor.hpp>

using namespace godot;

namespace spacerisk
{
    void PlayerInteractor::_bind_methods()
    {
        BIND_PROPERTY_PATH(PlayerInteractor, RayCast3D, raycast);

        BIND_METHOD(PlayerInteractor, on_interact);
    }

    void PlayerInteractor::on_interact(godot::String option)
    {
        RUNTIME_ONLY()

        if (collision_target == nullptr)
        {
            return;
        }

        auto options = collision_target->get_options();

        if (!options.has(option))
        {
            return;
        }

        Entity *entity = get_entity();
        // TODO Convert to a simple "call", since it should still work
        options[option].callv(Array::make(entity));
    }

    void PlayerInteractor::_ready()
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()

        interact_menu = GET_SYSTEM(UI)->get_node<InteractMenu>("root/interact_menu");
        interact_menu->connect("on_interact", Callable(this, "on_interact"));

        transform = get_entity()->get_component<Transform>();

        set_process_sleep(20.0f);
    }

    void PlayerInteractor::_process(double delta)
    {
        RUNTIME_ONLY()
        AUTHORITY_ONLY()
        PROCESS_SLEEP()

        if (raycast->is_colliding() &&
            raycast->get_collision_point().distance_to(transform->get_position()) < 5.0f)
        {

            Entity *target = get_entity(cast_to<Node>(raycast->get_collider()));
            Interactable *interactable = nullptr;
            if (target != nullptr)
            {
                interactable = target->get_component<Interactable>();
            }

            if (interactable == nullptr && collision_target != nullptr)
            {
                collision_target = nullptr;
                interact_menu->hide();
            }
            else if (interactable != collision_target)
            {
                collision_target = interactable;
                interact_menu->show(interactable->get_options());
            }
        }
        else
        {
            if (collision_target != nullptr)
            {
                interact_menu->hide();
                collision_target = nullptr;
            }
        }
    }
}
