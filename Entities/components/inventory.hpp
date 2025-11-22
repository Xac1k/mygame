#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"

enum class InventoryState { idle, selected, wrapped };
void inventory(EntitiesManager& manager, TextureLoader& textureLoader) {
    manager.addEntity();

    PositionComponent pos(100, 50);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(500, 300);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) InventoryState::idle);
    manager.addComponent<StateComponent>(state);

    InventoryComponent inventory(Vect2D(5, 3), Vect2D(0, 0));
    inventory.inventory[0][0] = Items::startSword;
    inventory.inventory[0][1] = Items::startPickaxe;
    inventory.inventory[1][1] = Items::firePickaxe;
    manager.addComponent<InventoryComponent>(inventory);

    DragAndDropComponent dragAndDrop;
    manager.addComponent<DragAndDropComponent>(dragAndDrop);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)InventoryState::idle, {
                {"Store/view/Inventory/Inventory.png", 1.f, false},
            }},

            {(int)InventoryState::selected, {
                {"Store/view/Inventory/Inventory.png", 1.f, false},
            }},
        }
    );

    manager.addComponent<AnimationComponent>(animationComponent);


}