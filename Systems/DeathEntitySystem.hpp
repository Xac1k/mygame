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

void DeathEntitySystem(EntitiesManager& manager, AudioSystem& audioManager, float df) {
    auto entityIDs = manager.with<HealthComponent>().get();
    for (int entityID : entityIDs) {
        auto healthComp = manager.getComponent<HealthComponent>(entityID).get();
        auto deathComp = manager.getComponent<DeathComponent>(entityID).get();
        if(healthComp->health <= 0 && !deathComp->isDead) {
            std::cout << "Умер:" << entityID << std::endl;
            deathComp->isDead = true;
            audioManager.playMusic(deathComp->soudnFXname, false);
        }
        if(deathComp->isDead) {
            deathComp->deathTime += df;
        }
    }
}