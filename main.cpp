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
#include <Entities/components/Label.hpp>
#include <Entities/components/map.hpp>

#include <Systems/InventoryContextMenuSystem.hpp>
#include <Systems/InventoryUpdateSystem.hpp>
#include <Systems/MovementPlayerSystem.hpp>
#include <Systems/InventoryDrawSystem.hpp>
#include <Systems/ButtonUpdateSystem.hpp>
#include <Systems/SpawnBarrelSystem.hpp>
#include <Systems/HurtPlayerSystem.hpp>
#include <Systems/AnimationUpdate.hpp>
#include <Systems/MapDrawSystem.hpp>
#include <Systems/SpawnSystem.hpp>

#include <Pages/SettingPage.hpp>
#include <Pages/StartPage.hpp>
#include <Pages/PlayPage.hpp>

//TODO: Взаимодействия с бочкой

void init(EntitiesManager& manager, TextureLoader& textureLoader, AudioSystem& audioManager) {
    gameState(manager);

    audioManager.loadMusic("Стартовое меню", "Sounds/asset/Дымок.mp3");
    audioManager.loadMusic("BarrelDeath", "Sounds/asset/BarrelDeath.mp3");
    audioManager.loadMusic("PlayerDamage", "Sounds/asset/PlayerDamage.mp3");
    audioManager.loadMusic("PlayerAttack", "Sounds/asset/PlayerAttack.wav");
    audioManager.loadMusic("item13", "Sounds/asset/items/item13.wav");
    //audioManager.playMusic("Стартовое меню", true);
    audioManager.setMasterVolume(1000);
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
    
    while(window.isOpen())
    {
        if(gameState->screen == GameScreen::start) {
            if(prevGameState != gameState->screen) {
                prevGameState = gameState->screen;
                manager.removeEntityByClass("settingMenu*");
                background(manager, textureLoader, "startMenu:background");
                startButton(manager, textureLoader, "startMenu:button:start");
                continueButton(manager, textureLoader, "startMenu:button:continue");
                settingButton(manager, textureLoader, "startMenu:button:setting");
            }
            StartPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }

        if(gameState->screen == GameScreen::setting) {
            if(prevGameState != gameState->screen) {
                prevGameState = gameState->screen;
                manager.removeEntityByClass("startMenu:button*");
                backgroundSetting(manager, textureLoader, "settingMenu:background");
                plusButton(manager, textureLoader, audioManager, "settingMenu:button:plus");
                minusButton(manager, textureLoader, audioManager, "settingMenu:button:minus");
                optionLable(manager, textureLoader, "settingMenu:lable:option");
                volumeLable(manager, textureLoader, "settingMenu:button:volume");
            }
            SettingPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }

        if(gameState->screen == GameScreen::play) {
            if(prevGameState != gameState->screen) {
                prevGameState = gameState->screen;
                manager.removeEntityByClass("startMenu:*");
                player(manager, textureLoader);
                inventory(manager, textureLoader);
                map(manager, textureLoader);
                SpawnSystemUpdate(manager);
                SpawnBarrelSystemUpdate(manager, textureLoader);
            }
            PlayPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }
    }
    return 0;
}