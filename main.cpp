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
#include <Entities/components/goblin.hpp>
#include <Entities/components/Label.hpp>
#include <Entities/components/map.hpp>

#include <Systems/InventoryContextMenuSystem.hpp>
#include <Systems/InventoryUpdateSystem.hpp>
#include <Systems/InventoryDrawSystem.hpp>
#include <Systems/ButtonUpdateSystem.hpp>
#include <Systems/SpawnBarrelSystem.hpp>
#include <Systems/AnimationUpdate.hpp>
#include <Systems/MapDrawSystem.hpp>
#include <Systems/SpawnSystem.hpp>

#include <Pages/SettingPage.hpp>
#include <Pages/StartPage.hpp>
#include <Pages/PlayPage.hpp>

#define PATH_TO_FRAG_SHADER "Shaders/shader.frag"
#define PATH_TO_VERTEX_SHADER "Shaders/basic.vert"
sf::Shader DrunkShader;
sf::RenderTexture renderTexture;

sf::Vector2f rescaleCoeff({1., 1.});

void init(EntitiesManager& manager, TextureLoader& textureLoader, AudioSystem& audioManager) {
    gameState(manager);

    audioManager.loadMusic("Стартовое меню", "Sounds/asset/Thunder-Unison-Action-Dramatic-Epic-Music-chosic.com_.mp3");
    audioManager.loadMusic("BarrelDeath", "Sounds/asset/BarrelDeath.mp3");
    audioManager.loadMusic("PlayerDamage", "Sounds/asset/PlayerDamage.mp3");
    audioManager.loadMusic("PlayerAttack", "Sounds/asset/PlayerAttack.wav");
    audioManager.loadMusic("item13", "Sounds/asset/items/item13.wav");
    audioManager.loadMusic("item14", "Sounds/asset/items/item14.wav");
    audioManager.loadMusic("item15", "Sounds/asset/items/item15.mp3");
    audioManager.loadMusic("PlayerDeath", "Sounds/asset/PlayerKilled.wav");
    audioManager.loadMusic("SkeletonDeath", "Sounds/asset/SkeletonDeath.wav");
    // audioManager.playMusic("Стартовое меню", true);
    audioManager.setMasterVolume(50);

    if (sf::Shader::isAvailable()) {
        if (!DrunkShader.loadFromFile(buildFullPath(PATH_TO_FRAG_SHADER, 1), sf::Shader::Fragment)) {
            std::cout << "ОШИБКА: Не удалось загрузить шейдер!\n";
            std::cout << "Проверь пути:\n";
            std::cout << "Vertex: " << buildFullPath(PATH_TO_VERTEX_SHADER, 1) << "\n";
            std::cout << "Fragment: " << buildFullPath(PATH_TO_FRAG_SHADER, 1) << "\n";
        } else {
            std::cout << "Шейдер успешно загружен!\n";
            DrunkShader.setUniform("texture", 0);
        }
    } else {
        std::cout << "Шейдеры не поддерживаются на этом железе\n";
    }
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Cave", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
    renderTexture.create(WINDOW_WIDTH + 80, WINDOW_HEIGHT);
    window.setVerticalSyncEnabled(false);

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

    SpawnBarrelSystem SpawnBarrel;
    
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
                SpawnSystemUpdate(manager, textureLoader);
                SpawnBarrel.update(manager, textureLoader);
            }
            PlayPage(clock, window, busEvent, manager, audioManager, animator, textureLoader, renderer);
        }
    }
    return 0;
}

//TODO:  Добавить сути (не понятно, что нужно сделать)
// Сделать больше экран.