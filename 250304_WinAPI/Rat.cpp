#include "Rat.h"
#include "D2DImageManager.h"
#include "Animator.h"
#include "CombatSyetem.h"

Rat::Rat(FPOINT pos, float speed) : Monster()
{
    position = pos;
    this->speed = speed;
    hp = 8;
    maxHp = hp;
    attackDmg = { 1, 5 };
    defense = 1;
    exp = 1;

    // 에너지
    actionCost = 10.f;
    energyPerTurn = 10.0f;

    image = D2DImageManager::GetInstance()->AddImage("쥐", L"Image\\rat.png", 15, 3);

    animator->AddClip("Idle", { 0,  1, 0.5f, true,  nullptr });
    animator->AddClip("Move", { 6,  10, 0.2f, true,  nullptr });
    animator->AddClip("Attack", { 2, 5, 0.1f, false, [this]() {
        // 공격 애니 끝나고 실행할 콜백
        if (target)
        {
            CombatSyetem::GetInstance()->ProcessAttack(this, target);
            Stop();
            SetState(EntityState::WAIT);
        }
    } });
    animator->AddClip("Dead", { 11, 14, 0.3f, false, nullptr });
}

Rat::~Rat()
{
}
