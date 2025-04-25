#pragma once
#include "config.h"
#include "Observer/EntityObserverHub.h"

class Level;
class D2DImage;
class PathFinder;
class Animator;

enum class EntityState
{
    IDLE,
    WAIT,
    SLEEP,
    MOVE,
    ATTACK,
    DEAD,
    USING_ITEM
};
enum class EntityType
{
    PLAYER,
    MONSTER,
    NPC,
    OBJECT
};
struct Damage
{
    int min;
    int max;
};

class Entity
{
protected:
    //test
    D2DImage* image;
    int curAnimFrame;

    // animation
    Animator* animator;

    // 공통 속성
    EntityType type;
    EntityState curState;
    FPOINT position;
    FPOINT targetPos;
    float speed;
    bool isMoving;

    // 전투 속성
    int hp, maxHp;
    Damage attackDmg;
    int defense;
    int exp, maxExp, level;
    Entity* target;

    //에너지 턴 test
    float energy = 10.f;
    float actionCost;
    float energyPerTurn;

    //길찾기
    vector<FPOINT> path;
    PathFinder* finder;
    FPOINT destPos;

    //Observer
    EntityObserverHub entityObserver;
public:
    int graphicID;
    bool isActive;

public:
    Entity();
    virtual ~Entity();

    virtual void Update();
    virtual void Render(HDC hdc);
    virtual void Act(Level* level);
    virtual void Attack(Level* level) {};

    virtual bool NeedsInput() = 0;
    virtual bool IsBusy() = 0;
    bool IsAlive() { return isActive; }

    void TakeDamage(int dmg);
    void TakeExp(int exp);
    void LevelUp();

    inline void SetPosition(const float x, const float y) { this->position.x = x; this->position.y = y; }
    inline void SetPosition(FPOINT postion) { this->position.x = postion.x; this->position.y = postion.y; }
    inline FPOINT GetPosition() const { return position; }
    inline Damage GetAttDmg() const { return attackDmg; }
    inline int GetDefense() const { return defense; }
    inline EntityState GetState() const { return curState; }
    inline EntityType GetType() const { return type; }
    inline int GetExp() { return exp; }
    inline int GetMaxExp() { return maxExp; }
    inline int GetLevel() { return level; }
    inline D2DImage* GetImage() { return image; }
    inline int GetCurAnimIdx() { return curAnimFrame; }
    inline int GetHP() const { return hp; };
    inline int GetMaxHP() const { return maxHp; };


    //에너지 관련 함수
    inline bool CanAct() const { return energy >= actionCost; }
    void AddEnergy() { energy += energyPerTurn; }
    void UseEnergy() { energy -= actionCost; }

    //아이템 관련 함수
    virtual void Heal(int healAmount) {};   // HealthPotion

    void Stop() { destPos = position; }
    virtual void SetState(EntityState state);


    // 옵저버
    inline EntityObserverHub& GetEntityObserverHub() { return entityObserver; }

    // AI
    FPOINT GetTargetPos() { return targetPos; }

};
