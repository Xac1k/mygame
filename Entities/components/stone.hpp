#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"
#include <Entities/utils/toolTableLoader.hpp>

Vect2D sizeCollRectStone(32, 10);

enum class StoneType {coal, iron, gold, rubin};
enum class StoneState {hurt0, hurt1, hurt2, hurt3, death = 450};
int ore(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D posOnMap, StoneType type) {
    manager.addEntity("playMenu:Enemy:stone:" + std::to_string((int)type) + " Static Deletable", true);

    PositionOnMapComponent posMap(posOnMap);
    manager.addComponent<PositionOnMapComponent>(posMap);

    SizeComponent size(32, 32);
    manager.addComponent<SizeComponent>(size);

    HealthComponent health(100, 100);
    manager.addComponent<HealthComponent>(health);

    StateComponent state((int) StoneState::hurt0);
    manager.addComponent<StateComponent>(state);

    CollisionComponent rect(sizeCollRectStone, Vect2D(32, 32) - sizeCollRectStone);
    manager.addComponent<CollisionComponent>(rect);

//============================= LOOT =============================
    LootTableComponent lootTable;
    switch (type)
    {
    case StoneType::coal:
        loadLootTable(lootTable.drops, {{Items::coal, 1, 3, 10, 30},});
        break;

    case StoneType::iron:
        loadLootTable(lootTable.drops, {{Items::iron, 1, 3, 10, 30},});
        break;
    case StoneType::gold:
        loadLootTable(lootTable.drops, {{Items::gold, 1, 3, 10, 30},});
        break;
    case StoneType::rubin:
        loadLootTable(lootTable.drops, {{Items::rubin, 1, 3, 10, 30},});
        break;
    default:
        break;
    };
    manager.addComponent<LootTableComponent>(lootTable);

//============================= DEATH =============================
    DeathComponent deathComp(0.125f* 7, "StoneCrush" + std::to_string((int)type) , 0);
    manager.addComponent<DeathComponent>(deathComp);

//============================= ANIMATION =============================
    AnimationGridComponent animationComponent;
    animationComponent.TileSizeInGrid = {64, 64};

    float column = (float)type;
    loadAnimations(animationComponent.animation, 
        {
            {(int)StoneState::hurt0, {
                {"Store/view/Stone/камни_все.png", {0, column}, 1.f, false},
            }},
            {(int)StoneState::hurt1, {
                {"Store/view/Stone/камни_все.png", {1, column}, 1.f, false},
            }},
            {(int)StoneState::hurt2, {
                {"Store/view/Stone/камни_все.png", {2, column}, 1.f, false},
            }},
            {(int)StoneState::hurt3, {
                {"Store/view/Stone/камни_все.png", {3, column}, 1.f, false},
            }},
            {(int)StoneState::death, {
                {"Store/view/Stone/камни_все.png", {3, column},  0.125f, false},
                {"Store/view/Stone/камни_все.png", {4, column},  0.125f, false},
                {"Store/view/Stone/камни_все.png", {3, column},  0.125f, false},
                {"Store/view/Stone/камни_все.png", {4, column},  0.125f, false},
                {"Store/view/Stone/камни_все.png", {3, column},  0.125f, false},
                {"Store/view/Stone/камни_все.png", {4, column},  0.125f, false},
                {"Store/view/Stone/камни_все.png", {3, column},  0.125f, false},
            }},
        }
    );

    manager.addComponent<AnimationGridComponent>(animationComponent);
    return manager.getID();
}