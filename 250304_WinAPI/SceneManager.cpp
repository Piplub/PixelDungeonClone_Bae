#include "SceneManager.h"
#include "GameObject.h"
#include "resource4.h"


GameObject* SceneManager::currentScene = nullptr;
GameObject* SceneManager::loadingScene = nullptr;
GameObject* SceneManager::nextScene = nullptr;

// 쓰레드 만들 때 기본 틀임
DWORD CALLBACK LoadingThread(LPVOID pvParam)	//LPVOID는 void의 포인터형. 어떤 타입을 넘겨주더라도 매개변수로 형변환이 가능하도록 void 포인터 넣어줌
{// 콜백으로 내가 종료됐다 알려줌.
	if (SUCCEEDED(SceneManager::nextScene->Init()))
	{
		SceneManager::currentScene = SceneManager::nextScene;
		SceneManager::loadingScene->Release();
		SceneManager::loadingScene = nullptr;
		SceneManager::nextScene = nullptr;		// 릴리즈는 하지 않는다.
	}

	return 0;
}

void SceneManager::Init()
{
}

void SceneManager::Release()
{
	map<string, GameObject*>::iterator iter;
	for (iter = mapScenes.begin(); iter != mapScenes.end(); iter++)
	{
		if (iter->second)
		{
			iter->second->Release();
			delete iter->second;
			iter->second = nullptr;
		}
	}
	mapScenes.clear();
	ReleaseInstance();
}

void SceneManager::Update()
{
	if (sceneChangeRequested)
	{
		sceneChangeRequested = false;
		ChangeScene(nextSceneKey); // 지금 있는 ChangeScene 재활용
	}

	if (currentScene)
	{
		currentScene->Update();
	}
}

void SceneManager::Render(HDC hdc)
{
	if (currentScene)
	{
		currentScene->Render(hdc);
	}
}

HRESULT SceneManager::ChangeScene(string key)
{
	auto iter = mapScenes.find(key);	// nextScene
	if (iter == mapScenes.end())
	{
		return E_FAIL;
	}

	if (iter->second == currentScene)
	{
		return S_OK;
	}

	if (SUCCEEDED(iter->second->Init()))
	{
		if (currentScene)
		{
			currentScene->Release();
		}
		currentScene = iter->second;

		
		if (key == "타일맵툴") {
			HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_TILEMAPTOOLMENU));
			SetMenu(g_hWnd, hMenu);
		}
		else if (key == "테스트게임씬") {
			HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_TESTMAPMENU));
			SetMenu(g_hWnd, hMenu);
		}
		else {
			SetMenu(g_hWnd, NULL);  // 메뉴 제거
		}
		DrawMenuBar(g_hWnd); // 메뉴 갱신 필수!

		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string key, string loadingKey)
{
	auto iter = mapScenes.find(key);	// nextScene
	if (iter == mapScenes.end())
	{
		return E_FAIL;
	}

	if (iter->second == currentScene)
	{
		return S_OK;
	}

	map<string, GameObject*>::iterator iterLoading;
	iterLoading = mapLoadingScenes.find(loadingKey);
	if (iterLoading == mapLoadingScenes.end())
	{
		return ChangeScene(key);
	}

	if (SUCCEEDED(iterLoading->second->Init()))
	{
		if (currentScene)
		{
			currentScene->Release();
		}
		currentScene = iterLoading->second;
		nextScene = iter->second;
		loadingScene = iterLoading->second;

		// 다음 씬을 초기화할 쓰레드를 생성
		DWORD loadingThreadId;
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, LoadingThread, NULL, 0, &loadingThreadId);		// 쓰레드를 하나 더 만들어서 다음 씬 준비. 로딩씬 나오는 동안.....

		if(hThread)
		{
			CloseHandle(hThread);
		}

		return S_OK;
	}
	return E_FAIL;
}

GameObject* SceneManager::AddScene(string key, GameObject* scene)
{
	if (scene == nullptr)
	{
		return nullptr;
	}

	auto iter = mapScenes.find(key);
	if (iter != mapScenes.end())
	{
		return iter->second;
	}

	mapScenes.insert(make_pair(key, scene));

    return scene;
}

GameObject* SceneManager::AddLoadingScene(string key, GameObject* scene)
{
	if (scene == nullptr)
	{
		return nullptr;
	}

	auto iter = mapLoadingScenes.find(key);
	if (iter != mapLoadingScenes.end())
	{
		return iter->second;
	}

	mapLoadingScenes.insert(make_pair(key, scene));

	return scene;
}

void SceneManager::RequestChangeScene(const string& key)
{
	nextSceneKey = key;
	sceneChangeRequested = true;
}
