#pragma once
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/components/goblin.hpp>
#include <Entities/components/items.hpp>

Vect2D getSpawnPosition(MapComponent *map) {
    std::map<int, int> countRooms;
    switch (map->map.type)
    {
    case mapType::Rect:
        for(auto pair : map->map.corridorsMap) {
            if(countRooms.find(pair.first) == countRooms.end()) 
                countRooms[pair.first] = 0;

            if(countRooms.find(pair.second) == countRooms.end()) 
                countRooms[pair.second] = 0;

            countRooms[pair.first]++;
            countRooms[pair.second]++;  
        }
        for(auto candidate : countRooms) {
            if(candidate.second == 1) {
                auto room = map->map.roomsMap.at(candidate.first);
                return Vect2D(room.x, room.y);
            }
        }
        break;
    case mapType::Circle:
        auto Vect = map->map.roomsMap.at(map->map.roomsMap.size() - 1);
        return Vect2D(Vect.x, Vect.y);  
    }

    auto room = map->map.roomsMap[0];
    return Vect2D(room.x, room.y);
}

void SpawnSystemUpdate(EntitiesManager& manager, TextureLoader textureLoader) {
    auto playerIds = manager.withClassName("*player*");
    if(playerIds.size() == 0) return;

    auto mapIds = manager.with<MapComponent>().get();
    if(mapIds.size() == 0) return;

    auto map = manager.getComponent<MapComponent>(mapIds[0]).get();
    auto player = manager.getComponent<PositionOnMapComponent>(playerIds[0]).get();
    if(!map || !player) return;

    Vect2D roomPos(map->map.roomsMap[0].x * TILE_SIZE, map->map.roomsMap[0].y * TILE_SIZE);
    player->point = getSpawnPosition(map) * TILE_SIZE;
    std::cout << "Спавн" << roomPos.x << " " << roomPos.y << std::endl;
    //skeleton(manager, textureLoader, player->point + Vect2D(100, 100));
}