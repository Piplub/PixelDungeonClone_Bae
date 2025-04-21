﻿#include "Level.h"
#include "CommonFunction.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "Entity.h"
#include "Monster.h"
#include "Player.h"
#include "TurnManager.h"
#include "Camera.h"

void Level::Init()
{
	turnManager = new TurnManager();

	camera = new Camera();
	camera->Init();

	sampleTile = D2DImageManager::GetInstance()->AddImage(
		"배틀시티_샘플타일", L"Image/mapTiles.bmp",
		SAMPLE_TILE_X, SAMPLE_TILE_Y);


	//tempTileSize = 30;

	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			tempTile[TILE_X * i + j] = 
			{	GRID_POS_OFFSET.x + j*TILE_SIZE, 
				GRID_POS_OFFSET.y + i* TILE_SIZE,
				GRID_POS_OFFSET.x + (j+1) * TILE_SIZE,
				GRID_POS_OFFSET.y + (i+1) * TILE_SIZE
			};
		}
	}

	mapRc = { tempTile[0].left, tempTile[0].top, tempTile[399].right, tempTile[399].bottom };

	// BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	// GreyBrush = CreateSolidBrush(RGB(100, 100, 100));
	// WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	// RedBrush = CreateSolidBrush(RGB(255, 0, 0));



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

	FileLoad();

	// 시작 위치 테스트용 매직넘버
	Entity* player = new Player(GetPosByGridIndex(3, 3), 100.f);
	Entity* monster1 = new Monster(GetPosByGridIndex(5, 4), 100.f);
	Entity* monster2 = new Monster(GetPosByGridIndex(4, 5), 100.f);

	AddActor(player);
	AddActor(monster1);
	AddActor(monster2);

	for (auto actor : actors)
	{
		if (actor)
			turnManager->AddActor(actor);
	}
	
}

void Level::Release()
{
	// DeleteObject(BlackBrush);
	// DeleteObject(GreyBrush);
	// DeleteObject(WhiteBrush);
	// DeleteObject(RedBrush);
	
	for (auto actor : actors)
	{
		if (actor)
		{
			// actor->Release();
			delete actor;
			actor = nullptr;
		}
	}
}

void Level::Update()
{
	camera->Update();

	POINT ConvertedDragEndP = {
		camera->ConvertToWorldX(MouseManager::GetInstance()->GetClickP().x),
		camera->ConvertToWorldY(MouseManager::GetInstance()->GetClickP().y)
	};

	if (PtInRect(&mapRc, ConvertedDragEndP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false) {

			long posX = ConvertedDragEndP.x;
			long posY = ConvertedDragEndP.y;

			long indX = (posX - mapRc.left) / TILE_SIZE;
			long indY = (posY - mapRc.top) / TILE_SIZE;

			if (indX >= 0 && indX < TILE_X && indY >= 0 && indY < TILE_Y)	
			{																/// 구현 하고 싶은 로직 넣는 부분
				map[indY * TILE_X + indX].type = TT::COUNT;					///
			}																

			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	} ///디버깅을 위해 마우스 왼쪽 버튼을 떼면 그 자리에 있는 타일이 빨간색으로 변하게 해놨습니다. 
	  ///맵으로 사용하실 땐 타일 선택 로직(이동 및 공격)을 써주세요!

	turnManager->ProcessTurns(this);

}

void Level::Render(HDC hdc)
{
	// PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	
	// RenderRect(hdc, mapRc);
	//sampleTile->DrawRect({(float)mapRc.left, (float)mapRc.top}, {(float)mapRc.right, (float)mapRc.bottom}, 1, 1);
	
	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			
			switch (map[TILE_X * i + j].type) {
				case TT::WALL :
					sampleTile->RenderFrameScale(
						(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
						(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
						camera->GetZoomScale(), camera->GetZoomScale(), 1, 0);
					// hOldBrush = (HBRUSH)SelectObject(hdc, GreyBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
				case TT::FLOOR:
					sampleTile->RenderFrameScale(
						(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
						(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
						camera->GetZoomScale(), camera->GetZoomScale(), 3, 0);
					// hOldBrush = (HBRUSH)SelectObject(hdc, WhiteBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
				case TT::NONE:
					sampleTile->RenderFrameScale(
						(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
						(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
						camera->GetZoomScale(), camera->GetZoomScale(), 0, 0);
					// hOldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
				default:
					sampleTile->RenderFrameScale(
						(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
						(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
						camera->GetZoomScale(), camera->GetZoomScale(), 1, 0);
					// hOldBrush = (HBRUSH)SelectObject(hdc, RedBrush);
					// RenderRect(hdc, tempTile[20 * i + j]);
					// SelectObject(hdc, hOldBrush);
					break;
			}
		}
	}

	for (auto actor : actors)
	{
		actor->Render(hdc);
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


void Level::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(actors.begin(), actors.end(), actor);
	if (it != actors.end())
		return;

	actors.push_back(actor);
}