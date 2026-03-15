#include <iostream>
#include <cassert>

#include "MarioGame.h"
#include "Scens/GameScen.h"
#include "GameClasses/Blocks/MysterBox.h"
#include "GameClasses/Items/Coin.h"
#include "GameClasses/Items/WaterWaves.h"
#include "GameClasses/Items/LavaWaves.h"
#include "GameClasses/Blocks/SpecialPipe.h"
#include "Scens/MenuScen.h"
#include <fstream>
#include <ctime>

using namespace std;

MarioGame* MarioGame::sInstance = nullptr;
const sf::Vector2u MarioGame::sSizeWindow{1280, 720};

FontManager MarioGame::sFontManager;
TextureManager MarioGame::sTextureManager;
MusicManager MarioGame::sMusicManager;
SoundManager MarioGame::sSoundManager;

MarioGame& MarioGame::Instance() {
    if (!sInstance)
        sInstance = new MarioGame();

    return *sInstance;
}

///------
MarioGame::~MarioGame() {
    /// Save completed levels number
    fstream file;

    file.open("Maps/Completed_lvls_number.txt", ios::out);
    file << mCompletedLevelsNumber;

    mCurrentScene.reset(); /// Important!

    sInstance = nullptr;
}

///------
MarioGame::MarioGame()
    : mLevelsNames(LoadLevelsNames()) {
    srand(time(NULL));

    LoadCompletedLevelsNumber();

    /// Set up static resources
    MarioGame::SetManagers();

    MysteryBox::setStaticAnimation();
    Coin::setStaticAnimation();
    WaterWaves::setStaticAnimation();
    LavaWaves::setStaticAnimation();

    ///---
    mGui.reset(new Gui);
    mEventHandler.reset(new EventHandler);

    ///----
    InitEvents();
    CreateWindow();

    mCurrentScene.reset(new MenuScene());
}

///------
void MarioGame::InitEvents() {
    /// The only event is activating the menu during gameplay
    std::function<void()> pausedMenuAction = []() {
        if (!(MarioGame::Instance().IsThisScene<GameScene>()) || Gui::IsMenuActive())
            return;

        MarioGame::sMusicManager.pause();
        MarioGame::sSoundManager.pause();

        sf::Text* name;

        const sf::Vector2f centerPos(MarioGame::Instance().GetWindow()->getView().getCenter());

        const sf::IntRect boundsMenu(centerPos.x, centerPos.y, 300, 400);

        sf::Vector2f posLabel(centerPos.x, boundsMenu.top - boundsMenu.height / 2.0f + Menu::sValueMargin * 2.5);

        vector<Label*> labels;

        /// Labels

        ///- RESUME
        name = Gui::CreateText("REASUME", posLabel, sf::Color(250, 250, 255, 100), "menu_font");
        labels.push_back(new Label(name, [](int pAscii) {
            Gui::RemoveCurrentMenu();
            MarioGame::sMusicManager.resume();
            MarioGame::sSoundManager.resume();
        }));

        ///- OPTIONS
        posLabel.y += Menu::sValueMargin * 2;

        labels.push_back(Menu::CreateOptionLabel(posLabel, centerPos, sf::Color(250, 250, 255, 100)));

        ///- BACK TO MAIN MENU
        posLabel.y += Menu::sValueMargin * 2;

        name = Gui::CreateText("BACK TO MENU", posLabel, sf::Color(250, 250, 255, 100), "menu_font");
        labels.push_back(new Label(name, [](int pAscii) {
            Gui::AddMenu(Gui::CreateAreYouSureMenu([](int pAscii) {
                Gui::RemoveAllMenus();

                MarioGame::Instance().ChangeScene(new MenuScene());
                EnteringPipe::resetActivatedPipes();
                Mario::resetLvlMario();

                MarioGame::sMusicManager.resetAllMusic();
                MarioGame::sSoundManager.stop();
            }));
        }));

        sf::RectangleShape* background = Gui::CreateRectangleShape(boundsMenu, sf::Color::Black, true, true);
        Gui::AddMenu(new Menu(true, labels, background, "PAUSED", true));
    };

    mEventHandler->addEvent(EventHandler::Event::PausedMenu, pausedMenuAction);
}

///---
void MarioGame::ResetView() {
    mWindow->setView(sf::View(sf::FloatRect(0, 0, sSizeWindow.x, sSizeWindow.y)));
}

