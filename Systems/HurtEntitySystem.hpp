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
#include <Entities/utils/animationLoader.hpp>
#include <Systems/Game/EntityControll/EntityConrtoll.hpp>

extern EntityConrtoll entityControll;

std::vector<int> sortByAttackArea(EntitiesManager& manager, std::shared_ptr<AttackComponent> attackComp, std::vector<int> entitiesIDs) {
    std::vector<int> relevant;
    for(int id : entitiesIDs) {
        auto pos = manager.getComponent<PositionOnMapComponent>(id)->point;
        auto deathComp = manager.getComponent<DeathComponent>(id);
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

void addHurtComponent(EntitiesManager& manager, int enemyID, Vect2D agressorPos) {
    auto targetPos = manager.getComponent<PositionOnMapComponent>(enemyID).get()->point;
    auto angle = getAngle(targetPos - agressorPos);
    HurtComponent hurt(angle);

    manager.addComponent<HurtComponent>(hurt, enemyID);
}

void addStuneComponent(EntitiesManager& manager, int enemyID) {
    // if enemy not stuneable
    if(!manager.hasComponent<CooldownInfo>(enemyID)) return;
    auto stuneInfo = manager.getComponent<CooldownInfo>(enemyID);

    // create stune
    if(!manager.hasComponent<StuneCompanent>(enemyID)){
        StuneCompanent stune(stuneInfo->cooldownMoving);
        manager.addComponent<StuneCompanent>(stune, enemyID);
    }
    // edit stune
    else {
        auto stuneComp = manager.getComponent<StuneCompanent>(enemyID);
        stuneComp->currentTime = 0;
        stuneComp->duration = stuneInfo->cooldownMoving;
    }
    
    // clear cooldown attack  to stop preparating process
    manager.removeComponent<AttackCooldownCompanent>(enemyID);
    manager.removeComponent<ColldownBeforeAttackComp>(enemyID);
    manager.removeComponent<ReadyToAttack>(enemyID);
}

void addEffectComponent(EntitiesManager& manager, int enemyID,  std::shared_ptr<AttackComponent> attackComp) {
    if(!manager.hasComponent<EffectsComponent>(enemyID)) return;
    auto effects = manager.getComponent<EffectsComponent>(enemyID);
    effects->addEffect(attackComp);
    std::cout << "Ставим эффект на моба: " << enemyID << std::endl;
};

void HurtEntitySystem(EntitiesManager& manager) {
    auto existedEntity = entityControll.getEntityFromProducingRoom();
    auto attackerIDs = manager.load<PositionOnMapComponent>(existedEntity).with<AttackComponent>().get();
    for(auto attackerID : attackerIDs) {
        auto attackComp = manager.getComponent<AttackComponent>(attackerID);
        auto entitiesIDs = manager.load<PositionOnMapComponent>(existedEntity).with<HealthComponent>().except(attackerID).get();
        auto relevantEntityIDs = sortByAttackArea(manager, attackComp, entitiesIDs);

        for(auto entityID: relevantEntityIDs) {
            std::cout << "Атака на: " << entityID << std::endl;
            auto healthComp = manager.getComponent<HealthComponent>(entityID);
            auto deathComp = manager.getComponent<DeathComponent>(entityID);
            
            healthComp->health -= attackComp->damage;
            deathComp->angleOfDeath = attackComp->attackArea;

            addStuneComponent(manager, entityID);
            addHurtComponent(manager, entityID, attackComp->attackerPos);  
            if(attackComp->haveEffect())
                addEffectComponent(manager, entityID, attackComp);
        }
        
        manager.removeComponent<AttackComponent>(attackerID);
    }
}

