#include "main.h"
#include "./Interfaces/busEvent.h"
#include "./Infrastructure/sfml.h"
#include "./Entities/utils/entitiesManager.hpp"
#include "./Systems/ButtonUpdateSystem.hpp"
#include "./Systems/AnimationUpdate.hpp"
#include "./Systems/InventoryUpdateSystem.hpp"
#include "./Entities/components/button.hpp"
#include "./Entities/components/backgound.hpp"
#include "./Entities/components/inventory.hpp"
#include <Systems/InventoryDrawSystem.hpp>

void init(EntitiesManager& manager, TextureLoader& textureLoader) {
    inventory(manager, textureLoader);
    // startButton(manager, textureLoader);
    // continueButton(manager, textureLoader);
    // settingButton(manager, textureLoader);
    background(manager, textureLoader);
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Cave", sf::Style::Default, settings);

    BusEvent busEvent;
    SfmlRenderer renderer;
    TextureLoader textureLoader;

    EntitiesManager manager;
    sf::Clock clock;

    init(manager, textureLoader);
    while(window.isOpen())
    {
        float df = clock.restart().asSeconds();
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            busEvent.update(event);
            ButtonUpdate(manager, busEvent);
            InventoryDndUpdate(manager, busEvent);
        }
        AnimationUpdate(manager, df);

        window.clear(sf::Color::White);
        renderer.render(window, manager, textureLoader);
        InventoryDrawSystem(window, textureLoader, manager);
        window.display();
    }
    return 0;
}