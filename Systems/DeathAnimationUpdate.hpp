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
        auto deathComp = manager.getComponent<DeathComponent>(entityID).get();
        auto stateComp = manager.getComponent<StateComponent>(entityID).get();
        if(deathComp->isDead) {
            if(manager.hasComponent<DirectionalDeathComponent>(entityID)) {
                auto directionalDeath = manager.getComponent<DirectionalDeathComponent>(entityID).get()->facing;
                switch (directionalDeath)
                {
                case Facing::Down:
                    break;
                case Facing::Rigth:
                    break;
                case Facing::Left:
                    break;
                case Facing::Up:
                    break;
                }
            }
            else {
                stateComp->state = (int)EntityState::death;
            }
        }
    }
}