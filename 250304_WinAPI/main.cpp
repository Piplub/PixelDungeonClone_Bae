// main.cpp

#include "config.h"
#include "D2DImage.h"
#include "FModSoundPlayer.h"
#include "MainGame.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
LPCWSTR g_lpszClassName = (LPCWSTR)TEXT("PixelDungeonClone");
MainGame g_mainGame;
POINT g_ptMouse;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpszCmdParam, int nCmdShow)
{
#pragma region WINAPI_Initialize
	g_hInstance = hInstance;

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 2068;
	wndClass.cbWndExtra = 2068;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHIELD));
	
	RegisterClassEx(&wndClass);

	RECT rcWindowSize = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect(&rcWindowSize, WS_OVERLAPPEDWINDOW, FALSE);
	int width = rcWindowSize.right - rcWindowSize.left;
	int height = rcWindowSize.bottom - rcWindowSize.top;

	g_hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, g_lpszClassName, g_lpszClassName,
		WS_OVERLAPPEDWINDOW, 50, 50, width, height,
		NULL, NULL, g_hInstance, NULL);

	ShowWindow(g_hWnd, nCmdShow);
#pragma endregion

#pragma region System_Initialize
	D2DImage::InitD2D(g_hWnd);
	TimerManager::GetInstance()->Init();
	FModSoundPlayer::GetInstance()->Init();
	g_mainGame.Init();
#pragma endregion

#pragma region Game_Loop
	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TimerManager::GetInstance()->Update();
			FModSoundPlayer::GetInstance()->Update();
			g_mainGame.Update();
			g_mainGame.Render();
		}
	}

	g_mainGame.Release();
	FModSoundPlayer::GetInstance()->Release();
	TimerManager::GetInstance()->Release();

	return (int)message.wParam;
#pragma endregion
}

LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}

