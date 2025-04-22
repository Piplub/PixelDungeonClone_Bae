﻿#pragma once
#include "config.h"

class Level;
class D2DImage;
class PathFinder;

enum class EntityState
{
    IDLE,
    MOVE,
    ATTACK,
    DEAD
};
enum class EntityType
{
    PLAYER,
    MONSTER,
    NPC,
    OBJECT
};

class Entity
{
protected:
    //test
    D2DImage* image;
    int curAnimFrame;
    float curTime;

    // 공통 속성
    EntityType type;
    EntityState curState;
    FPOINT position;
    FPOINT targetPos;
    float speed;
    bool isMoving;

    // 전투 속성
    int hp, maxHp;
    int attackDmg, defense;
    int exp, level;
    Entity* target;

    //에너지 턴 test
    float energy = 10.f;
    float actionCost;
    float energyPerTurn = 10.0f;

    //길찾기
    vector<FPOINT> path;
    PathFinder* finder;
    FPOINT destPos;
public:
    int graphicID;
    bool isActive;

    wchar_t szText[128]; // 디버깅용
public:
    Entity();
    virtual ~Entity();

    void Update();
    virtual void Render(HDC hdc);
    virtual void Act(Level* level);
    virtual void Attack(Level* level) {};

    virtual bool NeedsInput() = 0;
    virtual bool IsBusy() = 0;
    bool IsAlive() { return curState != EntityState::DEAD; }

    void TakeDamage(int dmg);
    void TakeExp(int exp);
    void LevelUp();

    inline void SetPosition(const float x, const float y) { this->position.x = x; this->position.y = y; }
    inline void SetPosition(FPOINT postion) { this->position.x = postion.x; this->position.y = postion.y; }
    inline FPOINT GetPosition() const { return position; }
    inline int GetAttDmg() const { return attackDmg; }
    inline int GetDefense() const { return defense; }
    inline EntityState GetState() const { return curState; }
    inline float GetSpeed() const { return speed; }
    inline EntityType GetType() const { return type; }
    inline int GetExp() { return exp; }
    inline D2DImage* GetImage() { return image; }

    //에너지 관련 함수
    inline bool CanAct() const { return energy >= actionCost; }
    void AddEnergy() { energy += energyPerTurn; }
    void UseEnergy() { energy -= actionCost; }

    //아이템 관련 함수
    virtual void Heal(int healAmount) {};

    void Stop() { destPos = position; }
};
