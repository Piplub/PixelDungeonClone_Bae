#pragma once
#include "config.h"
#include "Entity.h"

//enum class EntityState
//{
//    IDLE,
//    MOVE,
//    ATTACK,
//    DEAD
//};

class MonsterAI;
class Level;
class Monster : public Entity
{
public:
    Monster();
    virtual ~Monster();

    virtual void Act(Level* level) override;
    void Move(Level* level);
    virtual void Attack(Level* level) override;
   // void ActIdle(Level* level);

    bool NeedsInput() override { return false; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    // 랜덤한 위치로 이동-wandering-플레이어 있는지 계속 체크
   void SetRandomTargetPos();

    // monsterAI
    MonsterAI* monsterAi;
    // void SetEntityState(EntityState state) { this->curState = state; }
    void SetTarget(Entity* actor) { this->target = actor; }
    void SetTargetPos(FPOINT targetPos) { this->targetPos = targetPos; }
    void SetDestPos(FPOINT destPos) { this->destPos = destPos; }
    FPOINT GetDestPos() { return destPos; }
    //FPOINT GetTargetPos() { return targetPos; }
    Entity* GetTarget() { return target; }


    // 타겟 위치를 destPos로 잡고, 매 targetPos(지금 갈 곳)을 paht 1로 설정?
};

