#pragma once
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <map>
#include <Map/Creating/Config/configDirect.hpp>
#include <Map/Creating/Config/configCircle.hpp>
#include <Map/Creating/Circle/circleMapGenerator.hpp>
#include <Map/Creating/Direct/directMapGenerator.hpp>
#include <Common/Vect.hpp>
#include <Common/buildPath.hpp>
#include <main.h>
#include <Common/const.hpp>

enum class TileType {
    Empty,
    Wall,
    Floor,
};

enum class mapType { Rect, Circle, Preloaded };

struct Tile {
    TileType type = TileType::Empty;
};

class GameMap {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 800;
    static constexpr int WallRadius = 2;
    roomsType roomsMap;
    mapType type;
    corridorsType corridorsMap;
    spiralType spiralCorridors;
    Tile nullTile;
    Vect2D spawnPos = {0, 0};

    GameMap() : tiles(WIDTH * HEIGHT) {}

    void resetMap() {
        for(int y = 0; y < HEIGHT; y++) {
            for(int x = 0; x < WIDTH; x++) {
                auto& tile = get(x, y);
                tile.type = TileType::Empty;
            }
        }
    }

    Tile& get(int x, int y) {
        if(x>=0 && y>=0 && (y * WIDTH + x) < tiles.size()) 
            return tiles[y * WIDTH + x];
        return nullTile;
    }

    const Tile& get(int x, int y) const {
        return tiles[y * WIDTH + x];
    }
private:
    void resterizeCircle(sf::Vector3f circle) {
        sf::Vector2f upLeftP;
        upLeftP.x = std::max(0, (int)(circle.x - circle.z - 2));
        upLeftP.y = std::max(0, (int)(circle.y - circle.z - 2));

        sf::Vector2f downRightP;
        downRightP.x = std::min(WIDTH_MAP, (int)(circle.x + circle.z + 2));
        downRightP.y = std::min(HEIGHT_MAP, (int)(circle.y + circle.z + 2));

        for(int y = upLeftP.y; y < downRightP.y; y++) {
            for(int x = upLeftP.x; x < downRightP.x; x++) {
                float dx = x + 0.5f - circle.x;
                float dy = y + 0.5f - circle.y;
                auto& tile = get(x, y);
                if(sqrt(dx * dx + dy * dy) < sqrt(circle.z * circle.z)) {
                    tile.type = TileType::Floor;
                }
                else {
                    if(tile.type != TileType::Floor)
                        if(sqrt(dx * dx + dy * dy) - 1 < sqrt(circle.z * circle.z)) {
                            tile.type = TileType::Wall;
                        }
                }
            }
        }
    }

    void resterizeRooms(roomsType rooms) {
        for(auto room : rooms) {
            resterizeCircle(room);
        }
    }

    void resterizeCorridors(roomsType rooms, corridorsType corridors, float step = 1.f, float thikness = 2.f) {
        for(auto corridor : corridors) {
            sf::Vector3f sPoint = rooms[corridor.first];
            sf::Vector3f ePoint = rooms[corridor.second];

            sf::Vector2f dir(ePoint.x - sPoint.x, ePoint.y - sPoint.y);
            float length = sqrt(dir.x * dir.x + dir.y * dir.y); 

            dir /= length;
            for(float dist = 0; dist < length; dist += step) {
                sPoint.z = thikness / 2;
                sf::Vector3f p = sPoint + sf::Vector3f(dir.x, dir.y, 0) * dist;
                resterizeCircle(p);
            }
        }
    }

    void resterizeSpiralCorridors(roomsType rooms, spiralType spiral, float thikness = 2.f) {
        for(auto p : spiral) {
            sf::Vector3f circle({p.x, p.y, thikness/2});
            resterizeCircle(circle);
        }
    }

    void resterizeMap(roomsType rooms, corridorsType corridors) {
        resterizeRooms(rooms);
        resterizeCorridors(rooms, corridors, 1, 4);
    }

    void resterizeMap(roomsType rooms, spiralType spiral) {
        resterizeRooms(rooms);
        resterizeSpiralCorridors(rooms, spiral, 4);
    }

