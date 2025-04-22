#pragma once
#include "GameObject.h"
#include "config.h"

class D2DImage;
// class Image;
class Button;
class TilemapTool : public GameObject
{
private:
	D2DImage* sampleTile;
	RECT rcSampleTile;

	TileData tileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	
	RECT tempTile[20 * 20]; //타일 이미지 넣기 전 임시 이미지 그리기용 배열
	int tempTileSize; // 20*20 규격의 맵에 알맞은 임시 타일 사이즈

	// HBRUSH tBlackBrush;
	// HBRUSH tGreyBrush;
	// HBRUSH tWhiteBrush;
	// HBRUSH tRedBrush;
	//
	// HBRUSH hOldBrush;

	POINT selectedTile;

	Button* saveButton;

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

