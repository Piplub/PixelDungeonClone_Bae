#pragma once
#include "Singleton.h"
#include "config.h"

class GameObject;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, GameObject*> mapScenes;
	map<string, GameObject*> mapLoadingScenes;

	string nextSceneKey;
	bool sceneChangeRequested = false;

public:
	static GameObject* currentScene;
	static GameObject* loadingScene;
	static GameObject* nextScene;

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	HRESULT ChangeScene(string key);
	HRESULT ChangeScene(string key, string loadingKey);

	GameObject* AddScene(string key, GameObject* scene);
	GameObject* AddLoadingScene(string key, GameObject* scene);
	inline GameObject* GetCurrentScene() { return currentScene; }

	void RequestChangeScene(const string& key);
};