    Vect2D getSpawnPosition() {
        std::map<int, int> countRooms;
        switch (type)
        {
        case mapType::Rect:
            for(auto pair : corridorsMap) {
                if(countRooms.find(pair.first) == countRooms.end()) 
                    countRooms[pair.first] = 0;

                if(countRooms.find(pair.second) == countRooms.end()) 
                    countRooms[pair.second] = 0;

                countRooms[pair.first]++;
                countRooms[pair.second]++;  
            }
            for(auto candidate : countRooms) {
                if(candidate.second == 1) {
                    auto room = roomsMap.at(candidate.first);
                    return Vect2D(room.x, room.y);
                }
            }
            break;
        case mapType::Circle:
            auto Vect = roomsMap.at(roomsMap.size() - 1);
            return Vect2D(Vect.x, Vect.y);  
        }

        auto room = roomsMap[0];
        return Vect2D(room.x, room.y);
    }
public:
    void generateMap(mapType typeI) {
        srand(time(nullptr));
        type = typeI;
        resetMap();

        if(type == mapType::Rect) {
            auto [rooms, corridors] = generateDirectMap();
            resterizeMap(rooms, corridors);
            roomsMap = rooms;
            corridorsMap = corridors;
        }

        if (type == mapType::Circle) {
            auto [spiral, rooms] = generateSpiralMap();
            resterizeMap(rooms, spiral);
            roomsMap = rooms;
            spiralCorridors = spiral;
        }
    }

    void loadFromFile(std::string localFilename) {
        auto globalPath = buildFullPath(localFilename, 1);

        std::ifstream fileMap(globalPath);
        if (!fileMap.is_open()) {
            std::cerr << "Не удалось открыть файл для чтения! Путь: " << globalPath << std::endl;
            return;
        }
        resetMap();
        type = mapType::Preloaded;

        std::string line;
        std::getline(fileMap, line);
        auto width = std::stoi(line);
        std::getline(fileMap, line);
        auto height = std::stoi(line);

        auto init_height = HEIGHT/2 - height/2;
        auto init_width = WIDTH/2 - width/2;

        int heightID = 0;
        while(std::getline(fileMap, line)) {
            for(int id = 0; id < line.size(); id++) {
                char type = line[id];
                Vect2D point((float)(init_width + id), (float)(init_height + heightID));
                auto& tile = get((int)point.x, (int)point.y);
                if(type == '#') tile.type = TileType::Wall;
                if(type == '.') tile.type = TileType::Floor;
                if(type == 'x') {
                    spawnPos = point * TILE_SIZE;
                    tile.type = TileType::Floor;
                }
            }
            heightID++;
        }
    }

    Vect2D getSpawnPoint() {
        if(type == mapType::Preloaded)
            return spawnPos;
        else
            return getSpawnPosition() * TILE_SIZE;
    }
    private:
        std::vector<Tile> tiles;
};

class TileRenderer {
public:
    static constexpr int TILE_SIZE = 32;

    TileRenderer( sf::Texture& floorTex, sf::Texture& emptyTex, sf::Texture& wallTex) {
        textures[TileType::Floor] = floorTex;
        textures[TileType::Empty] = emptyTex;
        textures[TileType::Wall] = wallTex;
    }

    TileRenderer() {}

