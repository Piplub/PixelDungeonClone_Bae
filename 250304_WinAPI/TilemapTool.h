#pragma once
#include "GameObject.h"


class Image;
class Button;
class TilemapTool : public GameObject
{
private:
	Image* zoomedSampleTile[5];
	const float zoomScales[5] = { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };
	int nowZoomScale;

	TILE_INFO tileInfo[TILE_X * TILE_Y];

	const int gridSize{ 16 };
	HPEN hPen_forGrid;
	
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

