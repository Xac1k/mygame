#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>
#include <Sounds/soundManager.hpp>
#include <Interfaces/busEvent.h>
#include <Common/getAngle.hpp>
#include <Map/Creating/Common/dist.hpp>
#include <Map/Creating/Common/randomRange.hpp>
#include <Entities/components/items.hpp>

constexpr int pickUpRadius = 32;

std::vector<int> sortByDeath(EntitiesManager& manager, std::vector<int> entityIDs) {
    std::vector<int> result;
    for(int id : entityIDs) {
        auto deathComp = manager.getComponent<DeathComponent>(id).get();
        if(
            deathComp->isDead && 
            deathComp->deathTime > deathComp->fadeOutTime
        ) result.push_back(id);
    }
    return result;
}

float randFloat() {
    return (float)rand() / RAND_MAX;
}

void LootDropSystem(EntitiesManager& manager, TextureLoader textureLoader) {
    auto entityIDs = manager.with<LootTableComponent>().with<DeathComponent>().get();
    auto entityToLoot = sortByDeath(manager, entityIDs);

    for (int entityId : entityToLoot) {
        auto pos = manager.getComponent<PositionOnMapComponent>(entityId).get();
        auto lootTable = manager.getComponent<LootTableComponent>(entityId).get();

        for (const auto& drop : lootTable->drops) {
            if (randFloat() > drop.chance) continue;
            int count = randRange(drop.minCount, drop.maxCount);
            Vect2D offset = fromAngle(randFloat() * 360.0f) * randFloat() * drop.offsetRadius;
            Vect2D spawnPos = pos->point + offset;

            int newEntityID = item(manager, textureLoader, spawnPos);

            auto stateComp = manager.getComponent<StateComponent>(newEntityID).get();
            stateComp->state = (int)drop.itemID;
               
            PhysicsComponent physic(randRange(-50, 50), randRange(-50, 50));
            manager.addComponent<PhysicsComponent>(newEntityID, physic);

            PickUpItemComponent pickUp(count, pickUpRadius);
            manager.addComponent<PickUpItemComponent>(newEntityID, pickUp);
        }

        std::cout << "Удалили:" << entityId << " | Появился лут\n";
        manager.removeEntity(entityId);
    }
}