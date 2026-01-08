#pragma once
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/components/goblin.hpp>
#include <Entities/components/items.hpp>
#include <Entities/components/caveVillager.hpp>
#include <Entities/components/stone.hpp>

void SpawnSystemUpdate(EntitiesManager& manager, TextureLoader textureLoader) {
    auto playerIds = manager.withClassName("*player*");
    if(playerIds.size() == 0) return;

    auto mapIds = manager.with<MapComponent>().get();
    if(mapIds.size() == 0) return;

    auto map = manager.getComponent<MapComponent>(mapIds[0]).get();
    auto player = manager.getComponent<PositionOnMapComponent>(playerIds[0]).get();
    if(!map || !player) return;

    // skeleton(manager, textureLoader, player->point + Vect2D(100, 100));
    // skeleton(manager, textureLoader, player->point + Vect2D(200, 100));
    // skeleton(manager, textureLoader, player->point + Vect2D(300, 100));
    // skeleton(manager, textureLoader, player->point + Vect2D(100, 300));
    
    villager(manager, textureLoader, player->point - Vect2D(100, 100));
    addDialogToVillager(manager, "ExampleOfDialog.json");

    ore(manager, textureLoader, player->point - Vect2D(200, 200), StoneType::rubin);
    villager(manager, textureLoader, player->point - Vect2D(200, 100));
    villager(manager, textureLoader, player->point - Vect2D(300, 100));
    villager(manager, textureLoader, player->point - Vect2D(100, 300));
}