#pragma once
#include <Common/Vect.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/components/map.hpp>

bool isPermittedByMapStaticObj(EntitiesManager& manager, Vect2D pos, CollisionComponent rect) {
    auto mapIDs = manager.with<MapComponent>().get();
    if(mapIDs.size() == 0) return false;
    auto map = manager.getComponent<MapComponent>(mapIDs[0]);

    auto collRectLeftUp = pos + rect.shiftFromLeftUp;
    sf::Vector2i Left = {(int)collRectLeftUp.x, (int)(collRectLeftUp.y + rect.size.y/2)};
    sf::Vector2i LeftUp = {(int)collRectLeftUp.x, (int)collRectLeftUp.y};
    sf::Vector2i Up = {(int)(collRectLeftUp.x + rect.size.x/2), (int)collRectLeftUp.y};
    sf::Vector2i RigthUp = {(int)(collRectLeftUp.x + rect.size.x), (int)collRectLeftUp.y};
    sf::Vector2i Rigth = {(int)(collRectLeftUp.x + rect.size.x), (int)(collRectLeftUp.y + rect.size.y/2)};
    sf::Vector2i RigthDown = {(int)(collRectLeftUp.x + rect.size.x), (int)(collRectLeftUp.y + rect.size.y)};
    sf::Vector2i Down = {(int)(collRectLeftUp.x + rect.size.x/2), (int)(collRectLeftUp.y + rect.size.y)};
    sf::Vector2i LeftDown = {(int)(collRectLeftUp.x), (int)(collRectLeftUp.y + rect.size.y)};

    bool leftColl = map->map.get(Left.x/TILE_SIZE, Left.y/TILE_SIZE).type != TileType::Floor;
    bool leftUpColl = map->map.get(LeftUp.x/TILE_SIZE, LeftUp.y/TILE_SIZE).type != TileType::Floor;
    bool UpColl = map->map.get(Up.x/TILE_SIZE, Up.y/TILE_SIZE).type != TileType::Floor;
    bool RigthUpColl = map->map.get(RigthUp.x/TILE_SIZE, RigthUp.y/TILE_SIZE).type != TileType::Floor;
    bool RigthColl = map->map.get(Rigth.x/TILE_SIZE, Rigth.y/TILE_SIZE).type != TileType::Floor;
    bool RigthDownColl = map->map.get(RigthDown.x/TILE_SIZE, RigthDown.y/TILE_SIZE).type != TileType::Floor;
    bool DownColl = map->map.get(Down.x/TILE_SIZE, Down.y/TILE_SIZE).type != TileType::Floor;
    bool LeftDownColl = map->map.get(LeftDown.x/TILE_SIZE, LeftDown.y/TILE_SIZE).type != TileType::Floor;
    
    if(leftColl || leftUpColl || UpColl || RigthUpColl || RigthColl || RigthDownColl || DownColl || LeftDownColl)
    {
        return false;
    }
    return true;
}