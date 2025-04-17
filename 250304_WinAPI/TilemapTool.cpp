#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Button.h"
#include "Tiles.h"
#include <functional>

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);

	nowZoomScale = 0;

	for (int i = 0; i < 5; ++i) {
		zoomedSampleTile[i] = ImageManager::GetInstance()->AddZoomedImage(
			"PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
			SAMPLE_TILE_X, SAMPLE_TILE_Y, zoomScales[i]);
	}

	selectedTileCode = 0b000;

	// 샘플 타일 영역
	tile000rc = { 900, 50, 948, 98 };
	tile110rc = { 1000, 50, 1048, 98 };
	tile111rc = { 1100, 50, 1148, 98 };

	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			tileInfo[i * TILE_X + j].tileCode = selectedTileCode;
			tileInfo[i * TILE_X + j].indX = j;
			tileInfo[i * TILE_X + j].indY = i;
		}
	}

	Load();

	/// 메인 타일 영역
	// whole box
	rcMain = { 200, 30, 200 + TILE_X * gridSize, 30 + TILE_Y * gridSize };
	// grid
	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			mainGrid[i * TILE_X + j].left = 200 + j * gridSize;
			mainGrid[i * TILE_X + j].top = 30 + i * gridSize;
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
		this->SaveAs();
		});

	hPen_forGrid = CreatePen(PS_SOLID, 1, RGB(0, 168, 107));

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

	DeleteObject(hPen_forGrid);
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
			int tileY = (posY-30) / gridSize;
			tileInfo[tileY * TILE_X + tileX].tileCode = selectedTileCode;
		}
	}

	if (saveButton)	saveButton->Update();

	if (KeyManager::GetInstance()->IsOnceKeyUp(VK_LBUTTON)) {
		AutoSave();
	}
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);

	// 메인 타일 영역
	for (int i = 0; i < TILE_X * TILE_Y; ++i)
	{
		zoomedSampleTile[nowZoomScale]->FrameRender(hdc,
			200 + tileInfo[i].indX * gridSize,
			30 + tileInfo[i].indY * gridSize,
			(int)FrameAdapter(tileInfo[i].tileCode).x,
			(int)FrameAdapter(tileInfo[i].tileCode).y,
			false, false);
	}

	if (gridLineOn) {
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen_forGrid);

		HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
		for (auto& g : mainGrid) {
			RenderRect(hdc, g);
		}
		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
	}
	

	// 샘플 타일 영역
	zoomedSampleTile[4]->FrameRender(hdc, tile000rc.left, tile000rc.top,
		(int)FrameAdapter(0b000).x, (int)FrameAdapter(0b000).y, false, false);

	zoomedSampleTile[4]->FrameRender(hdc, tile110rc.left, tile110rc.top,
		(int)FrameAdapter(0b110).x, (int)FrameAdapter(0b110).y, false, false);

	zoomedSampleTile[4]->FrameRender(hdc, tile111rc.left, tile111rc.top,
		(int)FrameAdapter(0b111).x, (int)FrameAdapter(0b111).y, false, false);

	// 선택된 타일
	zoomedSampleTile[4]->FrameRender(hdc,
		975, 300,
		(int)FrameAdapter(selectedTileCode).x,
		(int)FrameAdapter(selectedTileCode).y,
		false, false);

	if (saveButton) saveButton->Render(hdc);
}

void TilemapTool::AutoSave()
{
	// 파일 저장
	HANDLE hFile = CreateFile(
		L"TestMapData.dat", GENERIC_WRITE, 0, NULL,
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

#include <commdlg.h>  // GetSaveFileName 사용을 위한 헤더

void TilemapTool::SaveAs()
{
	// 파일 저장 다이얼로그 설정
	OPENFILENAME ofn;
	WCHAR szFile[MAX_PATH] = L"TileMapData.dat"; // 기본 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd; // 너의 윈도우 핸들
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Tilemap 데이터 파일 (*.dat)\0*.dat\0모든 파일 (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_OVERWRITEPROMPT; // 같은 이름이 있을 경우 덮어쓸지 묻기

	// 사용자가 경로를 선택했다면
	if (GetSaveFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, TEXT("파일 생성 실패"), TEXT("경고"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		WriteFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL);
		CloseHandle(hFile);

		MessageBox(g_hWnd, TEXT("저장 완료"), TEXT("알림"), MB_OK);
	}
}


void TilemapTool::Load()
{	
	// 파일 로드
	HANDLE hFile = CreateFile(
		L"TestMapData.dat", GENERIC_READ, 0, NULL,
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

#include <commdlg.h>  // GetOpenFileName을 사용하려면 필요

void TilemapTool::LoadAs()
{
	// 파일 열기 다이얼로그 설정
	OPENFILENAME ofn;
	WCHAR szFile[MAX_PATH] = L""; // 초기 파일 이름은 없음

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd; // 네 윈도우 핸들
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Tilemap 데이터 파일 (*.dat)\0*.dat\0모든 파일 (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST; // 존재하는 파일만 선택 가능

	// 사용자가 파일을 선택했을 경우
	if (GetOpenFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
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

		//MessageBox(g_hWnd, TEXT("불러오기 완료"), TEXT("알림"), MB_OK);
	}
}

