#pragma once
#include "GameObject.h"
class Level;
class Dungeon : public GameObject
{
private:
	Level* levels[7];
	int currLevel;
	const int testLevel = 7;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
};

