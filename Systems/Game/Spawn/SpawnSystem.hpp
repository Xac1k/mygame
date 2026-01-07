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


StoneType getOreTypeByChance() {
    static const std::vector<std::pair<StoneType, float>> ores = {
        {StoneType::coal, 0.5f},
        {StoneType::iron, 0.3f},
        {StoneType::gold, 0.15f},
        {StoneType::rubin, 0.05f}
    };

    float total = 0.0f;
    for (auto& [t, w] : ores) total += w;

    float r = randFloat(); 

    for (auto& [type, weight] : ores) {
        if (r < weight) return type;
        r -= weight;
    }
    return ores.back().first;
}

class MapSort
{
private:
    int DISTANCE_FROM_WALL = 2;
    bool isRuleAppropriate(Vect2D candidate, MapComponent* map) {
        Vect2D checkLeftUp = candidate - Vect2D(DISTANCE_FROM_WALL, DISTANCE_FROM_WALL);
        for(int y = checkLeftUp.y; y < checkLeftUp.y + DISTANCE_FROM_WALL; y++) {
            for(int x = checkLeftUp.x; x < checkLeftUp.x + DISTANCE_FROM_WALL; x++) {
                bool isFloor = map->map.get(x, y).type == TileType::Floor;
                bool isCloseToCenter = dist(Vect2D(x, y), candidate) < DISTANCE_FROM_WALL ;
                if(!isFloor && isCloseToCenter) return false;
            }
        }
        return true;
    }

public:
    std::vector<Vect2D> sort(EntitiesManager& manager) {
        auto mapIDs = manager.with<MapComponent>().get();
        if(mapIDs.size() == 0) return {};

        auto map = manager.getComponent<MapComponent>(mapIDs[0]).get();
        if(!map) return {};

        auto settingsIDs = manager.with<ChunkLoaderComponent>().get();
        if(settingsIDs.size() == 0) return {};

        std::vector<Vect2D> candidates;
        for(int y = 0; y < HEIGHT_MAP; y++) {
            for(int x = 0; x < WIDTH_MAP; x++) {
                if(map->map.get(x, y).type == TileType::Floor) {
                    if(isRuleAppropriate(Vect2D(x, y), map))
                        candidates.push_back({(float)x, (float)y});
                }
            }
        }
        
        return candidates;
    }

    MapSort() = default;
    ~MapSort() = default;
};

class SpawnSystem{
    public:
        int MAX_ATTEMPTS = 5000;
        int MAX_ENTITY_COUNT = 80;
        int MIN_ENTITY_COUNT = 20;

        int MAX_ENTITY_COUNT_INTO_BUNDLE = 5;
        int DIST_BETWEEN_OBJ_IN_GROUP = TILE_SIZE * 3;
        int DIST_BETWEEN_GROUPS = TILE_SIZE * 30;

        int SPAWN_RADIUS = TILE_SIZE * 12;

    private: 
        int currCount = 0;
        int targetCountEntities;
        std::vector<Vect2D> candidates;
        std::vector<Vect2D> prevGroups; //Позиция в tile * TILE_SIZE
        std::vector<int> prevENTITY;

        bool isPermittedByObjects(EntitiesManager& manager, Vect2D pos, CollisionComponent collisionRect) {
            auto enemiesIDs = manager.with<CollisionComponent>().get();
            for(int ID : enemiesIDs) {
                auto enemyPosOnMapComp = manager.getComponent<PositionOnMapComponent>(ID).get()->point;
                Vect2D origin(0, 0);
                if(manager.hasComponent<OriginComponent>(ID)) {
                    auto shift = manager.getComponent<OriginComponent>(ID).get()->shift;
                    origin = shift;
                }
                auto enemyCollRectComp = manager.getComponent<CollisionComponent>(ID).get();
                auto collRectOnMap = getCollisionRectOnMap(enemyPosOnMapComp, origin, enemyCollRectComp);
                if(boxesOverlap(pos + collisionRect.shiftFromLeftUp, collisionRect.size, collRectOnMap.first, collRectOnMap.second)) {
                    return false;
                }
            }
            return true;
        }

