#include "GameScene.h"
#include "CommonFunction.h"
#include "Level.h"
#include "UIManager.h"
#include "GameOver.h"
#include "Player.h"
HRESULT GameScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	player = new Player({0,0}, 1000.f, 20, 50, 2);

	uiManager = UIManager::GetInstance();
	uiManager->Init();
	uiManager->RegisterPlayer(player);
	uiManager->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart();         // 함수 등록 (람다)
		});

	uiManager->GetUiGameOver()->SetExitCallBack([]() {
		PostQuitMessage(0); });
	
	currLevel = 0;
	for (int i = 0; i < 7; ++i) {
		levels[i] = new Level();
		levels[i]->Init(player, i+1, true);
		levels[i]->SetAscending([this]() {
			this->Ascend(); });
		levels[i]->SetDescending([this]() {
			this->Descend(); });
	}

	player->SetPosition(levels[currLevel]->GetPlayerInitP());

	
	

	return S_OK;
}

void GameScene::Release()
{
	for (auto& l : levels) {
		if (l != nullptr) {
			l->Release();
			delete l;
			l = nullptr;
		}
	}

	if (player)
	{
		//delete player;
		player = nullptr;
		UIManager::GetInstance()->SetCurrentPlayer(nullptr);
	}
}

void GameScene::Update()
{
	levels[currLevel]->Update();
}

void GameScene::Render(HDC hdc)
{
	levels[currLevel]->Render(hdc);
}

void GameScene::Restart()
{
	Release();
	Init();

}

void GameScene::Ascend()
{
	currLevel--;
	if (currLevel < 0) {
		currLevel = 0;
		// 로그창에 현재층 0층이라는 거 나오게 하기
	}
	else {
		player->SetPosition(levels[currLevel]->GetPlayerInitP());
	}
	
}

void GameScene::Descend()
{
	currLevel++;
	if (currLevel > 6) {
		currLevel = 6;
		// 로그창에 현재층 마지막 층이라는 거 나오게 하기
	}
	else {
		player->SetPosition(levels[currLevel]->GetPlayerInitP());
	}
}

GameScene::GameScene()
{
	for (auto& l : levels) {
		l = nullptr;
	}
}
