#include "map.hpp"

void positionUpdate(sf::Vector2i& playerPos) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        playerPos.x -= 5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            playerPos.x -= 95;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        playerPos.x += 5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            playerPos.x += 95;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        playerPos.y -= 5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            playerPos.y -= 95;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        playerPos.y += 5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            playerPos.y += 95;
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 500), "Cave Generator");
    window.setFramerateLimit(60);
    
    GameMap map;
    map.generateMap(GameMap::mapType::Circle);

    sf::Texture floorTex;
    floorTex.loadFromFile("/home/xac1k/Desktop/Project Practicum/mygame/Store/view/Tiles/Floor.png");
    sf::Texture emptyTex;
    emptyTex.loadFromFile("/home/xac1k/Desktop/Project Practicum/mygame/Store/view/Tiles/Empty.png");
    sf::Texture wallTex;
    wallTex.loadFromFile("/home/xac1k/Desktop/Project Practicum/mygame/Store/view/Tiles/Wall.png");
    TileRenderer render(floorTex, emptyTex, wallTex);

    miniMapRenderer miniMap;
    miniMap.pos = sf::Vector2f(0, 0);
    miniMap.size = sf::Vector2f(200, 200);

    sf::Vector2i playerPos(map.roomsMap[0].x * 32, map.roomsMap[0].y * 32);
    sf::Vector2i visiable(2000/32, 1000/32);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        positionUpdate(playerPos);
        window.clear(sf::Color::Black);

        render.render(map, window, playerPos, visiable);
        miniMap.draw(window, map.roomsMap, map.spiralCorridors, playerPos/32);

        window.display();
    }
}