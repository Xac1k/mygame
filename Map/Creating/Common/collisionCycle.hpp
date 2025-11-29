#pragma once
#include <SFML/Graphics.hpp>
#include "dist.hpp"

inline bool circlesCollision(sf::Vector3f a, sf::Vector3f b, const int INTERSECT_ROOMS) {
    return dist(a, b) < a.z + b.z + INTERSECT_ROOMS; 
}

