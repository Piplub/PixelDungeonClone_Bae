#pragma once
#include "Singleton.h"
#include "config.h"
#include <fmod.hpp>
#include <fmod_errors.h>

class FModSoundPlayer : public Singleton<FModSoundPlayer>
{
private:
    FMOD::System* fmodSystem = nullptr;
    FMOD::Sound* bgm = nullptr;
    map<string, FMOD::Sound*> sounds;
    multimap<string, FMOD::Channel*> channels;
    float timer = 0.f;
public:
    void Init();
    void Update();
    void Release();

    void LoadSound(const string& key, const char* filepath, bool loop = false);
    void Play(const string& key, float volume = 1.0f);
    void Stop(const string& key);
};
