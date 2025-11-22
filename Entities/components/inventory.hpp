#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"

constexpr Vect2D unwrapedPos(100, 50);
constexpr Vect2D wrapedPos(225, 350);

constexpr Vect2D unwrapedSize(500, 300);
constexpr Vect2D wrapedSize(250, 50);


enum class InventoryState { idle, selected, wrapped };
void inventory(EntitiesManager& manager, TextureLoader& textureLoader) {
    manager.addEntity();

    PositionComponent pos(wrapedPos);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(wrapedSize);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) InventoryState::wrapped);
    manager.addComponent<StateComponent>(state);

    InventoryComponent inventory(Vect2D(5, 3), Vect2D(0, 0));
    inventory.inventory[0][0] = Items::startSword;
    inventory.inventory[0][1] = Items::startPickaxe;
    inventory.inventory[0][2] = Items::firePickaxe;
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

            {(int)InventoryState::wrapped, {
                {"Store/view/Inventory/HotBar.png", 1.f, false},
            }}
        }
    );

    manager.addComponent<AnimationComponent>(animationComponent);


}