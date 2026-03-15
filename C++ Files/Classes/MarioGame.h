#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Managers.h"
#include "Scens/Scen.h"
#include "EventHandler.h"
#include "GUIClasses/GUI.h"

class MarioGame final {
    static MarioGame* sInstance;

    const std::string mTitle = "MARIO";
    const bool mIsFullScreen = false;
    const bool mIsWriteFramerate = false;

    std::unique_ptr<Scene> mCurrentScene;
    std::unique_ptr<sf::RenderWindow> mWindow;

    std::unique_ptr<EventHandler> mEventHandler;
    std::unique_ptr<Gui> mGui;

    const std::vector<std::string> mLevelsNames;
    const int mFramerate = 110;
    int mCurrentLevelIndex = 0;
    int mCompletedLevelsNumber;

    /// METHODS

    MarioGame();
    ~MarioGame();

    std::vector<std::string> LoadLevelsNames();
    void LoadCompletedLevelsNumber();
    void InitEvents();
    void CreateWindow();
    void WriteFrameRate(int& pFrameCounter, float& pSecondTimer, float pFrameTime);

    static void SetManagers();

public:
    static const sf::Vector2u sSizeWindow;

    static FontManager sFontManager;
    static TextureManager sTextureManager;
    static MusicManager sMusicManager;
    static SoundManager sSoundManager;

    /// METHODS

    static MarioGame& Instance();

    template <class T>
    [[nodiscard]] bool IsThisScene() const {
        return (bool)(dynamic_cast<T*>(mCurrentScene.get()));
    }

    void Run();
    void ChangeScene(Scene* pNewScene) { mCurrentScene.reset(pNewScene); }
    void ResetView();

    /// SETTERS

    void SetIndexLevel(int pWhichLevel) {
        assert(pWhichLevel < mLevelsNames.size() && pWhichLevel >= 0);
        mCurrentLevelIndex = pWhichLevel;
    }
    void SetNextLevel();
    void IncreaseCompletedLevelsNumber() { mCompletedLevelsNumber++; }

    /// GETTERS

    template <class T>
    [[nodiscard]] T& GetScene() const {
        assert(mCurrentScene);
        assert(IsThisScene<T>());

        return *(dynamic_cast<T*>(mCurrentScene.get()));
    }

    [[nodiscard]] const std::unique_ptr<sf::RenderWindow>& GetWindow() const { return mWindow; }
    [[nodiscard]] sf::Vector2f GetViewPosition() const { return mWindow->getView().getCenter(); }

    [[nodiscard]] std::string GetCurrentLevelName() const { return mLevelsNames[mCurrentLevelIndex]; }
    [[nodiscard]] const std::vector<std::string>& GetLevelsNames() const { return mLevelsNames; }

    [[nodiscard]] int GetCompletedLevelsNumber() const { return mCompletedLevelsNumber; }
    [[nodiscard]] int GetLevelIndex() const { return mCurrentLevelIndex; }

    [[nodiscard]] bool IsLastLevel() const { return mCurrentLevelIndex == mLevelsNames.size() - 1; }
};
