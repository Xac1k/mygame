#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"
#include "../../main.h"

constexpr int TILE_SIZE = 32;
int map(EntitiesManager& manager, TextureLoader& textureLoader) {
    auto floorTex = textureLoader.getTexture("Store/view/Tiles/Floor.png");
    auto emptyTex = textureLoader.getTexture("Store/view/Tiles/Empty.png");
    auto wallTex = textureLoader.getTexture("Store/view/Tiles/Wall.png");

    manager.addEntity();
    
    MapComponent mapComponent;
    mapComponent.map.generateMap(GameMap::mapType::Rect);
    mapComponent.cameraPos = Vect2D(mapComponent.map.roomsMap[0].x * TILE_SIZE, mapComponent.map.roomsMap[0].y * TILE_SIZE);
    mapComponent.visiableArea = Vect2D(2000/TILE_SIZE, 1000/TILE_SIZE);
    mapComponent.TileSize = TILE_SIZE;
    mapComponent.tileRender = TileRenderer (floorTex, emptyTex, wallTex);
    manager.addComponent<MapComponent>(mapComponent);

    MiniMapComponent miniMapComponent;
    miniMapComponent.miniMap.pos = sf::Vector2f(0, 0);
    miniMapComponent.miniMap.size = sf::Vector2f(200, 200);
    manager.addComponent<MiniMapComponent>(miniMapComponent);

    return  manager.getID();
}