    void render(GameMap& map, sf::RenderWindow& window, sf::Vector2i& cameraPosPixel, sf::Vector2i& visiableArea) {
        sf::Vector2f tempShift;
        tempShift.x = cameraPosPixel.x % TILE_SIZE;
        tempShift.y = cameraPosPixel.y % TILE_SIZE;

        sf::Vector2i cameraPos(cameraPosPixel.x/TILE_SIZE, cameraPosPixel.y/TILE_SIZE);

        int left   = cameraPos.x - visiableArea.x/2;
        int top    = cameraPos.y - visiableArea.y/2;
        int right  = std::min(GameMap::WIDTH, cameraPos.x + visiableArea.x/2);
        int bottom = std::min(GameMap::HEIGHT, cameraPos.y + visiableArea.y/2);

        sf::Vector2u winSize = window.getSize();
        sf::Vector2f shiftAlign;
        shiftAlign.x = - visiableArea.x/2 * TILE_SIZE + (float)(winSize.x/2);
        shiftAlign.y = - (float)(visiableArea.y/2) * TILE_SIZE +  (float)(winSize.y/2);

        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                Tile tile = map.get(x, y);
                auto texture = textures.find(tile.type);
                if (texture == textures.end()) continue;

                sf::Sprite sprite(texture->second);
                auto size = texture->second.getSize();

                sf::Vector2f pos({(float)((x - left) * TILE_SIZE), (float)((y - top) * TILE_SIZE)});

                sprite.setPosition(pos + shiftAlign - tempShift);
                sprite.setScale(TILE_SIZE/size.x, TILE_SIZE/size.y);
                window.draw(sprite);
            }
        }
    }

    void render(GameMap& map, sf::RenderWindow& window, Vect2D& cameraPosPixel, Vect2D& visiableArea) {
        sf::Vector2f tempShift;
        tempShift.x = (int)cameraPosPixel.x % TILE_SIZE;
        tempShift.y = (int)cameraPosPixel.y % TILE_SIZE;

        sf::Vector2i cameraPos(cameraPosPixel.x/TILE_SIZE, cameraPosPixel.y/TILE_SIZE);

        int left   = cameraPos.x - (int)visiableArea.x/2;
        int top    = cameraPos.y - (int)visiableArea.y/2;
        int right  = std::min(GameMap::WIDTH, cameraPos.x + (int)visiableArea.x/2);
        int bottom = std::min(GameMap::HEIGHT, cameraPos.y + (int)visiableArea.y/2);

        sf::Vector2u winSize = window.getSize();
        sf::Vector2f shiftAlign;
        shiftAlign.x = - (int)visiableArea.x/2 * TILE_SIZE + (float)(winSize.x/2);
        shiftAlign.y = - (float)(visiableArea.y/2) * TILE_SIZE +  (float)(winSize.y/2);

        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                Tile tile = map.get(x, y);
                auto texture = textures.find(tile.type);
                if (texture == textures.end()) continue;

                sf::Sprite sprite(texture->second);
                auto size = texture->second.getSize();

                sf::Vector2f pos({(float)((x - left) * TILE_SIZE), (float)((y - top) * TILE_SIZE)});

                sf::Vector2f Position = pos + shiftAlign - tempShift;
                sprite.setPosition(Position.x, Position.y);
                sprite.setScale(TILE_SIZE/size.x, TILE_SIZE/size.y);

                window.draw(sprite);
            }
        }
    }

    void render(GameMap& map, sf::RenderTexture& window, sf::Vector2i& cameraPosPixel, sf::Vector2i& visiableArea) {
        sf::Vector2f tempShift;
        tempShift.x = cameraPosPixel.x % TILE_SIZE;
        tempShift.y = cameraPosPixel.y % TILE_SIZE;

        sf::Vector2i cameraPos(cameraPosPixel.x/TILE_SIZE, cameraPosPixel.y/TILE_SIZE);

        int left   = cameraPos.x - visiableArea.x/2;
        int top    = cameraPos.y - visiableArea.y/2;
        int right  = std::min(GameMap::WIDTH, cameraPos.x + visiableArea.x/2);
        int bottom = std::min(GameMap::HEIGHT, cameraPos.y + visiableArea.y/2);

        sf::Vector2u winSize = window.getSize();
        sf::Vector2f shiftAlign;
        shiftAlign.x = - visiableArea.x/2 * TILE_SIZE + (float)(winSize.x/2);
        shiftAlign.y = - (float)(visiableArea.y/2) * TILE_SIZE +  (float)(winSize.y/2);

        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                Tile tile = map.get(x, y);
                auto texture = textures.find(tile.type);
                if (texture == textures.end()) continue;

                sf::Sprite sprite(texture->second);
                auto size = texture->second.getSize();

                sf::Vector2f pos({(float)((x - left) * TILE_SIZE), (float)((y - top) * TILE_SIZE)});

                sprite.setPosition(pos + shiftAlign - tempShift);
                sprite.setScale(TILE_SIZE/size.x, TILE_SIZE/size.y);
                window.draw(sprite);
            }
        }
    }

    void render(GameMap& map, sf::RenderTexture& window, Vect2D& cameraPosPixel, Vect2D& visiableArea) {
        sf::Vector2f tempShift;
        tempShift.x = (int)cameraPosPixel.x % TILE_SIZE;
        tempShift.y = (int)cameraPosPixel.y % TILE_SIZE;

        sf::Vector2i cameraPos(cameraPosPixel.x/TILE_SIZE, cameraPosPixel.y/TILE_SIZE);

        int left   = cameraPos.x - (int)visiableArea.x/2;
        int top    = cameraPos.y - (int)visiableArea.y/2;
        int right  = std::min(GameMap::WIDTH, cameraPos.x + (int)visiableArea.x/2);
        int bottom = std::min(GameMap::HEIGHT, cameraPos.y + (int)visiableArea.y/2);

        sf::Vector2u winSize = window.getSize();
        sf::Vector2f shiftAlign;
        shiftAlign.x = - (int)visiableArea.x/2 * TILE_SIZE + (float)(WINDOW_WIDTH/2);
        shiftAlign.y = - (float)(visiableArea.y/2) * TILE_SIZE +  (float)(WINDOW_HEIGHT/2);

        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                Tile tile = map.get(x, y);
                auto texture = textures.find(tile.type);
                if (texture == textures.end()) continue;

                sf::Sprite sprite(texture->second);
                auto size = texture->second.getSize();

                sf::Vector2f pos({(float)((x - left) * TILE_SIZE), (float)((y - top) * TILE_SIZE)});

                sf::Vector2f Position = pos + shiftAlign - tempShift;
                sprite.setPosition(Position.x, Position.y);
                sprite.setScale(TILE_SIZE/size.x, TILE_SIZE/size.y);

                window.draw(sprite);
            }
        }
    }

