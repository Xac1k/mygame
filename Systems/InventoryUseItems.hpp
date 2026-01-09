#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>
#include <Sounds/soundManager.hpp>
extern float drunkLevel;

class InventoryUseItems
{
private:
    bool prevStateButtonF = false;
public:
    void useItem(Items item, Vect2D cellID, std::shared_ptr<InventoryComponent> inventory, AudioSystem& audioManager) {
        switch (item)
        {
        case Items::beer:
            audioManager.playMusic("drink", false);
            inventory->countItems[cellID.y][cellID.x]-=1;
            if(inventory->countItems[cellID.y][cellID.x] == 0)
                inventory->inventory[cellID.y][cellID.x] = Items::none;
            drunkLevel += 0.1f;
            if(drunkLevel > 1) drunkLevel = 1;
            break;
        default:
            break;
        }
    }

    void update(EntitiesManager& manager, AudioSystem& audioManager) {
        auto inventory = manager.with<InventoryComponent>().getComponent<InventoryComponent>();
        if(!inventory) return;

        auto inventoryState = manager.with<InventoryComponent>().with<StateComponent>().getComponent<StateComponent>();
        if(!inventoryState) return;

        if(
            inventory->isSelected && 
            (
                inventoryState->state == (int)InventoryState::wrapped ||
                inventoryState->state == (int)InventoryState::selected
            )
            && !prevStateButtonF && sf::Keyboard::isKeyPressed(sf::Keyboard::F)
        ) {
            Vect2D selectedCellID = inventory->selection;
            auto item = inventory->inventory[selectedCellID.y][selectedCellID.x];
            useItem(item, selectedCellID, inventory, audioManager);
            prevStateButtonF = true;
        };

        if(prevStateButtonF && !sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            prevStateButtonF = false;
        }
    }
};


