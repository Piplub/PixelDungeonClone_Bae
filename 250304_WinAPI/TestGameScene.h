#pragma once
#include "GameObject.h"
class TestLevel;

class TestGameScene : public GameObject
{
private:
	TestLevel* testLevel;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void Restart();
	void Exit();

	TestGameScene();
	virtual ~TestGameScene() {};
};

