#pragma once
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Entities/utils/entitiesManager.hpp>

void SpawnSystemUpdate(EntitiesManager& manager) {
    auto playerIds = manager.with<PlayerPosComponent>().get();
    if(playerIds.size() == 0) return;

    auto mapIds = manager.with<MapComponent>().get();
    if(mapIds.size() == 0) return;

    auto map = manager.getComponent<MapComponent>(mapIds[0]).get();
    auto player = manager.getComponent<PlayerPosComponent>(playerIds[0]).get();
    if(!map || !player) return;

    Vect2D roomPos(map->map.roomsMap[0].x * TILE_SIZE, map->map.roomsMap[0].y * TILE_SIZE);
    //TODO: Нужно определять комнату в которой только один выход и вход.
    // Сейчас комната выбирается просто потому что
    player->point = roomPos;
    std::cout << "Спавн" << roomPos.x << " " << roomPos.y << std::endl;
}