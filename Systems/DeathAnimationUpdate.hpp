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
#include <Entities/components/entityState.hpp>

void DeathAnimationUpdateSystem(EntitiesManager& manager) {
    auto entityIDs = manager.with<DeathComponent>().get();
    for (int entityID : entityIDs) {
        auto deathComp = manager.getComponent<DeathComponent>(entityID);
        auto stateComp = manager.getComponent<StateComponent>(entityID);
        if(deathComp->isDead) {
            if(manager.hasComponent<DirectionalDeathComponent>(entityID)) {
                auto directionalDeath = manager.getComponent<DirectionalDeathComponent>(entityID)->facing;
                switch (directionalDeath)
                {
                case Facing::Backward:
                    stateComp->state = (int)EntityState::deathBackward;
                    break;
                case Facing::Rigth:
                    stateComp->state = (int)EntityState::deathRight;
                    break;
                case Facing::Left:
                    stateComp->state = (int)EntityState::deathLeft;
                    break;
                case Facing::Direct:
                    stateComp->state = (int)EntityState::deathDirect;
                    break;
                }
            }
            else {
                stateComp->state = (int)EntityState::death;
            }
        }
    }
}