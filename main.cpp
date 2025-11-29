#include "main.h"
#include "./Interfaces/busEvent.h"
#include "./Infrastructure/sfml.h"
#include "./Entities/utils/entitiesManager.hpp"

#include <Sounds/soundManager.hpp>

#include <Entities/components/gameState.hpp>
#include <Entities/components/backgound.hpp>
#include <Entities/components/inventory.hpp>
#include <Entities/components/button.hpp>
#include <Entities/components/player.hpp>
#include <Entities/components/map.hpp>

#include <Systems/InventoryContextMenuSystem.hpp>
#include <Systems/InventoryUpdateSystem.hpp>
#include <Systems/MovementPlayerSystem.hpp>
#include <Systems/InventoryDrawSystem.hpp>
#include <Systems/ButtonUpdateSystem.hpp>
#include <Systems/HurtPlayerSystem.hpp>
#include <Systems/AnimationUpdate.hpp>
#include <Systems/MapDrawSystem.hpp>
#include <Systems/SpawnSystem.hpp>

#include <Pages/SettingPage.hpp>
#include <Pages/StartPage.hpp>
#include <Pages/PlayPage.hpp>

//TODO: Переход между экранами
//TODO: Взаимодействия с сундуком

void init(EntitiesManager& manager, TextureLoader& textureLoader, AudioSystem& audioManager) {
    gameState(manager);

    audioManager.loadMusic("Стартовое меню", "Sounds/asset/the_wind.mp3");
    audioManager.loadMusic("PlayerDamage", "Sounds/asset/PlayerDamage.mp3");
    audioManager.playMusic("Стартовое меню", true);
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Cave", sf::Style::Default, settings);

    BusEvent busEvent;
    SfmlRenderer renderer;
    TextureLoader textureLoader;

    EntitiesManager manager;
    AudioSystem audioManager;
    sf::Clock clock;

    Animator animator;

    init(manager, textureLoader, audioManager);
    auto gameStateIds = manager.with<GameStateComponent>().get();
    auto prevGameState = GameScreen::none;
    auto gameState = manager.getComponent<GameStateComponent>(gameStateIds[0]).get();
    std::map<std::string, int> existedEntities;
    
    while(window.isOpen())
    {
        if(gameState->screen == GameScreen::start) {
            if(prevGameState != gameState->screen) {
                prevGameState = gameState->screen;
                existedEntities["background"] = background(manager, textureLoader);
                existedEntities["startBtn"] = startButton(manager, textureLoader);
                existedEntities["continueBtn"] = continueButton(manager, textureLoader);
                existedEntities["settingBtn"] = settingButton(manager, textureLoader);
            }
            StartPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }

        if(gameState->screen == GameScreen::setting) {
            if(prevGameState != gameState->screen) {
                prevGameState = gameState->screen;
                manager.removeEntity(existedEntities["continueBtn"]);
                manager.removeEntity(existedEntities["startBtn"]);
                manager.removeEntity(existedEntities["settingBtn"]);
                existedEntities["plusVolumeBtn"] = plusButton(manager, textureLoader);
                existedEntities["minusVolumeBtn"] = minusButton(manager, textureLoader);
            }
            SettingPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }

        if(gameState->screen == GameScreen::play) {
            if(prevGameState != gameState->screen) {
                prevGameState = gameState->screen;
                manager.removeEntity(existedEntities["background"]);
                manager.removeEntity(existedEntities["continueBtn"]);
                manager.removeEntity(existedEntities["startBtn"]);
                manager.removeEntity(existedEntities["settingBtn"]);

                player(manager, textureLoader);
                inventory(manager, textureLoader);
                existedEntities["map"] = map(manager, textureLoader);
                SpawnSystemUpdate(manager);
            }
            PlayPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }
    }
    return 0;
}