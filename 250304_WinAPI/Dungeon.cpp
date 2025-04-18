#include "Dungeon.h"
#include "CommonFunction.h"

HRESULT Dungeon::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	return S_OK;
}

void Dungeon::Release()
{
	for (auto& l : levels) {
		l->Release();
		l = nullptr;
	}
}

void Dungeon::Update()
{
	levels[currLevel]->Render(hdc);
}

void Dungeon::Render(HDC hdc)
{
	levels[currLevel]->Render(hdc);
}
