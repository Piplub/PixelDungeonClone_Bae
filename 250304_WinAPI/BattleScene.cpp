﻿#include "BattleScene.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Tiles.h"

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, TILEMAPTOOL_Y);

	sampleTile = ImageManager::GetInstance()->AddImage(
		"배틀시티_샘플타일", L"Image/tiles0.bmp", 256, 64,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);

	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	//Sleep(3000);
	Load();

	return S_OK;
}

void BattleScene::Release()
{
	if (backGround)
	{
		backGround->Release();
		delete backGround;
		backGround = nullptr;
	}
}

void BattleScene::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("전투씬_1", "로딩_1");
	}
}

void BattleScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	// 메인 타일 영역
	//PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);

	// 메인 타일 영역
	for (int i = 0; i < TILE_X * TILE_Y; ++i)
	{
		sampleTile->FrameRender(hdc,
			240 + tileInfo[i].indX * 12,
			50 + tileInfo[i].indY * 12,
			12, 12,
			(int)FrameAdapter(tileInfo[i].tileCode).x,
			(int)FrameAdapter(tileInfo[i].tileCode).y,
			false, false);
	}
}

void BattleScene::Load()
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
