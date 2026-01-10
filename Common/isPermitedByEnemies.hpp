#pragma once
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Common/const.hpp>
#include <math.h>
#include <unordered_map>
#include <Common/sortByDeath.hpp>

inline bool isPermittedByEnemies(EntitiesManager& manager, Vect2D spawnPos, CollisionComponent coll) {
    auto SpawnerCollRectLeftUp = spawnPos + coll.shiftFromLeftUp;
    //if(origin) moverCollRectLeftUp -= origin->shift;

    auto enemyIDs = manager.with<PositionOnMapComponent>().with<CollisionComponent>().get();
    enemyIDs = sortByDeath(manager, enemyIDs);
    
    for (int enemyID : enemyIDs) {
        auto enemyPos = manager.getComponent<PositionOnMapComponent>(enemyID);
        auto enemyCollRect = manager.getComponent<CollisionComponent>(enemyID);
        Vect2D origin({0, 0});
        if(manager.hasComponent<OriginComponent>(enemyID))
            origin = manager.getComponent<OriginComponent>(enemyID)->shift;
        auto enemyCollRectLeftUp = enemyPos->point - origin + enemyCollRect->shiftFromLeftUp;

        if(boxesOverlap(SpawnerCollRectLeftUp, coll.size, enemyCollRectLeftUp, enemyCollRect->size))
            return false;
    }
    return true;
}