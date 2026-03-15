#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <cassert>
#include <string>
#include <vector>
#include <stdexcept>

// Generic resource manager template for loading and retrieving assets
template<typename T>
class ResourceManager {
public:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager operator=(const ResourceManager&) = delete;
    virtual ~ResourceManager() {}

    virtual void LoadFromFile(std::string pName, const std::string& pFilePath);

    [[nodiscard]] const T& operator[](const std::string& pName);
    [[nodiscard]] const T& Get(const std::string& pName);

protected:
    std::map<std::string, std::unique_ptr<T>> mResources;
};

/// General resource loading (textures, fonts, etc.)
template<typename T>
void ResourceManager<T>::LoadFromFile(std::string pName, const std::string& pFilePath) {
    assert(mResources.find(pName) == mResources.end());
    mResources[pName] = std::unique_ptr<T>(new T);

    if (!mResources[pName]->loadFromFile(pFilePath)) {
        throw std::runtime_error(("Cannot load file: " + pFilePath).c_str());
    }
}

/// Specialization for sf::Music which uses openFromFile and loops by default
template<>
inline void ResourceManager<sf::Music>::LoadFromFile(std::string pName, const std::string& pFilePath) {
    assert(mResources.find(pName) == mResources.end());
    mResources[pName] = std::unique_ptr<sf::Music>(new sf::Music);

    if (!mResources[pName]->openFromFile(pFilePath)) {
        throw std::runtime_error(("Cannot load file: " + pFilePath).c_str());
    }

    mResources[pName]->setLoop(true);
}

/// Subscript operator for resource access by name
template<typename T>
const T& ResourceManager<T>::operator[](const std::string& pName) {
    assert(mResources.find(pName) != mResources.end());
    return (*mResources[pName]);
}

/// Named getter for resource access by name
template<typename T>
const T& ResourceManager<T>::Get(const std::string& pName) {
    assert(mResources.find(pName) != mResources.end());
    return (*mResources[pName]);
}

// Type aliases for common resource managers
using TextureManager = ResourceManager<sf::Texture>;
using FontManager = ResourceManager<sf::Font>;

// Controls volume level with increment/decrement support
class MusicVolumeController {
protected:
    float mVolume = 60;

public:
    [[nodiscard]] float GetVolume() const { return mVolume; }

    void UpdateVolume(bool pIsLouder) {
        if (pIsLouder) {
            if (mVolume < 100) {
                mVolume += 10;
            }
        } else {
            if (mVolume > 0) {
                mVolume -= 10;
            }
        }
    }
};

// Manages music playback with a stack of played tracks
class MusicManager : public ResourceManager<sf::Music>, public MusicVolumeController {
    std::vector<std::string> mPlayedMusics;

    [[nodiscard]] inline bool IsPlayingMusic() const { return mPlayedMusics.size() != 0; }
    sf::Music& GetCurrentMusic();

public:
    ~MusicManager() {}

    void Play(std::string pName);
    void Pause();
    void Resume();
    void Stop();
    void UpdateMusicVolume(bool pIsLouder);
    void PlayToDefineVolume();
    void ResetAllMusic();
};

// Manages sound effect playback using a pool of sound instances
class SoundManager : public ResourceManager<sf::SoundBuffer>, public MusicVolumeController {
    std::vector<sf::Sound> mSounds;

public:
    explicit SoundManager() : mSounds(25) {}
    ~SoundManager() {}

    void Play(std::string pName);
    void Stop();
    void Pause();
    void Resume();
};
