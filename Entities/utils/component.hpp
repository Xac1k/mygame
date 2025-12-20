#pragma once
#include "../../Common/Vect.hpp"
#include <functional>
#include <string>
#include <map>
#include <variant>
#include <set>
#include "../../Map/Preprocessing/map.hpp"

struct PositionOnMapComponent {
    Vect2D point;

    PositionOnMapComponent(float x, float y): point({x, y}) {};
    PositionOnMapComponent(Vect2D pointI): point(pointI) {};
    PositionOnMapComponent(): point({0, 0}) {};
    ~PositionOnMapComponent() = default;
};

struct  PositionComponent {
    Vect2D point;

    PositionComponent(float x, float y): point({x, y}) {};
    PositionComponent(Vect2D pointI): point(pointI) {};
    PositionComponent(): point({0, 0}) {};
    ~PositionComponent() = default;
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
    Vect2D mirror;
    float alpha = 100;
    bool deleteAfterPlay = false;


    FrameOnGrid(std::string pathI, Vect2D cellIDI, float time, bool repeat, Vect2D mirrorI, float alphaI, bool deleteAfterPlayI): 
        path(pathI), cellID(cellIDI), durationTime(time), repeatingFlag(repeat), mirror(mirrorI), alpha(alphaI), deleteAfterPlay{deleteAfterPlayI} {};
    FrameOnGrid(std::string pathI, Vect2D cellIDI, float time, bool repeat, Vect2D mirrorI, float alphaI): 
        path(pathI), cellID(cellIDI), durationTime(time), repeatingFlag(repeat), mirror(mirrorI), alpha(alphaI) {};
    FrameOnGrid(std::string pathI, Vect2D cellIDI, float time, bool repeat, Vect2D mirrorI): 
        path(pathI), cellID(cellIDI), durationTime(time), repeatingFlag(repeat), mirror(mirrorI) {};
    FrameOnGrid(std::string pathI, Vect2D cellIDI, float time, bool repeat): 
        path(pathI), cellID(cellIDI), durationTime(time), repeatingFlag(repeat), mirror(1, 1) {};
    FrameOnGrid(std::string pathI, Vect2D cellIDI): 
        path(pathI), cellID(cellIDI), mirror(1, 1) {};
    FrameOnGrid(): 
        cellID(0, 0), durationTime(0), repeatingFlag(0) {};
    ~FrameOnGrid() = default;
};

using Animation = std::vector<Frame>;
using AnimationGrid = std::vector<FrameOnGrid>;

//enum class DirectionFrame { forward, backward };
struct AnimationGridComponent {
    Vect2D TileSizeInGrid;
    //DirectionFrame dir = DirectionFrame::forward;
    float time = 0;
    std::map<int, AnimationGrid> animation;
};

struct AnimationComponent {
    float time = 0;
    std::map<int, Animation> animation;
};

struct OverlayAnimationComponent {
    Vect2D TileSizeInGrid;
    float overlayTime = 0.0f;
    AnimationGrid overlayAnim;
    Vect2D size;

    int priority = 1;
};

struct OverlayesStorageComponent {
    std::vector<OverlayAnimationComponent> overlayes;
};

struct OriginComponent {
    Vect2D shift;

    OriginComponent(float x, float y): shift({x, y}) {};
    OriginComponent(Vect2D shiftI): shift(shiftI) {};
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
    CollisionComponent(Vect2D sizeI, Vect2D shiftFromLeftUpI): size(sizeI), shiftFromLeftUp(shiftFromLeftUpI) {};
};

enum class Items {
    none,
    startSword, startSpear, startPickaxe, 
    ironSword, ironSpear, ironPickaxe,
    goldenSword, goldenSpear, goldenPickaxe,
    fireSword, fireSpear, firePickaxe,
    coin, skull, bone
};
struct InventoryComponent {
    std::vector<std::vector<Items>> inventory;
    std::vector<std::vector<int>> countItems;
    Vect2D size; //кол-во столбцов и строк
    Vect2D selection; //выделенная клетка
    bool isSetContextMenu = false; //отображается или нет контекстное меню предмета
    bool isSelected = false;

