#pragma once
#include "../../Common/Vect.hpp"
#include <functional>
#include <string>
#include <map>
#include <variant>
#include "../../Map/Preprocessing/map.hpp"

struct PositionOnMapComponent {
    Vect2D point;

    PositionOnMapComponent(float x, float y): point({x, y}) {};
    PositionOnMapComponent(Vect2D pointI): point(pointI) {};
    PositionOnMapComponent(): point({0, 0}) {};
    ~PositionOnMapComponent() = default;
};

struct  PositionComponent
{
    Vect2D point;

    PositionComponent(float x, float y): point({x, y}) {};
    PositionComponent(Vect2D pointI): point(pointI) {};
    PositionComponent(): point({0, 0}) {};
    ~PositionComponent() = default;
};

struct  PlayerPosComponent
{
    Vect2D point;

    PlayerPosComponent(float x, float y): point({x, y}) {};
    PlayerPosComponent(Vect2D pointI): point(pointI) {};
    PlayerPosComponent(): point({0, 0}) {};
    ~PlayerPosComponent() = default;
};

struct SizeComponent {
    Vect2D size;

    SizeComponent(float x, float y): size({x, y}) {};
    SizeComponent(Vect2D sizeI): size(sizeI) {};
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

struct FrameOnGrid {
    std::string path;
    Vect2D cellID;
    float durationTime;
    bool repeatingFlag;
    bool turn = false;
    Vect2D mirror; //(1, 1) (-1, -1) (-1, 1) (1, -1)

    FrameOnGrid(std::string pathI, Vect2D cellIDI, float time, bool repeat, Vect2D mirrorI): 
        path(pathI), cellID(cellIDI), durationTime(time), repeatingFlag(repeat), mirror(mirrorI) {};
    FrameOnGrid(std::string pathI, Vect2D cellIDI, float time, bool repeat): 
        path(pathI), cellID(cellIDI), durationTime(time), repeatingFlag(repeat), mirror(1, 1) {};
    FrameOnGrid(): 
        cellID(0, 0), durationTime(0), repeatingFlag(0) {};
    ~FrameOnGrid() = default;
};

using Animation = std::vector<Frame>;
using AnimationGrid = std::vector<FrameOnGrid>;

enum class DirectionFrame { forward, backward };
struct AnimationGridComponent {
    Vect2D TileSizeInGrid;
    DirectionFrame dir = DirectionFrame::forward;
    float time = 0;
    std::map<int, AnimationGrid> animation;
};

struct AnimationComponent {
    float time = 0;
    std::map<int, Animation> animation;
};

struct StateComponent {
    int state;

    StateComponent(int state): state(state) {};
    StateComponent(): state(0) {};
};

enum class GameScreen {start, play, exchange, inventory, setting, none};
struct GameStateComponent {
    GameScreen screen;
    int volume = 5;

    GameStateComponent(): screen(GameScreen::start) {};
};

struct MiniMapComponent {
    miniMapRenderer miniMap;
};

struct MapComponent {
    GameMap map;
    Vect2D cameraPos;
    Vect2D visiableArea;
    int TileSize;
    TileRenderer tileRender;

    MapComponent(): map(GameMap ()), cameraPos(0, 0), TileSize(64), tileRender(TileRenderer ())  {};
};

struct CollisionComponent {
    Vect2D size;
    Vect2D shiftFromLeftUp;

    CollisionComponent(): size(0, 0), shiftFromLeftUp(0, 0) {};
    CollisionComponent(Vect2D sizeI, Vect2D shiftFromCenterI): size(sizeI), shiftFromLeftUp(shiftFromCenterI) {};
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
    bool isSetContextMenu = false; //отображается или нет контекстное меню предмета
    bool isSelected = false;

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

    HealthComponent(int healthI): health(healthI) {};
};

struct AttackComponent {
    int damage;

    AttackComponent(): damage(0) {};
};


template<typename Enum>
struct MutexComponent {
    bool blocked;
    float currTime;
    float durationTime;
    Enum WhoIsUsing;

    MutexComponent(): blocked(false), currTime(0), durationTime(0) {};
};