#include "Monster.h"
// #include "Game.h"
// #include "Map.h"
#include "Level.h"
#include "TurnManager.h"
#include "PathFinder.h"
#include "CombatSyetem.h"

Monster::Monster(FPOINT pos, float speed, int hp, int attDmg, int defense)
{
    position = pos;
    this->speed = speed;
    this->hp = hp;
    this->maxHp = hp;
    this->attackDmg = attDmg;
    this->defense = defense;
    isMoving = false;
    isActive = true;

    type = EntityType::MONSTER;
    curState = EntityState::IDLE;

    targetPos = { position.x + TILE_SIZE, position.y + TILE_SIZE };
    // targetPos = pos

    // 에너지 test
    actionCost = 6.f;
    energyPerTurn = 10.0f;
}

Monster::~Monster()
{
}

void Monster::Act(Level* level)
{
    switch (curState)
    {
    case EntityState::IDLE:
        ActIdle(level);
        return;
    case EntityState::MOVE:
        Move(level);
        return;
    case EntityState::ATTACK:
        Attack(level);
        return;
    case EntityState::DEAD:
        // 애니메이션 끝나면 actor 목록에서 지워야함
        return;
    }
}

void Monster::Attack(Level* level)
{
    if (target)
    {
        CombatSyetem::GetInstance()->ProcessAttack(this, target);
        SetRandomTargetPos();
        curState = EntityState::IDLE;
    }
}

void Monster::ActIdle(Level* level)
{
    target = level->GetActorAt(targetPos);
    if (target)
    {
        if (target->GetType() == EntityType::PLAYER)
        {
            curState = EntityState::ATTACK;
            return;
        }
    }
    curState = EntityState::MOVE;
}

void Monster::Move(Level* level)
{
    // 이동 순서 체크용
    // Sleep(100);
    auto index = level->GetMapIndex(targetPos.x, targetPos.y);
    int a = 0;
    auto map = level->GetMap(targetPos.x, targetPos.y);
    
    if ((map && !map->CanGo()) || level->GetActorAt(targetPos))
    {
        SetRandomTargetPos();
        curState = EntityState::IDLE;
        return;
    }
    // if (!level->GetMap(targetPos.x, targetPos.x)->CanGo()) return;

    FPOINT delta = targetPos - position;

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    delta.Normalize();

    position.x += speed * deltaTime * delta.x;
    position.y += speed * deltaTime * delta.y;

    delta = targetPos - position;

    // 매직넘버로,,, -> 변수로 dir 저장해두고 쓰면 Dot Product
    if (delta.Length() <= 10.f)
    {
        position = targetPos;
        // 테스트용이라 도착지 정하는건 수정해야함
        //isMoving = false;
        SetRandomTargetPos();
        curState = EntityState::IDLE;
    }
}

void Monster::SetRandomTargetPos()
{
    // 랜덤으로 시드 생성
    random_device rd;

    // 고성능 엔진을 시드(rd())로 초기화
    mt19937_64 eng(rd());

    // 범위 설정
    uniform_int_distribution<int> dist(0, 3);
    FPOINT dir[] = { {-TILE_SIZE, 0}, {TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE} };

    targetPos = position + dir[dist(eng)];
}
