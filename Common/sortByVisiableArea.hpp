#pragma once
#include <vector>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>

inline std::vector<int> sortByVisiableArea(EntitiesManager& manager, std::vector<int> entityIDs) {
    std::vector<int> mapIDs = manager.with<MapComponent>().get();
    if(mapIDs.empty()) return {};
    auto mapComponent = manager.getComponent<MapComponent>(mapIDs[0]);

    // Получение резмеров и позиции видимой области
    Vect2D visibleArea = mapComponent->visiableArea * TILE_SIZE;
    Vect2D leftUpP = mapComponent->cameraPos - visibleArea/2;
    Vect2D RightDownP = mapComponent->cameraPos + visibleArea/2;

    std::vector<int> result;

    for(int entityID : entityIDs) {
        if(!manager.hasComponent<PositionOnMapComponent>(entityID)) continue;

        auto entityPos = manager.getComponent<PositionOnMapComponent>(entityID)->point;
        if(entityPos > leftUpP && entityPos < RightDownP) 
            result.push_back(entityID);
    }

    return result;
}