private:
    std::map<TileType, sf::Texture> textures;
};

class miniMapRenderer {
    public:
        sf::Vector2f pos;
        sf::Vector2f size;

        void draw(sf::RenderWindow& window, roomsType rooms, corridorsType corridors, sf::Vector2i cameraPos) {
            float scaleX = size.x / WIDTH_MAP;
            float scaleY = size.y / HEIGHT_MAP;

            // Рисуем комнаты
            for (auto room : rooms) {
                sf::CircleShape circle((room.z) * (scaleY/2 + scaleX/2));
                circle.setPosition((room.x - room.z + pos.x) * scaleX, (room.y - room.z + pos.y) * scaleY);
                circle.setFillColor(sf::Color(0, 100, 0));
                circle.setOutlineThickness(2);
                circle.setOutlineColor(sf::Color::Green);
                window.draw(circle);
            }

            // Рисуем коридоры
            for (auto corr : corridors) {
                sf::Vector3f sPoint = rooms[corr.first];
                sf::Vector3f ePoint = rooms[corr.second];

                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f((sPoint.x+pos.x) * scaleX, (sPoint.y+pos.y) * scaleY), sf::Color::Cyan),
                    sf::Vertex(sf::Vector2f((ePoint.x+pos.x) * scaleX, (ePoint.y+pos.y) * scaleY), sf::Color::Cyan)
                };

