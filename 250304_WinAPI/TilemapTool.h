#pragma once
#include "GameObject.h"


class Image;
class Button;
class TilemapTool : public GameObject
{
private:
	Image* sampleTile;

	TILE_INFO tileInfo[TILE_X * TILE_Y];

	const int gridSize{ 12 };
	
	//POINT selectedTile;

	Button* saveButton;

	int selectedTileCode;

	RECT tile000rc;
	RECT tile110rc;
	RECT tile111rc;

	RECT rcMain;
	RECT mainGrid[TILE_X * TILE_Y];

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Save();
	void Load();

	TilemapTool() {};
	virtual ~TilemapTool() {};
};

