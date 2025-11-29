#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <iostream>
#include "../Config/configCircle.hpp"
#include "../Common/randomRange.hpp"
#include "../Common/collisionCycle.hpp"
#include "../Common/dist.hpp"

inline sf::Vector2f circleCord(const float radius, const float angleDeg) {
    sf::Vector2f p;
    p.x = sin((angleDeg * M_PI / 180)) * radius;
    p.y = cos((angleDeg * M_PI / 180)) * radius;

    return p;
}

inline spiralType createArchimedeanSpiral(
    float initialRadius = 0.0f,
    float gain = 1.0f,
    float turns = 8.0f,
    int points = 2000)
{
    spiralType spiral;
    sf::Vector2f center(WIDTH_MAP_CIRCLE * 0.5f, HEIGHT_MAP_CIRCLE * 0.5f);

    for (int i = 0; i < points; ++i)
    {
        float t = (float)(i) / (points - 1);
        float angle = t * turns * 2.0f * M_PI;
        float r = initialRadius + gain * angle;

        float x = r * std::cos(angle);
        float y = r * std::sin(angle);

        spiral.push_back(center + sf::Vector2f(x, y));
    }

    return spiral;
}

inline sf::Vector2f getPointFromCenterOnLen(float targetLength, spiralType spiral) {
    float length = 0;
    int id = 0;
    while (length < targetLength && id < spiral.size() - 1 && spiral.size() > 1)
    {   
        length += dist(spiral[id], spiral[id + 1]);
        id++;
    }

    return spiral[id];
} 

inline void cropSpiralByLength(spiralType& spiral, float targetLength) {
    float length = 0;
    int id = 0;
    while (length < targetLength && id < spiral.size() - 1 && spiral.size() > 1)
    {   
        length += dist(spiral[id], spiral[id + 1]);
        id++;
    }
    spiral.resize(id);
}

inline std::tuple<spiralType, roomsType> generateSpiralMap() {
    srand(time(nullptr));
    spiralType spiral = createArchimedeanSpiral(
        25.0f,      // a
        10.0f,      // b — расстояние между витками 
        8.0f,
        5000
    );

    roomsType rooms;
    int targetRooms = randRange(MIN_NUM_ROOMS_CIRCLE, MAX_NUM_ROOMS_CIRCLE);
    rooms.push_back({spiral[0].x, spiral[0].y, 15});
    float currlen = 0;
    int countRooms = 0;
    while(countRooms < targetRooms) {
        float radius = randRange(MIN_RADIUS_ROOM_CIRCLE, MAX_RADIUS_ROOM_CIRCLE);
        currlen += radius + randRange(MIN_LEN, MAX_LEN);

        sf::Vector2f p = getPointFromCenterOnLen(currlen, spiral);

        bool collision = false;
        for (const auto r : rooms) {
            if (circlesCollision(r, {p.x, p.y, radius}, INTERSECT_ROOMS_CIRCLE)) {
                collision = true;
                break;
            }
        }
        if (!collision) {
            rooms.push_back({p.x, p.y, radius});
            countRooms++;
        }
    }

    cropSpiralByLength(spiral, currlen);

    return std::make_tuple(spiral, rooms);
}
