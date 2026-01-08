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
#include <Common/randFloat.hpp>

constexpr int pickUpRadius = 32;

std::vector<int> sortByDeath(EntitiesManager& manager, std::vector<int> entityIDs) {
    std::vector<int> result;
    for(int id : entityIDs) {
        auto deathComp = manager.getComponent<DeathComponent>(id);
        if(
            deathComp->isDead && 
            deathComp->deathTime > deathComp->fadeOutTime
        ) result.push_back(id);
    }
    return result;
}

Vect2D getVelocity(Vect2D angleArea) {
    float angle;

    angle = randRange(angleArea.x, angleArea.y);
    if(angleArea.x > angleArea.y) {
        int side = randRange(0, 1);
        if(side == 0) {
            angle = randRange(angleArea.x, 360);
        }
        if(side == 1) {
            angle = randRange(0, angleArea.y);
        }
    }

    float lenVelo = randRange(-100, 100);

    return Vect2D(lenVelo * sin(angle), lenVelo * cos(angle));
}

void LootDropSystem(EntitiesManager& manager, TextureLoader textureLoader) {
    auto entityIDs = manager.with<LootTableComponent>().with<DeathComponent>().get();
    auto entityToLoot = sortByDeath(manager, entityIDs);

    for (int entityId : entityToLoot) {
        auto pos = manager.getComponent<PositionOnMapComponent>(entityId)->point;
        auto drops = manager.getComponent<LootTableComponent>(entityId)->drops;
        auto angleOfDeath = manager.getComponent<DeathComponent>(entityId)->angleOfDeath;

        for (const auto& drop : drops) {
            if (randFloat() > drop.chance) continue;
            int count = randRange(drop.minCount, drop.maxCount);
            for(int i = 0; i < count; i++) {
                Vect2D offset = fromAngle(randFloat() * 360.0f) * randFloat() * drop.offsetRadius;
                Vect2D spawnPos = pos + offset;

                int newEntityID = item(manager, textureLoader, spawnPos);
                auto stateComp = manager.getComponent<StateComponent>(newEntityID);
                stateComp->state = (int)drop.itemID;

                auto velo = getVelocity(angleOfDeath);
                PhysicsComponent physic(velo.x, velo.y);
                manager.addComponent<PhysicsComponent>(newEntityID, physic);

                PickUpItemComponent pickUp(count, pickUpRadius);
                manager.addComponent<PickUpItemComponent>(newEntityID, pickUp);
            }
        }

        std::cout << "Удалили:" << entityId << " | Появился лут\n";
        manager.removeEntity(entityId);
    }
}