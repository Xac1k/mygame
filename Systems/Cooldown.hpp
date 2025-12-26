#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/components/goblin.hpp>
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
#include <Common/getMiddleAngle.hpp>

class Cooldown
{
private:
    void updateStune(EntitiesManager& manager, float df) {
        auto enemyIDs = manager.with<StuneCompanent>().get();

        for (int enemyID : enemyIDs) {
            auto stuneComp = manager.getComponent<StuneCompanent>(enemyID).get();
            stuneComp->currentTime += df;

            if(stuneComp->currentTime > stuneComp->duration)
                manager.removeComponent<StuneCompanent>(enemyID);
        }
    }

    void updateCooldownAttack(EntitiesManager& manager, float df) {
        auto enemyIDs = manager.with<AttackCooldownCompanent>().get();

        for (int enemyID : enemyIDs) {
            auto cooldownComp = manager.getComponent<AttackCooldownCompanent>(enemyID).get();
            cooldownComp->currentTime += df;

            if(cooldownComp->currentTime > cooldownComp->duration) {
                manager.removeComponent<AttackCooldownCompanent>(enemyID);
                ReadyToAttack ready; 
                manager.addComponent(ready, enemyID);
            }
        }
    }

    void updateCooldownBeforeAttack(EntitiesManager& manager, float df) {
        auto enemyIDs = manager.with<ColldownBeforeAttackComp>().get();

        for (int enemyID : enemyIDs) {
            auto cooldownComp = manager.getComponent<ColldownBeforeAttackComp>(enemyID).get();
            cooldownComp->currentTime += df;

            if(cooldownComp->currentTime > cooldownComp->duration){
                manager.removeComponent<ColldownBeforeAttackComp>(enemyID);
                ReadyToAttack ready; 
                manager.addComponent(ready, enemyID);
            }
        }
    }

    void updateCooldownBeforeEffectsExpand(EntitiesManager& manager, float df) {
        auto enemyIDs = manager.with<ColldownBeforeEffectExpand>().get();

        for (int enemyID : enemyIDs) {
            auto cooldownComp = manager.getComponent<ColldownBeforeEffectExpand>(enemyID).get();
            cooldownComp->currentTime += df;

            if(cooldownComp->currentTime > cooldownComp->duration){
                manager.removeComponent<ColldownBeforeEffectExpand>(enemyID);
                ReadyToExpandEffect ready; 
                manager.addComponent(ready, enemyID);
            }
        }
    }
public:
    void update(EntitiesManager& manager, float df) {
        updateStune(manager, df);
        updateCooldownAttack(manager, df);
        updateCooldownBeforeAttack(manager, df);
        updateCooldownBeforeEffectsExpand(manager, df);
    }
};

