#pragma once
#include "Vect.hpp"

inline bool collisionRect(Vect2D point, Vect2D pos, Vect2D size) {
    return point.x > pos.x &&
    point.y > pos.y &&
    point.x < pos.x + size.x &&
    point.y < pos.y + size.y;
}

inline bool boxesOverlap(const Vect2D& aPos, const Vect2D& aSize, const Vect2D& bPos, const Vect2D& bSize) {
    return aPos.x < bPos.x + bSize.x &&
           aPos.x + aSize.x > bPos.x &&
           aPos.y < bPos.y + bSize.y &&
           aPos.y + aSize.y > bPos.y;
}