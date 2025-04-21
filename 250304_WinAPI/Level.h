#pragma once
#include "config.h"
#include "Map.h"

class Entity;
class Item;
class Biome;
class Image;
class Level
{
private:
	//Map map[TILE_Y * TILE_X];
	Map map[TILE_Y * TILE_X];
	RECT mapRc;
	RECT tempTile[TILE_Y * TILE_X]; //타일 이미지 넣기 전 임시 이미지 그리기용 배열
	int tempTileSize; // 20*20 규격의 맵에 알맞은 임시 타일 사이즈: 30

	HBRUSH BlackBrush;
	HBRUSH GreyBrush;
	HBRUSH WhiteBrush;
	HBRUSH RedBrush;

	HBRUSH hOldBrush;

	Entity* player;
	int startInd;

	Image* zoomedSampleTile[5];
	const float zoomScales[5] = { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };
	float nowZoomScale;

	bool shouldBeRender[TILE_Y * TILE_X]; //인덱스별 렌더해야하는지 여부 저장하는 배열
	bool hasExplored[TILE_Y * TILE_X]; //인덱스별 해금되었는지 여부 저장하는 배열
	bool isSeen[TILE_Y * TILE_X]; //인덱스별 시야 안에 들어왔는지 여부 저장하는 배열


	//int currTileInd;
	//vector<Tile*> selectedTiles;

	vector<Biome*> bioms;
	vector<Item*> items;
	vector<Entity*> monsters;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void FileLoad();

	Level();
	~Level();
};

