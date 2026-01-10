#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Interfaces/busEvent.h>
#include <Entities/utils/component.hpp>
#include <Entities/components/inventory.hpp>
#include <Common/CollisionRect.hpp>
#include <Sounds/soundManager.hpp>
#include <Systems/UI/DrunkEffectSystem/DrunkEffectSystem.hpp>

extern DrunkEffectSystem drunkSystem;;

class InventoryUseItems
{
private:
    bool prevStateButtonF = false;

    void useBucketOfWater(EntitiesManager& manager) {
        auto effects = manager.with<EffectsComponent>().withClassName("*player*").getComponent<EffectsComponent>();
        if(!effects) return;
        EffectComponent effect;
        effect.effect = Effects::wet;
        effect.duration = 30.f;
        effect.period = 1.55f;
        effects->cloneEffect(&effect);
    }

    void useBerry(EntitiesManager& manager) {
        auto health = manager.with<PositionOnMapComponent>().withClassName("*player*").getComponent<HealthComponent>();
        if(!health) return;
        health->health += 10;
        if(health->health > health->maxHealth) health->health = health->maxHealth;
    }
public:
    void useItem(Items item, Vect2D cellID, std::shared_ptr<InventoryComponent> inventory, AudioSystem& audioManager, EntitiesManager& manager) {
        switch (item)
        {
        case Items::beer:
            audioManager.playMusic("drink", false);
            inventory->countItems[cellID.y][cellID.x]-=1;
            if(inventory->countItems[cellID.y][cellID.x] == 0)
                inventory->inventory[cellID.y][cellID.x] = Items::none;
            drunkSystem.addDrunkLevel(0.1f);
            break;
        case Items::bucketOfWater:
            audioManager.playMusic("dropWater", false);
            useBucketOfWater(manager);
            inventory->countItems[cellID.y][cellID.x]-=1;
            if(inventory->countItems[cellID.y][cellID.x] == 0)
                inventory->inventory[cellID.y][cellID.x] = Items::none;
            break;
        case Items::berry:
            audioManager.playMusic("eat", false);
            useBerry(manager);
            inventory->countItems[cellID.y][cellID.x]-=1;
            if(inventory->countItems[cellID.y][cellID.x] == 0)
                inventory->inventory[cellID.y][cellID.x] = Items::none;
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
            useItem(item, selectedCellID, inventory, audioManager, manager);
            prevStateButtonF = true;
        };

        if(prevStateButtonF && !sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            prevStateButtonF = false;
        }
    }
};


