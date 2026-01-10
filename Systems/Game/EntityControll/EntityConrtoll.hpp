#pragma once
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/SpawnSystem.hpp>
#include <Systems/Game/Spawn/SpawnSystem.hpp>
#include <Entities/components/dialogPoint.hpp>
#include <Entities/components/FinalPoint.hpp>
#include <Sounds/soundManager.hpp>
#include <Systems/Game/Spawn/SpawnSystemBlank.hpp>
#include <Entities/components/goblin.hpp>
#include <unordered_set>

class SpawnSkeletonSystem : public SpawnSystemBlankIntoRoom {
    int spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) override {
        SkeletonType type;
        float chance = randFloat();
        if(chance < 0.25) 
            type = SkeletonType::Fire;
        else if(chance < 0.5)
            type = SkeletonType::Wet;
        else if(chance < 0.75)
            type = SkeletonType::Poisoned;
        else 
            type = SkeletonType::none;

        skeleton(manager, textureLoader, spawnPos, type);
        return manager.getID();
    }
};

class SpawnOreSystem : public SpawnSystemBlankIntoRoom {
    int spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) override {
        StoneType type;
        float chance = randFloat();
        if(chance < 0.4) 
            type = StoneType::coal;
        else if(chance < 0.7)
            type = StoneType::iron;
        else if(chance < 0.85)
            type = StoneType::gold;
        else 
            type = StoneType::rubin;
        ore(manager, textureLoader, spawnPos, type);
        return manager.getID();
    }
};

class SpawnBarrelSystem : public SpawnSystemBlankIntoRoom {
    int spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) override {
        barrel(manager, textureLoader, spawnPos);
        return manager.getID();
    }
};

class EntityConrtoll
{
public:
    float ENTER_TOLERANCE = TILE_SIZE * 6.f;
private:
    std::unordered_map<int, std::vector<int>> EntityIntoRoom;
    bool init = false;
    sf::Vector3f init_room;
    int producingRoomID = 0;

    bool isPreloadedMap = false;
    std::vector<int> EntityOnMap;

    std::unordered_set<int> extraID;

    SpawnBarrelSystem SpawnBarrel;
    SpawnSkeletonSystem SpawnSkeleton;
    SpawnOreSystem SpawnOre; 

    bool isPlayerInsideRoom(const Vect2D& playerPos, const sf::Vector3f& room) {
        Vect2D roomCenter(room.x * TILE_SIZE, room.y * TILE_SIZE);
        float distToCenter = dist(playerPos, roomCenter);
        return distToCenter <= (room.z * TILE_SIZE + ENTER_TOLERANCE);
    }

public:
    EntityConrtoll() {
        SpawnBarrel.entityInRoom = {3, 10};
        SpawnSkeleton.entityInRoom = {3, 10};
        SpawnOre.entityInRoom = {7, 14};

        SpawnBarrel.coll = {sizeCollRectBarrel, Vect2D(32, 32) - sizeCollRectBarrel};
        SpawnSkeleton.coll = {Vect2D(26, 20), Vect2D(84, 60)};
        SpawnOre.coll = {sizeCollRectBarrel, Vect2D(32, 32) - sizeCollRectBarrel};
    };

    void resetSpawnSystems() {
        SpawnBarrel.reset();
        SpawnSkeleton.reset();
        SpawnOre.reset();
    }

    void updateSpawnSystems(EntitiesManager& manager, TextureLoader& textureLoader) {
        if(!init) return;
        if(SpawnBarrel.IsRoomNotSpawned(producingRoomID)) {
            SpawnBarrel.update(manager, textureLoader, producingRoomID);
            EntityIntoRoom[producingRoomID].insert(EntityIntoRoom[producingRoomID].end(), SpawnBarrel.spawnedEntity.begin(), SpawnBarrel.spawnedEntity.end());
        }
        if(SpawnSkeleton.IsRoomNotSpawned(producingRoomID)){
            SpawnSkeleton.update(manager, textureLoader, producingRoomID);
            EntityIntoRoom[producingRoomID].insert(EntityIntoRoom[producingRoomID].end(), SpawnSkeleton.spawnedEntity.begin(), SpawnSkeleton.spawnedEntity.end());
        }
        if(SpawnOre.IsRoomNotSpawned(producingRoomID)) {
            SpawnOre.update(manager, textureLoader, producingRoomID);
            EntityIntoRoom[producingRoomID].insert(EntityIntoRoom[producingRoomID].end(), SpawnOre.spawnedEntity.begin(), SpawnOre.spawnedEntity.end());
        }
    }

    void update(EntitiesManager& manager) {
        auto map = manager.with<MapComponent>().getComponent<MapComponent>();
        if(!map) return;

        auto player = manager.with<PositionOnMapComponent>().withClassName("*player*").getComponent<PositionOnMapComponent>();
        if(!player) return;

        Vect2D playerPos = player->point;
        isPreloadedMap = map->map.type == mapType::Preloaded;
        if(map->map.type == mapType::Preloaded) {
            EntityOnMap = manager.with<PositionOnMapComponent>().get();
            init = false;
        }
        else {
            if(init) {
                if (!isPlayerInsideRoom(playerPos, init_room)) {
                    init = false;
                }
            }
            else {
                for (size_t i = 0; i < map->map.roomsMap.size(); i++) {
                    auto& room = map->map.roomsMap[i];
                    if (isPlayerInsideRoom(playerPos, room)) {
                        init_room = room;
                        producingRoomID = i;
                        init = true;
                        return;
                    }
                }
            }
        }
    }

    void addEntityIntoExtraProducing(EntitiesManager& manager, int entityID) {
        if(!manager.isEntityExist(entityID)) return;
        extraID.insert(entityID);
    }

    std::vector<int> getEntityFromProducingRoom() {
        if(isPreloadedMap) {
            return EntityOnMap;
        }
        else if(init){
            std::vector<int> res;
            res.insert(res.end(), EntityIntoRoom[producingRoomID].begin(), EntityIntoRoom[producingRoomID].end());
            res.insert(res.end(), extraID.begin(), extraID.end());
            return res;
        }
        else {
            std::vector<int> res;
            for(auto it: extraID) res.push_back(it);
            return res;
        }
    }
};