#pragma once
#include <filesystem>
#include "./component.hpp"

template<typename Enum>
void loadAnimations(
    std::map<Enum, Animation>& animations, 
    std::initializer_list<std::pair<Enum, std::initializer_list<Frame>>> data
) {
    for(auto& [animation, frameList] : data) {
        std::vector<Frame>&  vect = animations[animation];
        for( auto frame : frameList) {
            vect.push_back(frame);
        }
    }
};

template<typename Enum>
void loadAnimations(
    std::map<Enum, AnimationGrid>& animations, 
    std::initializer_list<std::pair<Enum, std::initializer_list<FrameOnGrid>>> data
) {
    for(auto& [animation, frameList] : data) {
        std::vector<FrameOnGrid>&  vect = animations[animation];
        for( auto frame : frameList) {
            vect.push_back(frame);
        }
    }
};