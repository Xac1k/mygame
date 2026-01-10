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

class SpawnSystemBlankIntoRoom {
public:
    Vect2D entityInRoom = {3, 8};
    float ENTER_TOLERANCE = TILE_SIZE * 1.5f;

    virtual void spawnEntityFunc(EntitiesManager& manager, TextureLoader& textureLoader, Vect2D spawnPos) = 0;
private:
    bool isPlayerInsideRoom(const Vect2D& playerPos, const sf::Vector3f& room) {
        Vect2D roomCenter(room.x * TILE_SIZE, room.y * TILE_SIZE);
        float distToCenter = dist(playerPos, roomCenter);
        return distToCenter <= (room.z * TILE_SIZE + ENTER_TOLERANCE);
    }

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

            CollisionComponent coll(Vect2D(16, 16), Vect2D(0, 0));

            if (!isPermittedByMapStaticObj(manager, spawnPos, coll))
                continue;

            spawnEntityFunc(manager, textureLoader, spawnPos);
        }
    }
    std::unordered_map<int, bool> roomSpawned;

public:
    void update(EntitiesManager& manager, TextureLoader& textureLoader) {
        auto player = manager.with<PositionOnMapComponent>().withClassName("*player*").getComponent<PositionOnMapComponent>();
        if (!player) return;

        auto mapComp = manager.with<MapComponent>().getComponent<MapComponent>();
        if (!mapComp || mapComp->map.type == mapType::Preloaded) return;

        Vect2D playerPos = player->point;

        for (size_t i = 0; i < mapComp->map.roomsMap.size(); i++) {
            auto& room = mapComp->map.roomsMap[i];

            if (roomSpawned[i]) continue;

            if (isPlayerInsideRoom(playerPos, room)) {
                spawnItemsInRoom(manager, textureLoader, room);
                roomSpawned[i] = true;
            }
        }
    }

    void reset() {
        roomSpawned.clear();
    }
};