        bool isPermittedByDistFromOtherBarrel(EntitiesManager& manager, Vect2D pos) {
            for(auto entitylID :  prevENTITY) {
                Vect2D origin(0, 0);
                if(manager.hasComponent<OriginComponent>(entitylID)) {
                    auto shift = manager.getComponent<OriginComponent>(entitylID).get()->shift;
                    origin = shift;
                }
                auto enemyCollRectComp = manager.getComponent<CollisionComponent>(entitylID).get();
                auto enemyPos = manager.getComponent<PositionOnMapComponent>(entitylID).get()->point;
                auto collRectOnMap = getCollisionRectOnMap(enemyPos, origin, enemyCollRectComp);
                auto distance = dist(pos, collRectOnMap.first);

                if(distance < DIST_BETWEEN_OBJ_IN_GROUP) {
                    return false;
                }

                if(boxesOverlap(pos, sizeCollRectBarrel, collRectOnMap.first, collRectOnMap.second)) {
                    return false;
                }
            }
            return true;
        }

        bool isPermittedByDistFromOtherGroups(EntitiesManager& manager, Vect2D currGroup) {
            for(auto prevGroup : prevGroups) {
                auto distWithGroup = dist(prevGroup, currGroup);
                if(distWithGroup < DIST_BETWEEN_GROUPS) return false;
            }

            return true;
        }

        Vect2D generateGroup(EntitiesManager& manager, TextureLoader& textureLoader) {
            auto barrelsInGroup = randRange(1, MAX_ENTITY_COUNT_INTO_BUNDLE);
            auto candidateID = randRange(0, candidates.size());
            auto groupPos = candidates[candidateID] * TILE_SIZE;
            candidates.erase(candidates.begin() + candidateID);

            //TODO: Добавить обработку колизии с другими группами бочек. Отчистка candidates
            int attempts = 0;
            int initialCountBarrels = currCount;
            while(currCount < targetCountEntities && currCount - initialCountBarrels <  barrelsInGroup && attempts < MAX_ATTEMPTS) {
                auto radius = randFloat() * SPAWN_RADIUS;
                auto angle = randFloat() * 2 * M_PI;
                Vect2D shiftFromCenter = Vect2D(radius, radius) * Vect2D(sin(angle), cos(angle));
                Vect2D posBarrel = groupPos + shiftFromCenter;
                CollisionComponent collisionRect(Vect2D(32, 10), Vect2D(0, 22));
                bool isPremittedByMap = isPermittedByMapStaticObj(manager, posBarrel, collisionRect);
                bool isPermittedByObj = isPermittedByObjects(manager, posBarrel, collisionRect);
                bool isPermittedByBar = isPermittedByDistFromOtherBarrel(manager, posBarrel);

                if(isPremittedByMap && isPermittedByObj && isPermittedByBar) {
                    int entityID = ore(manager, textureLoader, posBarrel, getOreTypeByChance());
                    prevENTITY.push_back(entityID);
                    currCount++;
                } 
                else {
                    attempts++;
                }
            }
            
            prevENTITY.clear();

            return groupPos;
        }
    public:
        void update(EntitiesManager& manager, TextureLoader& textureLoader) {
            auto mapIds = manager.with<MapComponent>().get();
            if(mapIds.size() == 0) return;

            auto map = manager.getComponent<MapComponent>(mapIds[0]).get();
            if(!map) return;

            srand(time(nullptr));
            MapSort sorter;
            candidates = sorter.sort(manager);
            targetCountEntities = randRange(MIN_ENTITY_COUNT, MAX_ENTITY_COUNT);

            while(currCount < targetCountEntities) {
                auto pos = generateGroup(manager, textureLoader);
                std::cout << "Спавн руды" << pos.x << " " << pos.y << std::endl;
            }
        }
};




