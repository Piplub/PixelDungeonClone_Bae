#include "Item.h"
#include "D2DImageManager.h"
#include "Level.h"

Item::Item()
{
	image = D2DImageManager::GetInstance()->AddImage("아이템", L"Image\\Items.png", 16, 32);
}

Item::~Item()
{
}

void Item::Use(Entity* user, Level* level)
{
}
