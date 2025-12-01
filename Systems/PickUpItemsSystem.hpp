#pragma once
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Interfaces/busEvent.h>
#include <Map/Creating/Common/dist.hpp>
#include <Entities/components/items.hpp>
#include <Sounds/soundManager.hpp>

void insertIntoInventory(EntitiesManager& manager, int lootID, int count, AudioSystem& audioManager) {
    auto inventoryIDs = manager.with<InventoryComponent>().get();
    if(inventoryIDs.size() == 0) return;
    auto stateComp = manager.getComponent<StateComponent>(lootID).get()->state;
    auto invComp = manager.getComponent<InventoryComponent>(inventoryIDs[0]).get();

    for (auto& line : invComp->inventory) {
        for(auto& cell : line) {
            if(cell == Items::none) {
                cell = Items(stateComp);
                manager.removeEntity(lootID);
                audioManager.playMusic("item"+std::to_string(stateComp), false);
                return;
            }
        }
    }

}

void LootPickUpSystem(EntitiesManager& manager, AudioSystem& audioManager) {
    auto lootIDs = manager.with<PickUpItemComponent>().with<PositionOnMapComponent>().get();
    auto playerIDs = manager.withClassName("*player*");
    if(playerIDs.size() == 0 || lootIDs.size() == 0) return;
    auto playerPos = manager.getComponent<PositionOnMapComponent>(playerIDs[0]).get()->point;

    for (int lootId : lootIDs) {
        auto pos = manager.getComponent<PositionOnMapComponent>(lootId).get()->point;
        auto pickUpRadius = manager.getComponent<PickUpItemComponent>(lootId).get();
        if(dist(pos, playerPos) < pickUpRadius->pickUpRadius) {
            insertIntoInventory(manager, lootId, pickUpRadius->count, audioManager);
        }
    }
}