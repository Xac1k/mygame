#pragma once
#include "../utils/entitiesManager.hpp"
#include "../utils/component.hpp"
#include "../../Infrastructure/entityManager.h"
#include "../utils/animationLoader.hpp"

enum class ButtonState { idle, over, pressed, blocked };

void startButton(EntitiesManager& manager, TextureLoader& textureLoader) {
    manager.addEntity();

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

    CallBackComponent<BusEvent&> callBackComponent([](BusEvent& event) { std::cout << "Button Start clicked!\n"; });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);
}

void continueButton(EntitiesManager& manager, TextureLoader& textureLoader) {
    manager.addEntity();

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
}

void settingButton(EntitiesManager& manager, TextureLoader& textureLoader) {
    manager.addEntity();

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

    CallBackComponent<BusEvent&> callBackComponent([](BusEvent& event) { std::cout << "Button Setting clicked!\n"; });
    manager.addComponent<CallBackComponent<BusEvent&>>(callBackComponent);
}