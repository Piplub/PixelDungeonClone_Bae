#pragma once
#include "Singleton.h"

class Entity;
class CombatSyetem : public Singleton<CombatSyetem>
{
private:

public:
	int CalculateDmg(Entity* attacker, Entity* defender);
	void ProcessAttack(Entity* attacker, Entity* defender);
};

