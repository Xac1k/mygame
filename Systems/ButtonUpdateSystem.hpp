#pragma once
#include "../Entities/utils/entitiesManager.hpp"
#include "../Interfaces/busEvent.h"
#include "../Entities/utils/component.hpp"
#include "../Entities/components/button.hpp"
#include "../Common/CollisionRect.hpp"
#include "../Common/Vect.hpp"

void ButtonUpdate(EntitiesManager& manager, BusEvent& event) {
    auto buttonIDs = manager.with<CallBackComponent<BusEvent&>>().with<PositionComponent>().get();

    switch (event.currEvent)
    {
    case BusEvent::Event::MouseDownLeft:
        for(int id : buttonIDs) {
            auto posComponent = manager.getComponent<PositionComponent>(id).get();
            auto sizeComponent = manager.getComponent<SizeComponent>(id).get();
            auto stateComponent = manager.getComponent<StateComponent>(id);
            if(collisionRect(event.mousePos, posComponent->point, sizeComponent->size)) {
                auto animationComponent = manager.getComponent<AnimationComponent>(id);
                auto callbackComponent = manager.getComponent<CallBackComponent<BusEvent&>>(id);
                callbackComponent.get()->callback(event);
                stateComponent.get()->state = (int) ButtonState::pressed;
            }

        }
        break;
    case BusEvent::Event::MouseMove:
        for(int id : buttonIDs) {
            auto posComponent = manager.getComponent<PositionComponent>(id).get();
            auto sizeComponent = manager.getComponent<SizeComponent>(id).get();
            auto stateComponent = manager.getComponent<StateComponent>(id);
            if(
                collisionRect(event.mousePos, posComponent->point, sizeComponent->size) &&
                stateComponent.get()->state == (int) ButtonState::idle
            ) {
                auto animationComponent = manager.getComponent<AnimationComponent>(id);
                stateComponent.get()->state = (int) ButtonState::over;
                animationComponent.get()->time = 0;
            }
            else if(
                !collisionRect(event.mousePos, posComponent->point, sizeComponent->size) &&
                stateComponent.get()->state == (int) ButtonState::over
            ) {
                stateComponent.get()->state = (int) ButtonState::idle;
            }
        }
        break;

    case BusEvent::Event::MouseUp:
        for(int id : buttonIDs) {
            auto posComponent = manager.getComponent<PositionComponent>(id).get();
            auto sizeComponent = manager.getComponent<SizeComponent>(id).get();
            auto stateComponent = manager.getComponent<StateComponent>(id);
            if(collisionRect(event.mousePos, posComponent->point, sizeComponent->size)) {
                stateComponent.get()->state = (int) ButtonState::over;
            }
        }
        break;
    
    default:
        return;
    }  
};