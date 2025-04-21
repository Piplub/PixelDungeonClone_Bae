#include "Level.h"
#include "CommonFunction.h"

void Level::Init()
{

	nowZoomScale = 0.0f;

	player = nullptr;

	tempTileSize = 30;

	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			tempTile[TILE_X * i + j] = 
			{	240+j*tempTileSize, 
				60+i*tempTileSize,
				240 + (j+1) * tempTileSize, 
				60 + (i+1) * tempTileSize 
			};
		}
	}

	mapRc = { tempTile[0].left, tempTile[0].top, tempTile[399].right, tempTile[399].bottom };

	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	GreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	RedBrush = CreateSolidBrush(RGB(255, 0, 0));



	for (auto& s : shouldBeRender) 
	{
		s = true;
	}

	for (auto& h : hasExplored) 
	{
		h = true;
	}

	for (auto& i : isSeen) 
	{
		i = true;
	}

	/*for (int i = 0; i < 5; ++i) {
		zoomedSampleTile[i] = ImageManager::GetInstance()->AddZoomedImage(
			"PixelDungeon_Tile0", L"Image/tiles0.bmp", 256, 64,
			SAMPLE_TILE_X, SAMPLE_TILE_Y, zoomScales[i]);
	}*/

	//Sleep(3000);
	FileLoad();

	/*for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			tiles[i * TILE_X + j] = new Tile(j, i, tileInfo[i * TILE_X + j].tileCode);
		}
	}*/

}

void Level::Release()
{
	DeleteObject(BlackBrush);
	DeleteObject(GreyBrush);
	DeleteObject(WhiteBrush);
	DeleteObject(RedBrush);
}

void Level::Update()
{
	/*if (MouseManager::GetInstance()->GetValueUsed() == false) {
		POINT p = MouseManager::GetInstance()->GetMousePos();

		if (PtInRect(&mapRc, p)) {
			long indX = (p.x - mapRc.left) / tempTileSize;
			long indY = (p.y - mapRc.top) / tempTileSize;

			map[indY * 20 + indX].type = TT::COUNT;
		}

		MouseManager::GetInstance()->AlreadyUsed();
	}*/
	if (PtInRect(&mapRc, MouseManager::GetInstance()->GetDragEndP())) 
	{
		if (MouseManager::GetInstance()->GetValueUsed() == false) {

			long posX = MouseManager::GetInstance()->GetDragEndP().x;
			long posY = MouseManager::GetInstance()->GetDragEndP().y;

			long indX = (posX - mapRc.left) / tempTileSize;
			long indY = (posY - mapRc.top) / tempTileSize;

			if (indX >= 0 && indX < TILE_X && indY >= 0 && indY < TILE_Y)	///
			{														/// 구현 하고 싶은 로직 넣는 부분
				map[indY * TILE_X + indX].type = TT::COUNT;				///
			}														///

			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyUsed();
		}
	} ///디버깅을 위해 마우스 왼쪽 버튼을 떼면 그 자리에 있는 타일이 빨간색으로 변하게 해놨습니다. 
	  ///맵으로 사용하실 땐 타일 선택 로직(이동 및 공격)을 써주세요!
	
	if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT))
	{
		long tempDeltaX = MouseManager::GetInstance()->GetDeltaX();
		long tempDeltaY = MouseManager::GetInstance()->GetDeltaY();

		for (auto& t : tempTile) {
			t.left += tempDeltaX;
			t.right += tempDeltaX;
			t.top += tempDeltaY;
			t.bottom += tempDeltaY;
		}

		mapRc.left += tempDeltaX;
		mapRc.right += tempDeltaX;
		mapRc.top += tempDeltaY;
		mapRc.bottom += tempDeltaY;

	}
}

void Level::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);

	RenderRect(hdc, mapRc);

	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			
			switch (map[20 * i + j].type) {
				case TT::WALL :
					hOldBrush = (HBRUSH)SelectObject(hdc, GreyBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
				case TT::FLOOR:
					hOldBrush = (HBRUSH)SelectObject(hdc, WhiteBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
				case TT::NONE:
					hOldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
				default: 
					hOldBrush = (HBRUSH)SelectObject(hdc, RedBrush);
					RenderRect(hdc, tempTile[20 * i + j]);
					SelectObject(hdc, hOldBrush);
					break;
			}
		}
	}
}

void Level::FileLoad()
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
	if (!ReadFile(hFile, map, sizeof(map), &dwByte, NULL))
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
