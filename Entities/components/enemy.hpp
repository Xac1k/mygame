// #pragma once
// #include "../utils/entitiesManager.hpp"
// #include "../utils/component.hpp"
// #include "../../Infrastructure/entityManager.h"
// #include "../utils/animationLoader.hpp"
// #include "../../main.h"

// enum class EnemyState { 
//     IdleDirect, IdleBackward, IdleLeft, 
//     IdleRight, WalkDirect, WalkBackward, 
//     WalkLeft, WalkRight, HurtDirect, 
//     HurtBackward, HurtLeft, HurtRight };

// enum class ControlFlow {
//     HurtSystem, 

//     All
// };

// void player(EntitiesManager& manager, TextureLoader& textureLoader) {
//     manager.addEntity();

//     PlayerPosComponent pos(0, 0);
//     manager.addComponent<PlayerPosComponent>(pos);

//     PositionComponent posMap(WINDOW_WIDTH/2 - 128/2, WINDOW_HEIGHT/2 - 128/2);
//     manager.addComponent<PositionComponent>(posMap);

//     SizeComponent size(128, 128);
//     manager.addComponent<SizeComponent>(size);

//     VelocityComponent velo;
//     manager.addComponent<VelocityComponent>(velo);

//     HealthComponent health(100);
//     manager.addComponent<HealthComponent>(health);

//     StateComponent state((int) PlayerState::IdleDirect);
//     manager.addComponent<StateComponent>(state);

//     CollisionComponent rect({32, 22}, {46, 79});
//     manager.addComponent<CollisionComponent>(rect);

//     MutexComponent<ControlFlow> mutex;
//     manager.addComponent<MutexComponent<ControlFlow>>(mutex);

//     AnimationGridComponent animationComponent;
//     animationComponent.TileSizeInGrid = {64, 64};
//     loadAnimations(animationComponent.animation, 
//         {
//             {(int)PlayerState::IdleDirect, {
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {4, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {5, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {6, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {7, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {8, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {9, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {10, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {11, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {11, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {10, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {9, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {8, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {7, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {6, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {5, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {4, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 0}, 0.125f, true},
//             }},
//             {(int)PlayerState::IdleBackward, {
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 3}, 0.125f, true},
//             }},
//             {(int)PlayerState::IdleLeft, {
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {4, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {5, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {6, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {7, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {8, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {9, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {10, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {11, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {11, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {10, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {9, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {8, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {7, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {6, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {5, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {4, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 1}, 0.125f, true},
//             }},
//             {(int)PlayerState::IdleRight, {
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {4, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {5, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {6, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {7, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {8, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {9, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {10, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {11, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {11, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {10, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {9, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {8, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {7, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {6, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {5, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {4, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {3, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {2, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {1, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_idle.png", {0, 2}, 0.125f, true},
//             }},
//             {(int)PlayerState::WalkDirect,{
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {0, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {1, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {2, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {3, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {4, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {5, 0}, 0.125f, true},
//             }},
//             {(int)PlayerState::WalkBackward, {
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {0, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {1, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {2, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {3, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {4, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {5, 3}, 0.125f, true},
//             }},
//             {(int)PlayerState::WalkLeft, {
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {0, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {1, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {2, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {3, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {4, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {5, 1}, 0.125f, true},
//             }},
//             {(int)PlayerState::WalkRight, {
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {0, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {1, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {2, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {3, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {4, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_walk.png", {5, 2}, 0.125f, true},
//             }},
//             {(int)PlayerState::HurtDirect, {
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {0, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {1, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {2, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {3, 0}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {4, 0}, 0.125f, false},
//             }},
//             {(int)PlayerState::HurtLeft, {
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {0, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {1, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {2, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {3, 1}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {4, 1}, 0.125f, false},
//             }},
//             {(int)PlayerState::HurtRight, {
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {0, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {1, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {2, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {3, 2}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {4, 2}, 0.125f, false},
//             }},
//             {(int)PlayerState::HurtBackward, {
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {0, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {1, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {2, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {3, 3}, 0.125f, false},
//                 {"Store/view/Player/Swordsman_lvl1_Hurt.png", {4, 3}, 0.125f, false},
//             }},
//         }
//     );
//     manager.addComponent<AnimationGridComponent>(animationComponent);
// }