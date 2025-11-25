#include "circleMapGenerator.hpp"

sf::VertexArray dataToVisual(spiralType spiral, sf::Color color) {
    int points = spiral.size();
    sf::VertexArray spiralShape(sf::LinesStrip, points);

    for (int i = 0; i < points; i++) {
        spiralShape[i].position = spiral[i];
        spiralShape[i].color = color;
    }

    return spiralShape;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH_MAP_CIRCLE, HEIGHT_MAP_CIRCLE), "SFML Spiral Map");

    auto [spiral, rooms] = generateSpiralMap();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();

        window.clear(sf::Color::Black);
        window.draw(dataToVisual(spiral, sf::Color::Cyan));
        for(auto r : rooms) {
            sf::CircleShape point;
            point.setPosition(r.x, r.y);
            point.setRadius(r.z);
            point.setFillColor(sf::Color(0, 100, 0));
            point.setOutlineThickness(2);
            point.setOutlineColor(sf::Color::Green);
            point.setOrigin(r.z/2, r.z/2);
            window.draw(point);
        }
        window.display();
    }
}