#include "Gnoll.h"
#include "D2DImageManager.h"
#include "Animator.h"
#include "CombatSyetem.h"

Gnoll::Gnoll(FPOINT pos, float speed) : Monster()
{
    position = pos;
    this->speed = speed;
    hp = 12;
    maxHp = hp;
    attackDmg = { 2, 5 };
    defense = 2;
    exp = 2;

    // 에너지
    actionCost = 10.f;
    energyPerTurn = 15.0f;

    image = D2DImageManager::GetInstance()->AddImage("놀", L"Image\\gnoll.png", 11, 1);

    animator->AddClip("Idle", { 0,  1, 0.5f, true,  nullptr });
    animator->AddClip("Move", { 4,  7, 0.2f, true,  nullptr });
    animator->AddClip("Attack", { 2, 3, 0.2f, false, [this]() {
        // 공격 애니 끝나고 실행할 콜백
        if (target)
        {
            CombatSyetem::GetInstance()->ProcessAttack(this, target);
            Stop();
            SetState(EntityState::WAIT);
        }
    } });
    animator->AddClip("Dead", { 8, 11, 0.3f, false, nullptr });
}

Gnoll::~Gnoll()
{
}
