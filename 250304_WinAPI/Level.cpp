#include "Level.h"
#include "CommonFunction.h"
#include "Tiles.h"

void Level::Init()
{

	nowZoomScale = 0;

	for (int i = 0; i < 5; ++i) {
		zoomedSampleTile[i] = ImageManager::GetInstance()->AddZoomedImage(
			"PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
			SAMPLE_TILE_X, SAMPLE_TILE_Y, zoomScales[i]);
	}

	//Sleep(3000);
	FileLoad();

	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			tiles[i * TILE_X + j] = new Tile(j, i, tileInfo[i * TILE_X + j].tileCode);
		}
	}

}

void Level::Release()
{
}

void Level::Update()
{
}

void Level::Render()
{
}

void Level::FileLoad()
{
	// 파일 로드
	HANDLE hFile = CreateFile(
		L"TilemapDatas/TestMap_Heart.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	if (!ReadFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL))
	{
		MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
	}
	CloseHandle(hFile);
}

Level::Level()
{
}

Level::~Level()
{
}
