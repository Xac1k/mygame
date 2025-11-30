#pragma once
#include <SFML/Graphics.hpp>
#include "../Infrastructure/entityManager.h"
#include <Entities/utils/entitiesManager.hpp>


void MapDrawSystem(
    sf::RenderWindow &window, TextureLoader &textureLoader, 
    EntitiesManager& manager
) { 
    auto mapIDs = manager.with<MapComponent>().get();
    auto playerIDs = manager.withClassName("*player*");
    if(mapIDs.size() == 0 || playerIDs.size() == 0) return;

    auto mapComponent = manager.getComponent<MapComponent>(mapIDs[0]).get();
    auto playerComponent = manager.getComponent<PositionOnMapComponent>(playerIDs[0]).get();
    mapComponent->tileRender.render(mapComponent->map, window, playerComponent->point, mapComponent->visiableArea);

    auto miniMapComponent = manager.getComponent<MiniMapComponent>(mapIDs[0]).get();
    sf::Vector2i cameraPos = {(int)playerComponent->point.x / mapComponent->TileSize, (int)playerComponent->point.y / mapComponent->TileSize};
    if(mapComponent->map.spiralCorridors.size() == 0)
        miniMapComponent->miniMap.draw(window, mapComponent->map.roomsMap, mapComponent->map.corridorsMap, cameraPos); 
    else 
        miniMapComponent->miniMap.draw(window, mapComponent->map.roomsMap, mapComponent->map.spiralCorridors, cameraPos); 
}