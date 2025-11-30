#pragma once
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/components/barrel.hpp>
#include <Infrastructure/sfml.h>

void SpawnBarrelSystemUpdate(EntitiesManager& manager, TextureLoader& textureLoader) {
    auto mapIds = manager.with<MapComponent>().get();
    if(mapIds.size() == 0) return;

    auto map = manager.getComponent<MapComponent>(mapIds[0]).get();
    if(!map) return;

    Vect2D roomPos(map->map.roomsMap[0].x * TILE_SIZE + 100, map->map.roomsMap[0].y * TILE_SIZE + 100);
    std::cout << "Спавн бочки" << roomPos.x << " " << roomPos.y << std::endl;
    barrel(manager, textureLoader, roomPos);
}


