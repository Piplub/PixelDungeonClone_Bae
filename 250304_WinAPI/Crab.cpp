#include "Crab.h"
#include "D2DImageManager.h"
#include "Animator.h"
#include "CombatSyetem.h"

Crab::Crab(FPOINT pos, float speed) : Monster()
{
    position = pos;
    this->speed = speed;
    hp = 15;
    maxHp = hp;
    attackDmg = { 3, 6 };
    defense = 4;
    exp = 4;

    // 에너지
    actionCost = 10.f;
    energyPerTurn = 20.0f;

    image = D2DImageManager::GetInstance()->AddImage("놀", L"Image\\crab.png", 16, 1);

    animator->AddClip("Idle", { 0,  2, 0.5f, true,  nullptr });
    animator->AddClip("Move", { 3,  6, 0.2f, true,  nullptr });
    animator->AddClip("Attack", { 7, 9, 0.2f, false, [this]() {
        // 공격 애니 끝나고 실행할 콜백
        if (target)
        {
            CombatSyetem::GetInstance()->ProcessAttack(this, target);
            Stop();
            SetState(EntityState::WAIT);
        }
    } });
    animator->AddClip("Dead", { 10, 13, 0.3f, false, nullptr });
}

Crab::~Crab()
{
}
