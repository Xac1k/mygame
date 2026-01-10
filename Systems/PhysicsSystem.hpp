#pragma once
#include <cmath>
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

int sign(int x) {
    if(x > 0) return 1;
    if(x < 0) return -1;
    return 0;
}

int nsign(int x) {
    if(x > 0) return -1;
    if(x < 0) return 1;
    return 0;
}

void UpdatePhysicsSystem(EntitiesManager& manager, TextureLoader textureLoader, float dt) {
    auto entityIDs = manager.with<PhysicsComponent>().with<PositionOnMapComponent>().get();

    for (auto entityID : entityIDs) {
        auto pos = manager.getComponent<PositionOnMapComponent>(entityID);
        auto vel = manager.getComponent<PhysicsComponent>(entityID);

        if(!vel->velo.x && !vel->velo.y) continue;

        pos->point.x += vel->velo.x * dt;
        pos->point.y += vel->velo.y * dt;

        float decay_factor = std::exp(-2.f * dt);
        vel->velo.x = vel->velo.x * decay_factor;
        vel->velo.y = vel->velo.y * decay_factor;

        if(abs(vel->velo.x) < 0.5 ) vel->velo.x = 0; 
        if(abs(vel->velo.y) < 0.5 ) vel->velo.y = 0;
    }
}