#pragma once
#include "Vect.hpp"

bool collisionRect(Vect2D point, Vect2D pos, Vect2D size) {
    return point.x > pos.x &&
    point.y > pos.y &&
    point.x < pos.x + size.x &&
    point.y < pos.y + size.y;
}