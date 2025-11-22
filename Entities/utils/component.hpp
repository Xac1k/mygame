#pragma once
#include "../../Common/Vect.hpp"
#include <functional>
#include <string>
#include <map>

struct  PositionComponent
{
    Vect2D point;

    PositionComponent(float x, float y): point({x, y}) {};
    PositionComponent(): point({0, 0}) {};
    ~PositionComponent() = default;
};

struct SizeComponent {
    Vect2D size;

    SizeComponent(float x, float y): size({x, y}) {};
    SizeComponent(): size({0, 0}) {};
    ~SizeComponent() = default;
};

struct VelocityComponent {
    Vect2D dir;

    VelocityComponent(float x, float y): dir({x, y}) {};
    VelocityComponent(): dir({0, 0}) {};
    ~VelocityComponent() = default;
};

template<typename ...T>
struct CallBackComponent {
    std::function<void(T...)> callback;

    CallBackComponent(std::function<void(T...)> callbackI): callback(callbackI) {};
};

struct Frame {
    std::string path;
    float durationTime;
    bool repeatingFlag;

    Frame(std::string pathI, float time, bool repeat): path(pathI), durationTime(time), repeatingFlag(repeat) {};
    Frame(): path(""), durationTime(0), repeatingFlag(0) {};
    ~Frame() = default;
};
using Animation = std::vector<Frame>;

struct AnimationComponent {
    float time = 0;
    std::map<int, Animation> animation;
};

struct StateComponent {
    int state;

    StateComponent(int state): state(state) {};
    StateComponent(): state(0) {};
};

enum class GameScreen {start, play, exchange, inventory};
struct GameStateComponent {
    GameScreen screen;
    GameStateComponent(): screen(GameScreen::start) {};
};

enum class Items {
    none,
    startSword, startSpear, startPickaxe, 
    ironSword, ironSpear, ironPickaxe,
    goldenSword, goldenSpear, goldenPickaxe,
    fireSword, fireSpear, firePickaxe
};
struct InventoryComponent {
    std::vector<std::vector<Items>> inventory;
    Vect2D size; //кол-во столбцов и строк
    Vect2D selection; //выделенная клетка

    InventoryComponent(Vect2D size, Vect2D selection): size(size), selection(selection) {
        for (int y = 0; y < size.y; y++) {
            inventory.push_back({});
            for (int x = 0; x < size.x; x++) {
                inventory[y].push_back(Items::none);
            }
        }  
        
    }
};

struct DragAndDropComponent {
    bool isStarted;
    bool isMoving;
    Vect2D pos;

    DragAndDropComponent(): isStarted(0), isMoving(0), pos(Vect2D(0, 0)) {}; 
};

struct HealthComponent {
    int health;
};