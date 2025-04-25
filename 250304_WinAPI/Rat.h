#pragma once
#include "Monster.h"
class Rat : public Monster
{
public:
	Rat(FPOINT pos, float speed);
	virtual ~Rat();
};

