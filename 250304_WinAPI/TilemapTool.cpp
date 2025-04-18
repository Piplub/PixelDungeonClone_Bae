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

	if (wcslen(GameObject::nowFilePath) > 0) {
		Load();
	}

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
		this->Save();
		});

	hPen_forGrid = CreatePen(PS_SOLID, 1, RGB(0, 168, 107));
	hPen_forSample = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	dragRc = { 0, 0, 1, 1 };


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
	DeleteObject(hPen_forSample);
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

		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON)) {
			isDragging = true;
			dragStartP = g_ptMouse;
		}

		if (isDragging && KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON)) {
			dragNowP = g_ptMouse;

			dragRc.left = min(dragStartP.x, dragNowP.x);
			dragRc.right = max(dragStartP.x, dragNowP.x);
			dragRc.top = min(dragStartP.y, dragNowP.y);
			dragRc.bottom = max(dragStartP.y, dragNowP.y);
		}

		if (KeyManager::GetInstance()->IsOnceKeyUp(VK_RBUTTON)) {
			if (KeyManager::GetInstance()->IsStayKeyDown(VK_LSHIFT)) {

				for (int i = 0; i < TILE_Y; ++i) {
					for (int j = 0; j < TILE_X; ++j) {
						if (RectInRect(mainGrid[i * TILE_X + j], dragRc)) {
							tileInfo[i * TILE_X + j].tileCode = 0b110;
							specialTiles.push_back({ j, i });
						}
					}
				}

				POINT dir[8] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1,1} };
				for (auto& ind : specialTiles) {
					for (int i = 0; i < 8; ++i) {
						if (tileInfo[(ind.y + dir[i].y) * TILE_X + (ind.x + dir[i].x)].tileCode == 0b000) {
							tileInfo[(ind.y + dir[i].y) * TILE_X + (ind.x + dir[i].x)].tileCode = 0b111;
						}
					}
				}

				specialTiles.clear();

			}
			else {
				for (int i = 0; i < TILE_Y; ++i) {
					for (int j = 0; j < TILE_X; ++j) {
						if (RectInRect(mainGrid[i * TILE_X + j], dragRc)) {
							tileInfo[i * TILE_X + j].tileCode = selectedTileCode;
						}
					}
				}
			}
			
			isDragging = false;
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

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen_forSample);

	HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
	switch (selectedTileCode) {
	case 0b000:
		RenderRect(hdc, tile000rc);
		break;
	case 0b110:
		RenderRect(hdc, tile110rc);
		break;
	case 0b111:
		RenderRect(hdc, tile111rc);
		break;
	}

	if (isDragging) {
		RenderRect(hdc, dragRc);
	}

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	// 선택된 타일
	/*zoomedSampleTile[4]->FrameRender(hdc,
		975, 300,
		(int)FrameAdapter(selectedTileCode).x,
		(int)FrameAdapter(selectedTileCode).y,
		false, false);*/

	if (saveButton) saveButton->Render(hdc);
}

void TilemapTool::AutoSave()
{
	if (GameObject::nowFilePath[0] != L'\0' && wcscmp(GameObject::nowFilePath, L"TestMapData.dat") != 0)
	{
		return;
	}
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
	wcscpy_s(GameObject::nowFilePath, MAX_PATH, L"TestMapData.dat");
	CloseHandle(hFile);
}

void TilemapTool::Save()
{
	if (GameObject::nowFilePath[0] == L'\0' || wcscmp(GameObject::nowFilePath, L"TestMapData.dat") == 0)
	{
		SaveAs();
		return;
	}
	// save to exist file
	HANDLE hFile = CreateFile(
		nowFilePath, GENERIC_WRITE, 0, NULL,
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
	TCHAR prevDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, prevDir);
	OPENFILENAME ofn;
	WCHAR szFile[MAX_PATH] = L"TileMapData.dat"; 

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd; 
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Tilemap 데이터 파일 (*.dat)\0*.dat\0모든 파일 (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_OVERWRITEPROMPT; 

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
		wcscpy_s(GameObject::nowFilePath, MAX_PATH, ofn.lpstrFile);

		CloseHandle(hFile);
		SetCurrentDirectory(prevDir);
		MessageBox(g_hWnd, TEXT("저장 완료"), TEXT("알림"), MB_OK);
	}
}


void TilemapTool::Load()
{
	LPCWSTR pathToLoad = (GameObject::nowFilePath[0] != L'\0') ? GameObject::nowFilePath : L"TestMapData.dat";

	HANDLE hFile = CreateFile(
		pathToLoad, GENERIC_READ, 0, NULL,
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


#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


void TilemapTool::LoadAs()
{
	TCHAR prevDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, prevDir);
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
		wcscpy_s(GameObject::nowFilePath, MAX_PATH, ofn.lpstrFile);
		CloseHandle(hFile);
		//MessageBox(g_hWnd, TEXT("불러오기 완료"), TEXT("알림"), MB_OK);
	}
	TCHAR loadedPath[MAX_PATH];
	SetCurrentDirectory(prevDir);
}

void TilemapTool::Erase()
{
	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			tileInfo[i * TILE_X + j].tileCode = 0b000;
			tileInfo[i * TILE_X + j].indX = j;
			tileInfo[i * TILE_X + j].indY = i;
		}
	}
}

void TilemapTool::Paint()
{
}

void TilemapTool::Test()
{
	SceneManager::GetInstance()->ChangeScene("전투씬_1");
}

