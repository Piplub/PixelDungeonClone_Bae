#pragma once
#include "config.h"
class Actor;
class Item;
class Biome;
class Tile;
class Image;
class Level
{
private:
	TILE_INFO tileInfo[TILE_Y * TILE_X];

	Image* zoomedSampleTile[5];
	const float zoomScales[5] = { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };

	Tile* tiles[TILE_Y * TILE_X];
	int currTileInd;
	vector<Tile*> selectedTiles;

	vector<Biome*> bioms;
	vector<Item*> items;
	vector<Actor*> monsters;

	Actor* player;

	float nowZoomScale;

	

public:
	void Init();
	void Release();
	void Update();
	void Render();

	void FileLoad();

	Level();
	~Level();
};

