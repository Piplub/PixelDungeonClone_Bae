#include "TeleportScroll.h"
#include "Player.h"
#include "Level.h"

TeleportScroll::TeleportScroll()
{
	this->position = position;
	this->name = "텔포스크롤";
	this->imageIdX = 4;
	this->imageIdY = 18;
}

TeleportScroll::~TeleportScroll()
{
}

void TeleportScroll::Use(Entity* user, Level* level)
{
	FPOINT tPosition = level->GetRandomFloorTile();
	static_cast<Player*>(user)->SetPosition(tPosition);
}

void TeleportScroll::Render(HDC hdc)
{
}
