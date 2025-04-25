#include "FModSoundPlayer.h"
#include <iostream>
#include <fmod.hpp>
#include <fmod_errors.h>

void FModSoundPlayer::Init()
{
    FMOD::System_Create(&fmodSystem); // �ý��� ����
    fmodSystem->init(128, FMOD_INIT_NORMAL, nullptr); // �ʱ�ȭ

    //soundEffects
    {
        LoadSound("step", "Sounds/Sfx/step.mp3", false);
        LoadSound("descend", "Sounds/Sfx/descend.mp3", false);
    }
    //bmg
    {
        LoadSound("sewers_1", "Sounds/Bgm/sewers_1.ogg", true);
    }
}

void FModSoundPlayer::Update()
{
    timer += TimerManager::GetInstance()->GetDeltaTime();
    if (fmodSystem) fmodSystem->update();

    
    if (timer >= 1.f)
    {
        for (auto it = channels.begin(); it != channels.end(); ) {
            bool isPlaying = false;
            it->second->isPlaying(&isPlaying);
            if (!isPlaying) {
                it->second->stop();
                it = channels.erase(it);
            }
            else {
                ++it;
            }
        }
        timer = 0;
    }
}

void FModSoundPlayer::Release()
{
    for (auto& s : sounds)
        s.second->release();

    sounds.clear();
    fmodSystem->close();
    fmodSystem->release();
}

void FModSoundPlayer::LoadSound(const string& key, const char* filepath, bool loop)
{
    FMOD_MODE mode = FMOD_DEFAULT | FMOD_CREATESAMPLE;
    if (loop) mode |= FMOD_LOOP_NORMAL;
    else mode |= FMOD_LOOP_OFF;

    FMOD::Sound* sound = nullptr;
    fmodSystem->createSound(filepath, mode, nullptr, &sound);
    sounds[key] = sound;
}

void FModSoundPlayer::Play(const string& key, float volume)
{
    auto it = sounds.find(key);
    if (it == sounds.end()) return;

    FMOD::Channel* channel = nullptr;
    auto res = fmodSystem->playSound(it->second, nullptr, false, &channel);
    if (channel) {
        channel->setVolume(volume);

        // ������ ��带 Ȯ���Ͽ� ���� ������ ��쿡�� channels�� �����մϴ�.
        FMOD_MODE mode;
        it->second->getMode(&mode);
        if (mode & FMOD_LOOP_NORMAL) { // ���� ������ ������ ���
            channels.insert({ key, channel });
        }
    }
}

void FModSoundPlayer::Stop(const string& key)
{
    auto it = channels.find(key);
    if (it != channels.end() && it->second) {
        it->second->stop();
        channels.erase(it);
    }
}