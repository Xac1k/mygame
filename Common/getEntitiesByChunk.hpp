#pragma once
#include <vector>
#include <Common/Vect.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Common/const.hpp>

class ArchiveOfEntitiesInChunk {
    private:
        bool init = false;
        Vect2D init_pos;
        std::vector<int> cache;

        bool returnFromCache(Vect2D PlayerTilePos) {
            // std::cout << "Сдвиг игрока в tile: " << (PlayerTilePos).x << ' ' << (PlayerTilePos).y << "\n"
            // << (init_pos).x << ' ' << (init_pos).y << "\n"
            // << abs((init_pos - PlayerTilePos).x) << ' ' << abs((init_pos - PlayerTilePos).y) << "\n";
            return init && ((abs((init_pos - PlayerTilePos).x) < CHUNK_SIZE.x) || (abs((init_pos - PlayerTilePos).y) < CHUNK_SIZE.y)); 
        };
    public:
        std::vector<int> getEntitiesByChunk(EntitiesManager& manager) {
            auto playerIDs = manager.withClassName("*player*");
            if(playerIDs.size() == 0) return {};
            auto playerPosComp = manager.getComponent<PositionOnMapComponent>(playerIDs[0]).get();
            auto playerTilePos = playerPosComp->point / TILE_SIZE;
            if(returnFromCache(playerTilePos)) 
                return cache;

            auto entityIDs = manager.with<PositionOnMapComponent>().get();

            auto mapIDs = manager.with<MapComponent>().get();
            if(mapIDs.size() == 0) return {};

            auto map = manager.getComponent<MapComponent>(mapIDs[0]).get();
            if(!map) return {};

            auto settingsIDs = manager.with<ChunkLoaderComponent>().get();
            if(settingsIDs.size() == 0) return {};
            auto settingComp = manager.getComponent<ChunkLoaderComponent>(settingsIDs[0]).get();

            auto LeftUpTilePos = playerTilePos - Vect2D(settingComp->countChunk/2, settingComp->countChunk/2) * CHUNK_SIZE;
            if(LeftUpTilePos.x < 0) LeftUpTilePos.x = 0;
            if(LeftUpTilePos.y < 0) LeftUpTilePos.y = 0;
            auto RightDownTilePos = LeftUpTilePos + Vect2D(settingComp->countChunk * CHUNK_SIZE.x, settingComp->countChunk * CHUNK_SIZE.y);

            auto LeftUpPos = LeftUpTilePos * TILE_SIZE;
            auto RightDownPos = RightDownTilePos * TILE_SIZE;
            
            std::vector<int> res;
            for(auto entityID : entityIDs) {
                Vect2D entityPos = manager.getComponent<PositionOnMapComponent>(entityID)->point;
                if(entityPos > LeftUpPos && entityPos < RightDownPos) res.push_back(entityID);
            }
            cache = res;
            init = true;
            init_pos = playerTilePos;
            return res;
        }
};

std::vector<int> getEntitiesByChunk(EntitiesManager& manager) {
    auto entityIDs = manager.with<PositionOnMapComponent>().get();

    auto mapIDs = manager.with<MapComponent>().get();
    if(mapIDs.size() == 0) return {};

    auto map = manager.getComponent<MapComponent>(mapIDs[0]).get();
    if(!map) return {};

    auto settingsIDs = manager.with<ChunkLoaderComponent>().get();
    if(settingsIDs.size() == 0) return {};

    auto playerIDs = manager.withClassName("*player*");
    if(playerIDs.size() == 0) return {};

    auto settingComp = manager.getComponent<ChunkLoaderComponent>(settingsIDs[0]).get();
    auto playerPosComp = manager.getComponent<PositionOnMapComponent>(playerIDs[0]).get();
    auto playerTilePos = playerPosComp->point / map->TileSize;

    auto LeftUpTilePos = playerTilePos - Vect2D(settingComp->countChunk/2, settingComp->countChunk/2) * CHUNK_SIZE;
    if(LeftUpTilePos.x < 0) LeftUpTilePos.x = 0;
    if(LeftUpTilePos.y < 0) LeftUpTilePos.y = 0;
    auto RightDownTilePos = LeftUpTilePos + Vect2D(settingComp->countChunk * CHUNK_SIZE.x, settingComp->countChunk * CHUNK_SIZE.y);

    auto LeftUpPos = LeftUpTilePos * TILE_SIZE;
    auto RightDownPos = RightDownTilePos * TILE_SIZE;
    
    std::vector<int> res;
    for(auto entityID : entityIDs) {
        Vect2D entityPos = manager.getComponent<PositionOnMapComponent>(entityID)->point;
        if(entityPos > LeftUpPos && entityPos < RightDownPos) res.push_back(entityID);
    }
    return res;
}