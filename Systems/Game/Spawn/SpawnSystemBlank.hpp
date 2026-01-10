#pragma once
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/components/barrel.hpp>
#include <Common/const.hpp>
#include <Map/Creating/Common/dist.hpp>
#include <Map/Creating/Common/randomRange.hpp>
#include <Map/Creating/Config/configDirect.hpp>
#include <Common/CollisionRect.hpp>
#include <Common/getCollisionRectOnMap.hpp>
#include <math.h>
#include <Common/randFloat.hpp>
#include <Common/isPermitedByMap.hpp>
#include <Entities/components/stone.hpp>
#include <unordered_map>
#include <Common/isPermitedByEnemies.hpp>

class SpawnSystemBlankIntoRoom {
public:
    Vect2D entityInRoom = {3, 8};
    std::vector<int> spawnedEntity;
    CollisionComponent coll;

    virtual int spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) = 0;
private:
    void spawnItemsInRoom(EntitiesManager& manager, TextureLoader& textureLoader, const sf::Vector3f& room) {
        int count = randRange(entityInRoom.x, entityInRoom.y);

        for (int i = 0; i < count; i++) {
            float angle = randFloat() * 2 * M_PI;
            float radius = randFloat() * room.z * TILE_SIZE;

            Vect2D offset(
                cos(angle) * radius,
                sin(angle) * radius
            );

            Vect2D spawnPos(
                room.x * TILE_SIZE,
                room.y * TILE_SIZE
            );

            spawnPos += offset;

            if (!isPermittedByMapStaticObj(manager, spawnPos, coll))
                continue;

            if(!isPermittedByEnemies(manager, spawnPos, coll)){
                int i;
                i++;
                continue;
            }

            spawnedEntity.push_back(spawnEntityFunc(manager, textureLoader, spawnPos));
        }
    }
    
    std::unordered_map<int, bool> roomSpawned;
public:
    void update(EntitiesManager& manager, TextureLoader& textureLoader, int ProducingRoomID) {
        auto map = manager.with<MapComponent>().getComponent<MapComponent>();
        if(!map) return;
        sf::Vector3f room = map->map.roomsMap[ProducingRoomID];
        if (roomSpawned[ProducingRoomID]) return;

        spawnedEntity.clear();
        spawnItemsInRoom(manager, textureLoader, room);
        roomSpawned[ProducingRoomID] = true;
    }

    bool IsRoomNotSpawned(int ProducingRoomID) {
        return !roomSpawned[ProducingRoomID];
    }

    void reset() {
        roomSpawned.clear();
    }
};
