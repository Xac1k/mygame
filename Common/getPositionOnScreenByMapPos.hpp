#pragma once
#include <vector>
#include <Entities/utils/entitiesManager.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>
#include <Common/Vect.hpp>
 
Vect2D getPositionOnScreenByMapPos(EntitiesManager& manager, int entityID) {
    std::vector<int> mapIDs = manager.with<MapComponent>().get();
    if(mapIDs.empty()) return Vect2D(0, 0);
    Vect2D cameraPos = manager.getComponent<MapComponent>(mapIDs[0])->cameraPos;

    if(!manager.hasComponent<PositionOnMapComponent>(entityID)) return Vect2D(0, 0);
    Vect2D entityPos = manager.getComponent<PositionOnMapComponent>(entityID)->point;
    Vect2D pos = entityPos - cameraPos + Vect2D(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    if(manager.hasComponent<OriginComponent>(entityID)) {
        auto shift = manager.getComponent<OriginComponent>(entityID)->shift;
        pos -= shift;
    }
    
    return pos;
}