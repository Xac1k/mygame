#pragma once
#include <SFML/Graphics.hpp>
#include "./Interfaces/busEvent.h"
#include "./Infrastructure/sfml.h"
#include "./Entities/utils/entitiesManager.hpp"
#include "./Systems/ButtonUpdateSystem.hpp"
#include "./Systems/AnimationUpdate.hpp"
#include "./Systems/InventoryUpdateSystem.hpp"
#include <Sounds/soundManager.hpp>

#include <Entities/components/button.hpp>
#include <Entities/components/backgound.hpp>
#include <Entities/components/inventory.hpp>
#include <Entities/components/player.hpp>
#include <Entities/components/map.hpp>
#include <Entities/components/gameState.hpp>

#include <Systems/InventoryDrawSystem.hpp>
#include <Systems/InventoryContextMenuSystem.hpp>
#include <Systems/MapDrawSystem.hpp>
#include <Systems/SpawnSystem.hpp>

extern sf::Vector2f rescaleCoeff;

void StartPage(
    sf::Clock& clock, sf::RenderWindow& window, BusEvent& busEvent,
    EntitiesManager& manager, AudioSystem& audioManager, Animator& animator,
    TextureLoader& textureLoader, SfmlRenderer& renderer
) {
    float df = clock.restart().asSeconds();
    sf::Event event;

    while(window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window.close();
        }
        if(event.type == sf::Event::Resized) {
            sf::Vector2u winSize = window.getSize();
            rescaleCoeff.x = (float)winSize.x / WINDOW_WIDTH;
            rescaleCoeff.y = (float)winSize.y / WINDOW_HEIGHT;
            std::cout << "Разрешение изменено коофициент пропорции: " << rescaleCoeff.x << 'x' << rescaleCoeff.y << std::endl;
        }
        busEvent.update(event, rescaleCoeff);
        ButtonUpdate(manager, busEvent);
    }
    animator.AnimationUpdate(manager, df);

    window.clear(sf::Color::White);
    renderer.render(window, manager, textureLoader);
    window.display();
}