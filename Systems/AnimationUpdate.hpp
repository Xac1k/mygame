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


float getFullTimeLine(
    AnimationGrid Animation
) {
    float res = 0;
    for (auto spriteElt : Animation) {
        res += spriteElt.durationTime;
        if(spriteElt.repeatingFlag) return res;
    }
    return MAXFLOAT;
}

float getFullTimeLineForDelete(
    OverlayAnimationComponent &overlay
) {
    float res = 0;
    for (auto spriteElt : overlay.overlayAnim) {
        res += spriteElt.durationTime;
        if(spriteElt.deleteAfterPlay) return res;
    }
    return MAXFLOAT;
}

class Animator {
    public:
        void AnimationUpdate(EntitiesManager& manager, float df) {
            auto entityAnimIDs = manager.with<AnimationComponent>().with<StateComponent>().get();
            for(int id : entityAnimIDs) {
                auto animComp = manager.getComponent<AnimationComponent>(id).get();
                auto stateComp = manager.getComponent<StateComponent>(id).get();

                if(entityStatus[id] != stateComp->state) animComp->time = 0; // Обновление таймера если state поменялся
                entityStatus[id] = stateComp->state; // Обновление предыдущего состояния

                animComp->time += df;
                auto anim = animComp->animation[stateComp->state];

                if (animComp->time >= getFullTimeLine(anim)) {
                    animComp->time = 0.0f;
                }
            }

            auto entityGridAnimIDs = manager.with<AnimationGridComponent>().with<StateComponent>().get();
            for(int id : entityGridAnimIDs) {
                auto animComp = manager.getComponent<AnimationGridComponent>(id).get();
                auto stateComp = manager.getComponent<StateComponent>(id).get();

                if(entityStatus[id] != stateComp->state) animComp->time = 0; // Обновление таймера если state поменялся
                entityStatus[id] = stateComp->state; // Обновление предыдущего состояния

                animComp->time += df;
                auto anim = animComp->animation[stateComp->state];

                if (animComp->time >= getFullTimeLine(anim)) {
                    animComp->time = 0.0f;
                }
            }

            auto entityOveralyAnimIDs = manager.with<OverlayesStorageComponent>().get();
            for(int id : entityOveralyAnimIDs) {
                auto anims = manager.getComponent<OverlayesStorageComponent>(id).get();
                int overlayID = 0;

                for(auto& overlay : anims->overlayes) {
                    overlay.overlayTime += df;

                    if (overlay.overlayTime >= getFullTimeLine(overlay.overlayAnim)) {
                        overlay.overlayTime = 0.0f;
                    }

                    if(overlay.overlayTime >= getFullTimeLineForDelete(overlay)) {
                        anims->overlayes.erase(anims->overlayes.begin() + overlayID);
                        std::cout << "Удаление слоя на " << id << " id слоя " << overlayID << std::endl;
                    }
                    overlayID++;
                }
            }
        }

    private:
        std::map<int, int> entityStatus;
};

