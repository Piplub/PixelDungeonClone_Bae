#include "HealPotion.h"
#include "Entity.h"

HealPotion::HealPotion()
{
}

HealPotion::~HealPotion()
{
}

void HealPotion::Use(Entity* user)
{
	if (!user || !user->IsAlive())	return;
	// �������̽���,,
	// user->Heal(healAmount);
}
