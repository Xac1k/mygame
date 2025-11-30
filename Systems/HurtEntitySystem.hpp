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

std::vector<int> sortByAttackArea(EntitiesManager& manager, AttackComponent *attackComp, std::vector<int> entitiesIDs) {
    std::vector<int> relevant;
    for(int id : entitiesIDs) {
        auto pos = manager.getComponent<PositionOnMapComponent>(id).get()->point;
        auto deathComp = manager.getComponent<DeathComponent>(id).get();
        auto angle = getAngle(pos - attackComp->attackerPos); 

        if(
            attackComp->attackArea.y - attackComp->attackArea.x < 0 &&
            !deathComp->isDead
        ) {
            if(
                angle > attackComp->attackArea.x && angle < 360 || 
                angle > 0 && angle < attackComp->attackArea.y
            ) {
                auto len = dist(pos, attackComp->attackerPos);
                if(len < attackComp->attackLen) {
                    relevant.push_back(id);
                }
            }
        }
        else if(
            angle > attackComp->attackArea.x && 
            angle < attackComp->attackArea.y &&
            !deathComp->isDead
        ) {
            auto len = dist(pos, attackComp->attackerPos);
            if(len < attackComp->attackLen) {
                relevant.push_back(id);
            }
        }
    }

    return relevant;
}

void HurtEntitySystem(EntitiesManager& manager) {
    auto attackerIDs = manager.with<AttackComponent>().get();
    for(auto attackerID : attackerIDs) {
        auto attackComp = manager.getComponent<AttackComponent>(attackerID).get();
        auto entitiesIDs = manager.with<HealthComponent>().except(attackerID).get();
        auto relevantEntityIDs = sortByAttackArea(manager, attackComp, entitiesIDs);

        for(auto entityID: relevantEntityIDs) {
            std::cout << "Атака на:" << entityID << std::endl;
            auto healthComp = manager.getComponent<HealthComponent>(entityID).get();
            healthComp->health -= attackComp->damage;
        }
        
        manager.removeComponent<AttackComponent>(attackerID);
    }
}