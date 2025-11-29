#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"

enum class ButtonState { idle, over, pressed, blocked };

int startButton(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(287, 93);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(126, 70);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) ButtonState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)ButtonState::idle, {
                {"Store/view/ButtonStart/ButtonStartIdle.png", 1.f, false},
            }},

            {(int)ButtonState::over, {
                {"Store/view/ButtonStart/ButtonStartHover1.png", 0.5f, false},
                {"Store/view/ButtonStart/ButtonStartHover2.png", 0.5f, false},
                {"Store/view/ButtonStart/ButtonStartHover3.png", 0.5f, false},
                {"Store/view/ButtonStart/ButtonStartHover4.png", 0.5f, false},
            }},

            {(int)ButtonState::blocked, {
                {"Store/view/ButtonStart/ButtonStartBlocked.png", 1.f, false},
            }},

            {(int)ButtonState::pressed, {
                {"Store/view/ButtonStart/ButtonStartPressed.png", 1.f, false},
            }}
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);

    CallBackComponent<BusEvent&> callBackComponent([&manager](BusEvent& event) { 
        auto gameStateIDs = manager.with<GameStateComponent>().get();
        if(gameStateIDs.size() == 0) return;
        auto gameState = manager.getComponent<GameStateComponent>(gameStateIDs[0]).get();
        gameState->screen = GameScreen::play;
        std::cout << "Button Start clicked!\n"; 
    });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);

    return manager.getID();
}

int continueButton(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(287, 167);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(126, 70);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) ButtonState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)ButtonState::idle, {
                {"Store/view/ButtonContinue/ButtonContinueIdle.png", 1.f, false},
            }},

            {(int)ButtonState::over, {
                {"Store/view/ButtonContinue/ButtonContinueHover1.png", 0.5f, false},
                {"Store/view/ButtonContinue/ButtonContinueHover2.png", 0.5f, false},
                {"Store/view/ButtonContinue/ButtonContinueHover3.png", 0.5f, false},
                {"Store/view/ButtonContinue/ButtonContinueHover4.png", 0.5f, false},
            }},

            {(int)ButtonState::blocked, {
                {"Store/view/ButtonContinue/ButtonContinueBlocked.png", 1.f, false},
            }},

            {(int)ButtonState::pressed, {
                {"Store/view/ButtonContinue/ButtonContinuePressed.png", 1.f, false},
            }}
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);

    CallBackComponent<BusEvent&> callBackComponent([](BusEvent& event) { std::cout << "Button Continue clicked!\n"; });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);

    return manager.getID();
}

int settingButton(EntitiesManager& manager, TextureLoader& textureLoader, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(287, 237);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(126, 70);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) ButtonState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)ButtonState::idle, {
                {"Store/view/ButtonSetting/ButtonSettingIdle.png", 1.f, false},
            }},

            {(int)ButtonState::over, {
                {"Store/view/ButtonSetting/ButtonSettingHover1.png", 0.5f, false},
                {"Store/view/ButtonSetting/ButtonSettingHover2.png", 0.5f, false},
                {"Store/view/ButtonSetting/ButtonSettingHover3.png", 0.5f, false},
                {"Store/view/ButtonSetting/ButtonSettingHover4.png", 0.5f, false},
            }},

            {(int)ButtonState::blocked, {
                {"Store/view/ButtonSetting/ButtonSettingBlocked.png", 1.f, false},
            }},

            {(int)ButtonState::pressed, {
                {"Store/view/ButtonSetting/ButtonSettingPressed.png", 1.f, false},
            }}
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);

    CallBackComponent<BusEvent&> callBackComponent([&manager](BusEvent& event) { 
        auto gameStateIDs = manager.with<GameStateComponent>().get();
        if(gameStateIDs.size() == 0) return;
        auto gameState = manager.getComponent<GameStateComponent>(gameStateIDs[0]).get();
        gameState->screen = GameScreen::setting;
        std::cout << "Button Setting clicked!\n"; 
    });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);

    return manager.getID();
}

int plusButton(EntitiesManager& manager, TextureLoader& textureLoader, AudioSystem& audioManager, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(455, 186);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(32, 32);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) ButtonState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)ButtonState::idle, {
                {"Store/view/Lable/BtnPlus.png", 1.f, false},
            }},
            {(int)ButtonState::over, {
                {"Store/view/Lable/BtnPlus.png", 1.f, false},
            }},
            {(int)ButtonState::blocked, {
                {"Store/view/Lable/BtnPlus.png", 1.f, false},
            }},
            {(int)ButtonState::pressed, {
                {"Store/view/Lable/BtnPlus.png", 1.f, false},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);

    CallBackComponent<BusEvent&> callBackComponent([&manager, &audioManager](BusEvent& event) { 
        auto gameStateIDs = manager.with<GameStateComponent>().get();
        if(gameStateIDs.size() == 0) return;
        auto gameState = manager.getComponent<GameStateComponent>(gameStateIDs[0]).get();
        gameState->volume += 1;
        if(gameState->volume > 10) gameState->volume = 10;
        audioManager.setMasterVolume(gameState->volume*10);
        std::cout << "Button Plus clicked!\n"; 
    });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);

    return manager.getID();
}

int minusButton(EntitiesManager& manager, TextureLoader& textureLoader, AudioSystem& audioManager, std::string className) {
    manager.addEntity(className);

    PositionComponent pos(213, 186);
    manager.addComponent<PositionComponent>(pos);

    SizeComponent size(32, 32);
    manager.addComponent<SizeComponent>(size);

    StateComponent state((int) ButtonState::idle);
    manager.addComponent<StateComponent>(state);

    AnimationComponent animationComponent;
    loadAnimations(animationComponent.animation, 
        {
            {(int)ButtonState::idle, {
                {"Store/view/Lable/BtnMinus.png", 1.f, false},
            }},
            {(int)ButtonState::over, {
                {"Store/view/Lable/BtnMinus.png", 1.f, false},
            }},
            {(int)ButtonState::pressed, {
                {"Store/view/Lable/BtnMinus.png", 1.f, false},
            }},
            {(int)ButtonState::blocked, {
                {"Store/view/Lable/BtnMinus.png", 1.f, false},
            }},
        }
    );
    manager.addComponent<AnimationComponent>(animationComponent);

    CallBackComponent<BusEvent&> callBackComponent([&manager, &audioManager](BusEvent& event) { 
        auto gameStateIDs = manager.with<GameStateComponent>().get();
        if(gameStateIDs.size() == 0) return;
        auto gameState = manager.getComponent<GameStateComponent>(gameStateIDs[0]).get();
        gameState->volume -= 1;
        if(gameState->volume < 0) gameState->volume = 0;
        audioManager.setMasterVolume(gameState->volume*10);
        std::cout << "Button Minus clicked!\n"; 
    });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);

    return manager.getID();
}