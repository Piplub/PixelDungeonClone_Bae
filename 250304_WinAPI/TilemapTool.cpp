#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Button.h"
#include "Tiles.h"
#include <functional>

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);

	sampleTile = ImageManager::GetInstance()->AddImage(
		"PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);

	selectedTileCode = 0b000;

	// 샘플 타일 영역
	tile000rc = { 900, 50, 950, 100 };
	tile110rc = { 1000, 50, 1050, 100 };
	tile111rc = { 1100, 50, 1150, 100 };

	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			tileInfo[i * TILE_X + j].tileCode = selectedTileCode;
			tileInfo[i * TILE_X + j].indX = j;
			tileInfo[i * TILE_X + j].indY = i;
		}
	}

	//Load();

	/// 메인 타일 영역
	// whole box
	rcMain = { 200, 50, 200 + TILE_X * gridSize, 50 + TILE_Y * gridSize };
	// grid
	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			mainGrid[i * TILE_X + j].left = 200 + j * gridSize;
			mainGrid[i * TILE_X + j].top = 50 + i * gridSize;
			mainGrid[i * TILE_X + j].right = mainGrid[i * TILE_X + j].left + gridSize;
			mainGrid[i * TILE_X + j].bottom = mainGrid[i * TILE_X + j].top + gridSize;
		}
	}
	
	// UI - 버튼
	saveButton = new Button();
	saveButton->Init(1000,500);
	//saveButton->SetFunction(&TilemapTool::Save, this);
	//saveButton->SetFunction(std::bind(&TilemapTool::Save, this));
	saveButton->SetFunction([this]() {
		this->Save();
		});

	return S_OK;
}

void TilemapTool::Release()
{
	if (saveButton)
	{
		saveButton->Release();
		delete saveButton;
		saveButton = nullptr;
	}
}

void TilemapTool::Update()
{
	if (PtInRect(&tile000rc, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			selectedTileCode = 0b000;
		}
	}

	if (PtInRect(&tile110rc, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			selectedTileCode = 0b110;
		}
	}

	if (PtInRect(&tile111rc, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			selectedTileCode = 0b111;
		}
	}
	else if (PtInRect(&rcMain, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x;
			int posY = g_ptMouse.y;
			int tileX = (posX-200) / gridSize;
			int tileY = (posY-50) / gridSize;
			tileInfo[tileY * TILE_X + tileX].tileCode = selectedTileCode;
		}
	}

	if (saveButton)	saveButton->Update();
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);

	// 메인 타일 영역
	for (int i = 0; i < TILE_X * TILE_Y; ++i)
	{
		sampleTile->FrameRender(hdc, 
			200 + tileInfo[i].indX * gridSize,
			50 + tileInfo[i].indY * gridSize,
			gridSize, gridSize,
			(int)FrameAdapter(tileInfo[i].tileCode).x,
			(int)FrameAdapter(tileInfo[i].tileCode).y,
			false, false);
	}

	/*for (auto& g : mainGrid) {
		RenderRect()
	}*/

	// 샘플 타일 영역
	sampleTile->FrameRender(hdc, tile000rc.left, tile000rc.top, 50, 50, 
		(int)FrameAdapter(0b000).x, (int)FrameAdapter(0b000).y, false, false);

	sampleTile->FrameRender(hdc, tile110rc.left, tile110rc.top, 50, 50,
		(int)FrameAdapter(0b110).x, (int)FrameAdapter(0b110).y, false, false);

	sampleTile->FrameRender(hdc, tile111rc.left, tile111rc.top, 50, 50,
		(int)FrameAdapter(0b111).x, (int)FrameAdapter(0b111).y, false, false);

	// 선택된 타일
	sampleTile->FrameRender(hdc, 
		975, 300, 50, 50,
		(int)FrameAdapter(selectedTileCode).x,
		(int)FrameAdapter(selectedTileCode).y,
		false, false);

	if (saveButton) saveButton->Render(hdc);
}

void TilemapTool::Save()
{
	// 파일 저장
	HANDLE hFile = CreateFile(
		L"TileMapData.dat", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 생성 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	WriteFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL);
	CloseHandle(hFile);
}

void TilemapTool::Load()
{	
	// 파일 로드
	HANDLE hFile = CreateFile(
		L"TileMapData.dat", GENERIC_READ, 0, NULL,
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
