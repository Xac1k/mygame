#include "directMapGenerator.hpp"

int main() {
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(WIDTH_MAP, HEIGHT_MAP), "Cave Generator - Fixed Kruskal");
    window.setFramerateLimit(60);

    auto [rooms, corridors] = generateDirectMap();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        // Рисуем комнаты
        for (auto room : rooms) {
            sf::CircleShape circle(room.z);
            circle.setPosition(room.x - room.z, room.y - room.z);
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
                sf::Vertex(sf::Vector2f(sPoint.x, sPoint.y), sf::Color::Cyan),
                sf::Vertex(sf::Vector2f(ePoint.x, ePoint.y), sf::Color::Cyan)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }

    return 0;
}