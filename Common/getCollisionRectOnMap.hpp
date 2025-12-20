#pragma once
#include <map>
#include <Common/Vect.hpp>
#include <Entities/utils/component.hpp>

std::pair<Vect2D, Vect2D> getCollisionRectOnMap(Vect2D pos, Vect2D origin, CollisionComponent *collRect) {
    auto playerCollRectLeftUp = pos - origin + collRect->shiftFromLeftUp;
    return std::make_pair(playerCollRectLeftUp, collRect->size);
}