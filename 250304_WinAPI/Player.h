#pragma once
#include "Entity.h"

class Level;
class PathFinder;
class Player : public Entity
{
public:
    Player(FPOINT pos, float speed, int hp, int attDmg, int defense);
    virtual ~Player();

    virtual void Act(Level* level) override;
    void Move(Level* level);
    virtual void Attack(Level* level) override;
    void ActIdle(Level* level);

    bool NeedsInput() override { return true; }
    bool IsBusy() override { return curState != EntityState::IDLE; }

    void SetNextPos(FPOINT pos) { destPos = pos; }
private:

};