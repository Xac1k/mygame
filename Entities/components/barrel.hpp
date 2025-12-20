#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"
#include <Entities/utils/toolTableLoader.hpp>

Vect2D sizeCollRectBarrel(32, 10);

enum class BarrelState {idle, death = 450};
enum class ControlFlowBarrel {
    HurtSystem, 
    All
};
int barrel(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D posOnMap) {
    manager.addEntity("playMenu:Enemy:barrel", true);

    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    SizeComponent size(32, 32);
    manager.addComponent<SizeComponent>(size);

    VelocityComponent velo;
    manager.addComponent<VelocityComponent>(velo);

    HealthComponent health(20, 20);
    manager.addComponent<HealthComponent>(health);

    StateComponent state((int) BarrelState::idle);
    manager.addComponent<StateComponent>(state);

    CollisionComponent rect(sizeCollRectBarrel, Vect2D(32, 32) - sizeCollRectBarrel);
    manager.addComponent<CollisionComponent>(rect);

    MutexComponent<ControlFlowBarrel> mutex;
    manager.addComponent<MutexComponent<ControlFlowBarrel>>(mutex);

    LootTableComponent lootTable;
    loadLootTable(lootTable.drops, {{Items::coin, 1, 3, 10, 30},});
    manager.addComponent<LootTableComponent>(lootTable);

    DeathComponent deathComp(0.125f* 7, "BarrelDeath", 0);
    manager.addComponent<DeathComponent>(deathComp);

    AnimationGridComponent animationComponent;
    animationComponent.TileSizeInGrid = {64, 64};
    loadAnimations(animationComponent.animation, 
        {
            {(int)BarrelState::idle, {
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 0}, 1.f, true},
            }},
            {(int)BarrelState::death, {
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 1},  0.125f, false},
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 0},  0.125f, false},
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 1},  0.125f, false},
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 0},  0.125f, false},
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 1},  0.125f, false},
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 0},  0.125f, false},
                {"Store/view/Enemy/Barrel/Barrel Sprite.png", {0, 1},  0.125f, false},
            }},
        }
    );
    manager.addComponent<AnimationGridComponent>(animationComponent);

    return manager.getID();
}