                window.draw(line, 2, sf::Lines);
            }

            sf::CircleShape camera;
            camera.setRadius(2);
            camera.setFillColor(sf::Color::Magenta);
            camera.setPosition((cameraPos.x + pos.x) * scaleX, (cameraPos.y+pos.y) * scaleY);
            window.draw(camera);
        }

        void draw(sf::RenderWindow& window, roomsType rooms, spiralType spiral, sf::Vector2i cameraPos) {
            float scaleX = size.x / WIDTH_MAP;
            float scaleY = size.y / HEIGHT_MAP;
            sf::Vector2f scale(scaleX, scaleY);
            // Рисуем комнаты
            for (auto room : rooms) {
                sf::CircleShape circle((room.z) * (scaleY + scaleX) / 2);
                circle.setPosition((room.x - room.z) * scaleX, (room.y - room.z) * scaleY);
                circle.setFillColor(sf::Color(0, 100, 0));
                circle.setOutlineThickness(2);
                circle.setOutlineColor(sf::Color::Green);
                window.draw(circle);
            }

            int points = spiral.size();
            sf::VertexArray spiralShape(sf::LinesStrip, points);

            for (int i = 0; i < points; i++) {
                
                spiralShape[i].position = {spiral[i].x * scale.x, spiral[i].y * scale.y};
                spiralShape[i].color = sf::Color::Cyan;
            }

            window.draw(spiralShape);

            sf::CircleShape camera;
            camera.setRadius(2);
            camera.setFillColor(sf::Color::Magenta);
            camera.setPosition((cameraPos.x + pos.x) * scaleX, (cameraPos.y+pos.y) * scaleY);
            window.draw(camera);
        }

        void draw(sf::RenderTexture& window, roomsType rooms, corridorsType corridors, sf::Vector2i cameraPos) {
            float scaleX = size.x / WIDTH_MAP;
            float scaleY = size.y / HEIGHT_MAP;

            // Рисуем комнаты
            for (auto room : rooms) {
                sf::CircleShape circle((room.z) * (scaleY/2 + scaleX/2));
                circle.setPosition((room.x - room.z + pos.x) * scaleX, (room.y - room.z + pos.y) * scaleY);
                circle.setFillColor(sf::Color(0, 100, 0));
                circle.setOutlineThickness(2);
                circle.setOutlineColor(sf::Color::Green);
                window.draw(circle);
            }

            // Рисуем коридоры
            for (auto corr : corridors) {
                sf::Vector3f sPoint = rooms[corr.first];
                sf::Vector3f ePoint = rooms[corr.second];

                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f((sPoint.x+pos.x) * scaleX, (sPoint.y+pos.y) * scaleY), sf::Color::Cyan),
                    sf::Vertex(sf::Vector2f((ePoint.x+pos.x) * scaleX, (ePoint.y+pos.y) * scaleY), sf::Color::Cyan)
                };

                window.draw(line, 2, sf::Lines);
            }

            sf::CircleShape camera;
            camera.setRadius(2);
            camera.setFillColor(sf::Color::Magenta);
            camera.setPosition((cameraPos.x + pos.x) * scaleX, (cameraPos.y+pos.y) * scaleY);
            window.draw(camera);
        }

        void draw(sf::RenderTexture& window, roomsType rooms, spiralType spiral, sf::Vector2i cameraPos) {
            float scaleX = size.x / WIDTH_MAP;
            float scaleY = size.y / HEIGHT_MAP;
            sf::Vector2f scale(scaleX, scaleY);
            // Рисуем комнаты
            for (auto room : rooms) {
                sf::CircleShape circle((room.z) * (scaleY + scaleX) / 2);
                circle.setPosition((room.x - room.z) * scaleX, (room.y - room.z) * scaleY);
                circle.setFillColor(sf::Color(0, 100, 0));
                circle.setOutlineThickness(2);
                circle.setOutlineColor(sf::Color::Green);
                window.draw(circle);
            }

            int points = spiral.size();
            sf::VertexArray spiralShape(sf::LinesStrip, points);

            for (int i = 0; i < points; i++) {
                
                spiralShape[i].position = {spiral[i].x * scale.x, spiral[i].y * scale.y};
                spiralShape[i].color = sf::Color::Cyan;
            }

            window.draw(spiralShape);

            sf::CircleShape camera;
            camera.setRadius(2);
            camera.setFillColor(sf::Color::Magenta);
            camera.setPosition((cameraPos.x + pos.x) * scaleX, (cameraPos.y+pos.y) * scaleY);
            window.draw(camera);
        }
};