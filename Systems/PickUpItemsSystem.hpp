#pragma once
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Interfaces/busEvent.h>
#include <Map/Creating/Common/dist.hpp>
#include <Entities/components/items.hpp>
#include <Sounds/soundManager.hpp>
#include <Entities/utils/itemsConfig/ItemsStack.hpp>

void insertIntoInventory(EntitiesManager& manager, int lootID, int count, AudioSystem& audioManager) {
    auto inventoryIDs = manager.with<InventoryComponent>().get();
    if(inventoryIDs.size() == 0) return;
    auto stateComp = manager.getComponent<StateComponent>(lootID).get()->state;
    auto invComp = manager.getComponent<InventoryComponent>(inventoryIDs[0]).get();

    int y = 0; int x = 0;
    for (auto& line : invComp->inventory) {
        for(auto& cell : line) {
            if(cell == Items(stateComp) || cell == Items::none) {
                if(invComp->countItems[y][x] == limitationStack[Items(stateComp)]) continue;
                invComp->countItems[y][x]++;
                if(cell != Items(stateComp))
                    cell = Items(stateComp);
                manager.removeEntity(lootID);
                audioManager.playMusic("item"+std::to_string(stateComp), false);
                return;
            }
            x++;
        }
        y++;
        x = 0;
    }
}

void LootPickUpSystem(EntitiesManager& manager, AudioSystem& audioManager) {
    auto lootIDs = manager.with<PickUpItemComponent>().with<PositionOnMapComponent>().get();
    if(lootIDs.size() == 0) return;
    auto playerPosPtr = manager.with<PositionOnMapComponent>().withClassName("*player*").getComponent<PositionOnMapComponent>();
    if(!playerPosPtr) return;

    for (int lootId : lootIDs) {
        auto pos = manager.getComponent<PositionOnMapComponent>(lootId)->point;
        auto pickUpRadius = manager.getComponent<PickUpItemComponent>(lootId);
        if(dist(pos, playerPosPtr->point) < pickUpRadius->pickUpRadius) {
            insertIntoInventory(manager, lootId, pickUpRadius->count, audioManager);
        }
    }
}