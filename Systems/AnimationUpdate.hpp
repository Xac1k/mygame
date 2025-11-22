#pragma once
#include "../Entities/utils/entitiesManager.hpp"
#include "../Entities/utils/component.hpp"

float getFullTimeLine(
    Animation Animation
) {
    float res = 0;
    for (auto spriteElt : Animation) {
        res += spriteElt.durationTime;
        if(spriteElt.repeatingFlag) return res;
    }
    return MAXFLOAT;
}

void AnimationUpdate(EntitiesManager& manager, float df) {
    manager.with<AnimationComponent>().with<StateComponent>().forEach(
        [df]
        (
            int id, 
            std::shared_ptr<AnimationComponent> animationComponentPtr,
            std::shared_ptr<StateComponent> state
        ){
        auto animationComponent = animationComponentPtr.get();
        animationComponent->time += df;
        auto Animation = animationComponent->animation[state.get()->state];

        if(animationComponent->time > getFullTimeLine(Animation)) {
            animationComponentPtr.get()->time = 0;
        }
    });
}