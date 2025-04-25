#pragma once
#include "Monster.h"
class Gnoll : public Monster
{
public:
	Gnoll(FPOINT pos, float speed);
	virtual ~Gnoll();
};

