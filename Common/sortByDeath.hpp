#pragma once
#include <vector>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Common/const.hpp>
#include <math.h>
#include <unordered_map>

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