    InventoryComponent(Vect2D size, Vect2D selection): size(size), selection(selection) {
        for (int y = 0; y < size.y; y++) {
            inventory.push_back({});
            countItems.push_back({});
            for (int x = 0; x < size.x; x++) {
                inventory[y].push_back(Items::none);
                countItems[y].push_back(0);
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
    int maxHealth;

    HealthComponent(int healthI): health(healthI), maxHealth(100) {};
    HealthComponent(int healthI, int maxHealth): health(healthI), maxHealth(maxHealth) {};
};



enum class Effects {none, fire, wet, frozen, poisoned};
struct WeaponComponent {
    int damage; // Урон оружия
    float atackLength; // Растояние атаки
    float deviation; // Градус отклонения от угла атаки
    float cooldownMoving;
    float cooldown;

    Effects effect = Effects::none;
    float duration = 0;
    float period = 0;
    int effectDamage = 0;

    WeaponComponent(int damageI, float atackLengthI, float deviationI, float cooldownI, float cooldownMovingI): 
    damage(damageI), atackLength(atackLengthI), deviation(deviationI), cooldown(cooldownI), cooldownMoving(cooldownMovingI) {};

    void setEffect(Effects effectI, float durationI, float periodI, int effectDamageI) {
        effect = effectI;
        duration = durationI;
        period = periodI; 
        effectDamage = effectDamageI;
    }

    bool haveEffect() {
        return effect != Effects::none;
    }
};

// Персонаж атакует кого-то
struct AttackComponent {
    int damage;
    Vect2D attackerPos; // Позиция атакующего
    Vect2D attackArea; // [x, y] x - начальный угол, y - конечный угол. Диапозон из которого мы берем entity 
    float attackLen; // Растояние между атакующим и атакованным

    Effects effect = Effects::none; // Нужно ли накладывать эффект или нет?
    float period = 0; // Как часто будет срабатывать эффект
    float duration = 0; // Как долгот будет длиться эффект
    float effectDamage = 0; // Урона за раз


    Vect2D repulsionVel; //Если нужно кого-то оттолкнуть(скорость)

    AttackComponent(
        WeaponComponent* weapon,
        Vect2D AttackerPosI, 
        Vect2D attackAreaI
    ): damage(weapon->damage), attackerPos(AttackerPosI), attackArea(attackAreaI), attackLen(weapon->atackLength) {};


    void moveEffect(WeaponComponent* weapon) {
        effect = weapon->effect;
        duration = weapon->duration;
        period = weapon->period; 
        effectDamage = weapon->effectDamage;
    }

    bool haveEffect() {
        return effect != Effects::none;
    }
};

struct EffectComponent {
    float duration = 0; // how much in Seconds
    float currentTime = 0; // what is now in Seconds
    float period = 0; // how often in Seconds

    int count = 0; // how much was

    int damage = 0; // damage per one time
    Effects effect;

    void moveEffect(AttackComponent* weapon) {
        effect = weapon->effect;
        duration = weapon->duration;
        period = weapon->period; 
        damage = weapon->effectDamage;
    }

    bool operator==(const EffectComponent& other) {
        return effect == other.effect;
    };
};

struct EffectsComponent {
    std::vector<EffectComponent> effects;

    std::vector<EffectComponent>::iterator find(EffectComponent effect) {
        int j = 0;
        for(int i = 0; i < effects.size(); i++) {
            if(effect == effects[i]) break;
            j++;
        }

        if(effects.begin() + j == effects.end()) return effects.end();
        return effects.begin() + j;
    }

    std::vector<EffectComponent>::iterator find(Effects effect) {
        int j = 0;
        for(int i = 0; i < effects.size(); i++) {
            if(effect == effects[i].effect) break;
            j++;
        }

        if(effects.begin() + j == effects.end()) return effects.end();
        return effects.begin() + j;
    }

    void addEffect(AttackComponent* weapon) {
        EffectComponent effect;
        effect.moveEffect(weapon);
        auto it = find(effect);
        if(it != effects.end()) return;
        effects.push_back(effect);
    }

    void removeEffect(EffectComponent effect) {
        auto it = find(effect);
        if(it == effects.end()) return;
        effects.erase(it);
    }

    void removeEffect(Effects effect) {
        auto it = find(effect);
        if(it == effects.end()) return;
        effects.erase(it);
    }
};

struct EffectsInfo {
    float distExpandFire;
    float distExpandWet;
    
    EffectsInfo(float distFire, float distWet): distExpandWet(distWet), distExpandFire(distFire) {};
};

struct CanFire{};
struct CanFrozen{};
struct CanWet{};
struct CanPoisoned{};




struct LootDrop {
    Items itemID;        
    float chance;  
    int minCount;
    int maxCount;
    float offsetRadius;  

    LootDrop( Items itemIDI, float chanceI = 1.0f, 
        int minCountI = 1, int maxCountI = 1, 
        float offsetRadiusI = 30.0f
    ): itemID(itemIDI), chance(chanceI),
    minCount(minCountI), maxCount(maxCountI), 
    offsetRadius(offsetRadiusI) {};
};

struct LootTableComponent {
    std::vector<LootDrop> drops;

    LootTableComponent(): drops({}) {};
    LootTableComponent(std::vector<LootDrop> lootTableI): drops(lootTableI) {};
};

struct PhysicsComponent {
    Vect2D velo;

    PhysicsComponent(float x, float y): velo(Vect2D(x, y)) {};
};

struct PickUpItemComponent {
    int count = 0;
    float pickUpRadius;

    PickUpItemComponent(int countI, float pickUpRadiusI): count(countI), pickUpRadius(pickUpRadiusI) {};
    PickUpItemComponent() {};
};

template<typename Enum>
struct MutexComponent {
    bool blocked;
    float currTime;
    float durationTime;
    Enum WhoIsUsing;

    MutexComponent(): blocked(false), currTime(0), durationTime(0) {};
};

struct DeathComponent {
    bool isDead = false;
    float deathTime = 0.0f;
    float fadeOutTime = 2.0f;
    std::string soudnFXname;
    Vect2D angleOfDeath = 0;

    DeathComponent(float timeLimit, std::string soundFX, float angleOfDeathI): fadeOutTime(timeLimit), soudnFXname(soundFX), angleOfDeath(angleOfDeathI) {}
    DeathComponent(std::string soundFX): soudnFXname(soundFX) {};
};

struct SoundFX {
    std::string nameSoundFX;        
    float chance;  

    SoundFX(std::string name, float chanceI): nameSoundFX(name), chance(chanceI) {};
};

enum class Action {Hit, Death, Step, Attack};
struct SoundFXComponent {
    Action act;
    std::vector<SoundFX> sounds;

    SoundFXComponent(Action actI): act(actI), sounds({}) {};
    void loadSound(std::string name, float chance) {
        SoundFX sound(name, chance);
        sounds.push_back(sound);
    }
};

enum class Facing {Backward, Direct, Left, Rigth};
struct DirectionalDeathComponent {
    Facing facing = Facing::Direct;

    DirectionalDeathComponent(Facing facingI): facing(facingI) {};
    DirectionalDeathComponent() = default;
};

struct ChunkLoaderComponent {
    int countChunk;

    ChunkLoaderComponent(int countChunkI): countChunk(countChunkI) {};
};

enum class AIState {
    Wandering,
    Chasing,
    Attacking
};
enum class HowToFindTarget {
    ByClassName,
    ByID
};
struct AIAgentCompanent {
    AIState state = AIState::Wandering;
    HowToFindTarget mode = HowToFindTarget::ByID;
    int targetEntityID = -1;
    std::string classNameOfTarget;

    float detectionRadius = 10.0f;
    float attackRadius = 1.5f;
    float wanderRadius = 5.0f;

    float speedOfWalk;
    float speedOfChasing;

    AIAgentCompanent(HowToFindTarget modeI, float detectionRadiusI, float wanderRadiusI, float attackRadiusI, std::string className, float speedOfWalkI, float speedOfChasingI): 
    detectionRadius(detectionRadiusI), wanderRadius(wanderRadiusI), attackRadius(attackRadiusI), 
    classNameOfTarget(className), mode(modeI), speedOfWalk(speedOfWalkI), speedOfChasing(speedOfChasingI) {};

    AIAgentCompanent(HowToFindTarget modeI, float detectionRadiusI, float wanderRadiusI, float attackRadiusI, int ID, float speedOfWalkI, float speedOfChasingI): 
    detectionRadius(detectionRadiusI), wanderRadius(wanderRadiusI), attackRadius(attackRadiusI), targetEntityID(ID), mode(modeI), speedOfWalk(speedOfWalkI),
    speedOfChasing(speedOfChasingI) {};
};

struct StuneCompanent {
    float duration;
    float currentTime = 0;

    StuneCompanent(float duration): duration(duration) {};
};

struct AttackCooldownCompanent {
    float duration;
    float currentTime = 0;

    AttackCooldownCompanent(float duration): duration(duration) {};
};

struct ReadyToAttack{};

struct ColldownBeforeAttackComp {
    float duration;
    float currentTime = 0;

    ColldownBeforeAttackComp(float duration): duration(duration) {};
};

struct WanderPointCompanent {
    Vect2D point;
    float cooldown = 0.0f;
    float currTime = 0.f;
};

struct CooldownInfo {
    float cooldownMoving;
    float cooldownAttack;
    float cooldownBeforeAttack;

    CooldownInfo(float cooldownMoving, float cooldownAttack, float cooldownBeforeAttack):
    cooldownMoving(cooldownMoving), cooldownAttack(cooldownAttack), cooldownBeforeAttack(cooldownBeforeAttack)
    {};
};

struct HurtComponent {
    float angle; //Угол принятого урона (угол от damager до target) 

    HurtComponent(float angleI): angle(angleI) {};
};