///----
void MarioGame::CreateWindow() {
    if (!mIsFullScreen)
        mWindow.reset(new sf::RenderWindow(sf::VideoMode({sSizeWindow.x, sSizeWindow.y}), mTitle, sf::Style::Titlebar | sf::Style::Close));
    else {
        mWindow.reset(new sf::RenderWindow(sf::VideoMode({sSizeWindow.x, sSizeWindow.y}), mTitle, sf::Style::Fullscreen));
        mWindow->setMouseCursorVisible(false);
    }

    mWindow->setFramerateLimit(mFramerate);
    mWindow->setKeyRepeatEnabled(false);

    sf::Image iconImage;
    iconImage.loadFromFile("res/Textures/MarioIcon.png");
    mWindow->setIcon(25, 34, iconImage.getPixelsPtr());
}

///-----
void MarioGame::Run() {
    sf::Time timer = sf::Time::Zero;
    sf::Clock clock;

    int frameCounter = 0;
    float secondTimer = 0;

    while (mWindow->isOpen()) {
        timer = clock.restart();
        mEventHandler->handleEvents(mWindow);

        if (mIsWriteFramerate)
            WriteFrameRate(frameCounter, secondTimer, timer.asSeconds());

        ///--- DRAW
        mWindow->clear();

        mCurrentScene->draw(mWindow);
        mGui->draw(mWindow);

        ///----- UPDATE
        if (!Gui::IsMenuActive()) {
            Scene::UpdateFrameTime(timer.asSeconds());

            mCurrentScene->update();
            mGui->update();
        }

        mWindow->display();
    }

    delete this;
}

///--------------
void MarioGame::SetManagers() {
    const string marioResPath = "res/";

    /// LOAD TEXTURES
    const string texturesDir = marioResPath + "Textures/";
    for (string texture : {"Menumshr", "MarioIcon", "LoadingMarioImage", "Mario_left", "Mario_right", "Tiles", "AnimTiles", "Enemies_left", "Enemies_right", "Bowser_left", "Bowser_right", "Items", "TheEndLvlLine", "Castle", "Barse"})
        sTextureManager.LoadFromFile(texture, texturesDir + texture + ".png");

    for (string texture : {"Sky", "LoadingScen", "Underground", "MenuBackground", "Water"})
        sTextureManager.LoadFromFile(texture, texturesDir + "backgrounds\\" + texture + ".png");

    /// LOAD SOUNDS
    const std::string soundsDir = marioResPath + "Sounds/";
    for (string sound : {"breakblock", "bump", "coin", "fireball", "jump_super", "kick", "stomp", "powerup_appears",
                         "powerup", "pipe", "flagpole", "bowser_falls", "bowser_fire", "mario_die", "stage_clear", "game_over", "1-up", "warning", "world_clear"})
        sSoundManager.LoadFromFile(sound, soundsDir + sound + ".wav");

    /// LOAD MUSICS
    const std::string musicDir = marioResPath + "Music/";
    for (string music : {"overworld", "underworld", "bowsercastle", "underwater", "invincibility"})
        sMusicManager.LoadFromFile(music, musicDir + music + ".ogg");

    /// LOAD FONTS
    const std::string fontsDir = marioResPath + "Fonts/";
    for (string font : {"arial", "menu_font", "main_font", "score_font", "some_font"})
        sFontManager.LoadFromFile(font, fontsDir + font + ".ttf");
}

///-----------
void MarioGame::SetNextLevel() {
    if (!IsLastLevel())
        mCurrentLevelIndex++;
}

///------
std::vector<string> MarioGame::LoadLevelsNames() {
    fstream file;

    std::vector<string> levelsNames;
    file.open("Maps/Lvls_names.txt", ios::in);

    if (!file.good())
        throw runtime_error("-----------CANNOT LOAD MAPS!!!----------");

    string name;
    int numberLines = 1;

    while (getline(file, name)) {
        if (numberLines == 6 || name.size() == 0)
            continue;

        levelsNames.push_back(name);
        numberLines++;
    }

    return levelsNames;
}

///----
void MarioGame::LoadCompletedLevelsNumber() {
    fstream file;

    file.open("Maps/Completed_lvls_number.txt", ios::in);

    if (file.good()) {
        file >> mCompletedLevelsNumber;

        if (mCompletedLevelsNumber > mLevelsNames.size() || mCompletedLevelsNumber < 0)
            mCompletedLevelsNumber = 0;
    } else
        mCompletedLevelsNumber = 0;
}

///----
void MarioGame::WriteFrameRate(int& pFrameCounter, float& pSecondTimer, float pFrameTime) {
    if (pSecondTimer >= 1) {
        cout << pFrameCounter << endl;
        pFrameCounter = 0;
        pSecondTimer = 0;
    } else {
        pSecondTimer += pFrameTime;
        pFrameCounter++;
    }
}
