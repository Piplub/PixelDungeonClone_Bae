#pragma once
#include "Consumable.h"
class TeleportScroll : public Consumable
{
private:

public:
	TeleportScroll();
	virtual ~TeleportScroll();

	void Use(Entity* user, Level* level) override;
	void Render(HDC hdc) override;
};

