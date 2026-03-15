#include "Managers.h"
#include <iostream>
#include "GUIClasses/GUI.h"

using namespace std;

/// Play a named music track, pausing any currently playing track
void MusicManager::Play(string pName) {
    assert(mResources.find(pName) != mResources.end());

    if (IsPlayingMusic()) {
        GetCurrentMusic().pause();
    }

    mPlayedMusics.push_back(pName);

    GetCurrentMusic().play();
    GetCurrentMusic().setVolume(mVolume);
}

/// Play a random unplayed track to establish volume
void MusicManager::PlayToDefineVolume() {
    bool finding = true;

    while (finding) {
        const int whichMusic = Gui::Rand(mResources.size(), 0);
        int i = 0;

        for (auto it = mResources.begin(); ; it++, i++) {
            if (i == whichMusic) {
                if (mPlayedMusics.end() == find(mPlayedMusics.begin(), mPlayedMusics.end(), (*it).first)) {
                    mPlayedMusics.push_back((*it).first);
                    finding = false;
                }

                break;
            }
        }
    }

    GetCurrentMusic().play();
    GetCurrentMusic().setVolume(mVolume);
}

/// Return reference to the currently active music track
sf::Music& MusicManager::GetCurrentMusic() {
    assert(IsPlayingMusic());

    return *mResources[mPlayedMusics.back()];
}

/// Stop and remove the most recent music track from the stack
void MusicManager::Stop() {
    if (IsPlayingMusic()) {
        GetCurrentMusic().stop();
        mPlayedMusics.pop_back();
    }
}

/// Pause the currently playing music
void MusicManager::Pause() {
    if (IsPlayingMusic()) {
        GetCurrentMusic().pause();
    }
}

/// Resume the currently paused music
void MusicManager::Resume() {
    if (IsPlayingMusic()) {
        GetCurrentMusic().play();
        GetCurrentMusic().setVolume(mVolume);
    }
}

/// Adjust volume up or down and apply to current music
void MusicManager::UpdateMusicVolume(bool pIsLouder) {
    if (!IsPlayingMusic()) {
        return;
    }

    UpdateVolume(pIsLouder);

    GetCurrentMusic().setVolume(mVolume);
}

/// Stop all tracked music and clear the played list
void MusicManager::ResetAllMusic() {
    for (auto musicName : mPlayedMusics) {
        mResources[musicName]->stop();
    }

    mPlayedMusics.clear();
}

///------------------------SoundManager-------------///

/// Play a named sound effect using the first available sound slot
void SoundManager::Play(string pName) {
    assert(mResources.find(pName) != mResources.end());

    for (auto& sound : mSounds) {
        if (sound.getStatus() == sf::Sound::Stopped) {
            sound.setBuffer(*mResources[pName]);
            sound.setVolume(mVolume);
            sound.play();
            return;
        }
    }

    cout << "----------------TOO MANY SOUNDS--------------" << endl;
    assert(0);
}

/// Stop all active sounds
void SoundManager::Stop() {
    for (auto& sound : mSounds) {
        sound.stop();
    }
}

/// Pause all currently playing sounds
void SoundManager::Pause() {
    for (auto& sound : mSounds) {
        if (sound.getStatus() == sf::Sound::Playing) {
            sound.pause();
        }
    }
}

/// Resume all paused sounds
void SoundManager::Resume() {
    for (auto& sound : mSounds) {
        if (sound.getStatus() == sf::Sound::Paused) {
            sound.play();
        }
    }
}
