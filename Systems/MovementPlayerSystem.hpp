#pragma once
#include <Entities/components/map.hpp>
#include <Entities/components/player.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <SFML/Graphics.hpp>

constexpr float lenVeloVect = TILE_SIZE * 8;

enum class Direction {
    left, 
    right, 
    down, 
    up,
    leftUp, 
    leftDown, 
    rightUp, 
    rightDown,
    none
};

Direction getDirection(Vect2D vect) {
    if(vect.x < 0 && vect.y > 0) return Direction::leftDown;
    if(vect.x > 0 && vect.y > 0) return Direction::rightDown;
    if(vect.x < 0 && vect.y < 0) return Direction::leftUp;
    if(vect.x > 0 && vect.y < 0) return Direction::rightUp;
    if(vect.x < 0) return Direction::left;
    if(vect.x > 0) return Direction::right;
    if(vect.y < 0) return Direction::up;
    if(vect.y > 0) return Direction::down;
    return Direction::none;
}

std::tuple<int, int> isPermitted(MapComponent* map, PlayerPosComponent* playerPos, CollisionComponent* rect, SizeComponent* size, VelocityComponent* velo, float df) {
    auto collRectLeftUp = playerPos->point - Vect2D(size->size.x/2, size->size.y/2) + rect->shiftFromLeftUp + velo->dir * df;
    sf::Vector2i Left = {(int)collRectLeftUp.x, (int)(collRectLeftUp.y + rect->size.y/2)};
    sf::Vector2i LeftUp = {(int)collRectLeftUp.x, (int)collRectLeftUp.y};
    sf::Vector2i Up = {(int)(collRectLeftUp.x + rect->size.x/2), (int)collRectLeftUp.y};
    sf::Vector2i RigthUp = {(int)(collRectLeftUp.x + rect->size.x), (int)collRectLeftUp.y};
    sf::Vector2i Rigth = {(int)(collRectLeftUp.x + rect->size.x), (int)(collRectLeftUp.y + rect->size.y/2)};
    sf::Vector2i RigthDown = {(int)(collRectLeftUp.x + rect->size.x), (int)(collRectLeftUp.y + rect->size.y)};
    sf::Vector2i Down = {(int)(collRectLeftUp.x + rect->size.x/2), (int)(collRectLeftUp.y + rect->size.y)};
    sf::Vector2i LeftDown = {(int)(collRectLeftUp.x), (int)(collRectLeftUp.y + rect->size.y)};

    auto dir = getDirection(velo->dir);

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
        if(dir == Direction::leftUp && UpColl && !leftColl) return std::make_tuple<int, int>(1, 0);
        if(dir == Direction::leftUp && RigthUpColl && !leftColl) return std::make_tuple<int, int>(1, 0);
        if(dir == Direction::rightUp && UpColl && !RigthColl) return std::make_tuple<int, int>(1, 0);
        if(dir == Direction::rightUp && leftUpColl && !RigthColl) return std::make_tuple<int, int>(1, 0);

        if(dir == Direction::leftUp && leftColl && !UpColl) return std::make_tuple<int, int>(0, 1);
        if(dir == Direction::leftUp && LeftDownColl && !UpColl) return std::make_tuple<int, int>(0, 1);
        if(dir == Direction::leftDown && leftUpColl && !DownColl) return std::make_tuple<int, int>(0, 1);
        if(dir == Direction::leftDown && leftColl && !DownColl) return std::make_tuple<int, int>(0, 1);

        if(dir == Direction::rightDown && LeftDownColl && !RigthColl) return std::make_tuple<int, int>(1, 0);
        if(dir == Direction::leftDown && RigthDownColl && !leftColl) return std::make_tuple<int, int>(1, 0);

        if(dir == Direction::rightUp && RigthDownColl && !UpColl) return std::make_tuple<int, int>(0, 1);
        if(dir == Direction::rightDown && RigthUpColl && !DownColl) return std::make_tuple<int, int>(0, 1);
        return std::make_tuple<int, int>(0, 0);
    }
    return std::make_tuple<int, int>(1, 1);
}

void CreateMovementPlayerSystem(EntitiesManager& manager) {
    auto playerIds = manager.with<PlayerPosComponent>().get();
    if(playerIds.size() == 0) return;

    auto playerVelocity = manager.getComponent<VelocityComponent>(playerIds[0]).get();
    playerVelocity->dir.x = 0;
    playerVelocity->dir.y = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        playerVelocity->dir.x = -lenVeloVect;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

        playerVelocity->dir.x = lenVeloVect;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

        playerVelocity->dir.y = -lenVeloVect;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

        playerVelocity->dir.y = lenVeloVect;
    }
}

void MovementPlayerSystem(EntitiesManager& manager, float df) {
    CreateMovementPlayerSystem(manager);
    auto playerIds = manager.with<PlayerPosComponent>().get();
    if(playerIds.size() == 0) return;

    auto playerMutex = manager.getComponent<MutexComponent<ControlFlow>>(playerIds[0]).get();
    if(playerMutex->blocked && playerMutex->WhoIsUsing != ControlFlow::All) return;

    auto mapIds = manager.with<MapComponent>().get();
    if(mapIds.size() == 0) return;

    auto playerVelocity = manager.getComponent<VelocityComponent>(playerIds[0]).get();
    auto playerPos = manager.getComponent<PlayerPosComponent>(playerIds[0]).get();
    auto playerState = manager.getComponent<StateComponent>(playerIds[0]).get();
    auto playerCollisionRect = manager.getComponent<CollisionComponent>(playerIds[0]).get();
    auto playerSize = manager.getComponent<SizeComponent>(playerIds[0]).get();

    auto map = manager.getComponent<MapComponent>(mapIds[0]).get();
    auto [xCoef, yCoef] = isPermitted(map, playerPos, playerCollisionRect, playerSize, playerVelocity, df);

    if((playerVelocity->dir.x || playerVelocity->dir.y) && (xCoef || yCoef)) {
        playerPos->point += Vect2D(playerVelocity->dir.x * xCoef * df, playerVelocity->dir.y * yCoef * df);

        if(0 < playerVelocity->dir.x * xCoef) playerState->state = (int)PlayerState::WalkRight;
        if(0 > playerVelocity->dir.x * xCoef) {playerState->state = (int)PlayerState::WalkLeft;}
        if(0 < playerVelocity->dir.y * yCoef) {playerState->state = (int)PlayerState::WalkDirect;}
        if(0 > playerVelocity->dir.y * yCoef) {playerState->state = (int)PlayerState::WalkBackward;}
    }
    else {
        if(playerState->state == (int)PlayerState::WalkRight) playerState->state = (int)PlayerState::IdleRight;
        if(playerState->state == (int)PlayerState::WalkLeft) playerState->state = (int)PlayerState::IdleLeft;
        if(playerState->state == (int)PlayerState::WalkDirect) playerState->state = (int)PlayerState::IdleDirect;
        if(playerState->state == (int)PlayerState::WalkBackward) playerState->state = (int)PlayerState::IdleBackward;
    }
}