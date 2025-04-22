#pragma once
#include "config.h"

class D2DImage;
class TurnManager;
class Entity;
class Item;
class Biome;
class Image;
class Camera;
class Level
{
private:

	//map
	Map map[TILE_Y * TILE_X];
	RECT mapRc;
	TileData tileData[TILE_Y * TILE_X];
	//RECT tempTile[TILE_Y * TILE_X]; //타일 이미지 넣기 전 임시 이미지 그리기용 배열
	//int tempTileSize; // 20*20 규격의 맵에 알맞은 임시 타일 사이즈

	Camera* camera;

	// Image* zoomedSampleTile[5];
	// const float zoomScales[5] = { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };
	
	bool shouldBeRender[TILE_Y * TILE_X]; //인덱스별 렌더해야하는지 여부 저장하는 배열
	bool hasExplored[TILE_Y * TILE_X]; //인덱스별 해금되었는지 여부 저장하는 배열
	bool isSeen[TILE_Y * TILE_X]; //인덱스별 시야 안에 들어왔는지 여부 저장하는 배열

	const POINT GRID_POS_OFFSET = {(3240 - TILE_SIZE*TILE_X)/2, (2160 - TILE_SIZE * TILE_Y) / 2 };

	vector<Biome*> bioms;
	vector<Item*> items;
	vector<Entity*> monsters;

	D2DImage* sampleTile;
	
	// HBRUSH BlackBrush;
	// HBRUSH GreyBrush;
	// HBRUSH WhiteBrush;
	// HBRUSH RedBrush;
	//
	// HBRUSH hOldBrush;

	// Entity* player;
	TurnManager* turnManager;
	
	vector<Entity*> actors;
	// int startInd;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	Entity* GetActorAt(FPOINT pos);
	void AddActor(Entity* actor);
	int GetMapIndex(int x, int y) {return (y - GRID_POS_OFFSET.y - TILE_SIZE / 2) / 30 * TILE_X + (x - GRID_POS_OFFSET.x - TILE_SIZE / 2) / 30;}
	Map* GetMap(int x, int y) { return &map[(y - GRID_POS_OFFSET.y - TILE_SIZE / 2) / 30 * TILE_X + (x - GRID_POS_OFFSET.x - TILE_SIZE / 2) / 30]; }

	//FPOINT GetPosByGridIndex(int idx, int idy) const { return {GRID_POS_OFFSET.x + (idx * 2 + 1) / 2.f * tempTileSize, GRID_POS_OFFSET.y + (idy * 2 + 1) / 2.f * tempTileSize}; }
	
	void FileLoad();

	Level();
	~Level();
};

