#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <iostream>
#include "../Config/configDirect.hpp"
#include "../Common/randomRange.hpp"
#include "../Common/collisionCycle.hpp"
#include "../Common/dist.hpp"

struct Edge {
    int u, v;
    float weight;
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

void paint(std::map<int, int>& coloredVertex, int x, int y) {
    int targetColor = coloredVertex[y];
    int mainColor = coloredVertex[x];

    for(auto& color : coloredVertex) {
        if(color.second == targetColor) color.second = mainColor;
    }
}

std::tuple<roomsType, corridorsType> generateDirectMap() {
    roomsType rooms;
    corridorsType corridors;

    // Генерация комнат
    int targetRooms = MIN_NUM_ROOMS + rand() % (MAX_NUM_ROOMS - MIN_NUM_ROOMS + 1);
    int attempts = 0;
    while (rooms.size() < targetRooms && attempts < MAX_ATTEMPTS) {
        attempts++;
        sf::Vector3f room;
        room.x = randRange(MAX_RADIUS_ROOM, WIDTH_MAP - MAX_RADIUS_ROOM);
        room.y = randRange(MAX_RADIUS_ROOM, HEIGHT_MAP - MAX_RADIUS_ROOM);
        room.z = randRange(MIN_RADIUS_ROOM, MAX_RADIUS_ROOM);

        bool collision = false;
        for (const auto r : rooms) {
            if (circlesCollision(r, room, INTERSECT_ROOMS)) {
                collision = true;
                break;
            }
        }
        if (!collision) rooms.push_back(room);
    }

    // Алгоритм Краскала
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    
    for (int i = 0; i < rooms.size(); ++i) {
        for (int j = i + 1; j < rooms.size(); ++j) {
            pq.push({i, j, dist(rooms[i], rooms[j])});
        }
    }

    std::map<int, int> coloredVertex;
    for (int i = 0; i < rooms.size(); ++i) coloredVertex[i] = i;

    while (!pq.empty()) {
        Edge e = pq.top();
        pq.pop();

        int uColor = coloredVertex[e.u];
        int vColor = coloredVertex[e.v];

        if (uColor != vColor) {
            paint(coloredVertex, e.u, e.v);
            corridors.emplace_back(e.u, e.v);

            if (corridors.size() >= rooms.size() - 1)
                break;
        }
    }

    // crop - приводим в начало координат
    float minX=MAXFLOAT; float minY=MAXFLOAT;
    for(auto room : rooms) {
        if(minX > room.x - room.z) minX = room.x - room.z;
        if(minY > room.y - room.z) minY = room.y - room.z;
    }

    for(auto& room : rooms) {
        room.x -= minX;
        room.y -= minY;
    }

    return std::make_tuple(rooms, corridors);
}



