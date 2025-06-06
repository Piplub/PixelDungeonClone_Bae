﻿#include "MainGame.h"
#include "CommonFunction.h"
// #include "Image.h"
#include "Timer.h"
#include "TilemapTool.h"
// #include "AstarScene.h"
#include "LoadingScene.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "MouseManager.h"
// #include "BJS_TestScene.h"
#include "GameScene.h"
#include "TestGameScene.h"
#include "resource4.h"
#include "LobbyScene.h"


HRESULT MainGame::Init()
{
#pragma region Manager_Initialze
	// ImageManager::GetInstance()->Init();
	D2DImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	MouseManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
#pragma endregion

#pragma region Scene_Initialize
	SceneManager::GetInstance()->AddScene("타일맵툴", new TilemapTool());
	SceneManager::GetInstance()->AddScene("게임씬", new GameScene());
	SceneManager::GetInstance()->AddScene("테스트게임씬", new TestGameScene());
	SceneManager::GetInstance()->AddLoadingScene("로딩_1", new LoadingScene());

	SceneManager::GetInstance()->AddScene("로비씬", new LobbyScene());
	SceneManager::GetInstance()->ChangeScene("로비씬");
	//SceneManager::GetInstance()->ChangeScene("게임씬");
	//SceneManager::GetInstance()->ChangeScene("테스트게임씬");

	hdc = GetDC(g_hWnd);
#pragma endregion

	return S_OK;
}

void MainGame::Release()
{
	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	MouseManager::GetInstance()->Release();
	D2DImageManager::GetInstance()->Release();
	// ImageManager::GetInstance()->Release();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	MouseManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	D2DImage::BeginDraw();
	D2DImage::Clear(D2D1::ColorF(D2D1::ColorF::Black));
	
	SceneManager::GetInstance()->Render(hdc);

	D2DImage::EndDraw();
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			// 타일맵툴 메뉴
		case ID_FILE_NEW:
			//
			break;
		case ID_FILE_OPEN:
			if (dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene())->LoadAs();
			}
			break;
		case ID_FILE_SAVE:
			if (dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene())->Save();
			}
			break;
		case ID_FILE_SAVEAS:
			if (dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene())->SaveAs();
			}
			break;
		case ID_TILE_TEST:
			if (dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene())->Test();
			}
			break;
		case ID_TILE_SHOWGRID:
			if (dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene())->SetGridLine();
			}
			break;
		case ID_TILE_ERASE:
			if (dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene())->Erase();
			}
			break;
			// 테스트게임씬 메뉴
		case ID_BATTLE_RESTART:
			if (dynamic_cast<TestGameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TestGameScene*>(SceneManager::GetInstance()->GetCurrentScene())->Restart();
			}
			break;
		case ID_BATTLE_EXIT:
			if (dynamic_cast<TestGameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
			{
				static_cast<TestGameScene*>(SceneManager::GetInstance()->GetCurrentScene())->Exit();
			}
			break;
		case ID_BATTLE_SOUND:
			//
			break;
		}
		break;
	
	case WM_MOUSEWHEEL:
	{
		short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam); // 120 단위로 올라감/내려감
		MouseManager::GetInstance()->SetWheelDelta(wheelDelta);
		break;
	}
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);

		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
