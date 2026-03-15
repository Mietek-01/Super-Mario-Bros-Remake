#pragma once

#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "../GameClasses/Landscape.h"
#include "../GameClasses/Blocks/Block.h"
#include "../GameClasses/PhysicalObject.h"
#include "../GameClasses/Blocks/SpecialPipe.h"

#include "Scen.h"

class LoadingGameScene : public Scene {
    enum class KindsObject {
        BLOCKS,
        ENEMIES,
        ITEMS,
        LANDSCAPES,
    } mCurrentKindObject;

    /// !!! IMPORTANT: MUST START FROM 2
    enum BLOCKS {
       Ground = 2,
       DarkGround,
       Brick,
       DarkBrick,
       StaticBrick,
       DarkStaticBrick,
       MysterBoxCoin,
       MysterBoxMushroom,
       Bridge,
       MetalBlock,
       LeftMoveableBlock,
       RightMoveableBlock,
       UpMoveableBlock,
       DownMoveableBlock,
       LeftFieryBlock,
       RightFieryBlock,
       SmallPipe,
       BigPipe,
       ReturnPipeBlock,
       EnteringBigPipe,

       EnteringSmallPipe
    };

    enum ENEMIES {
        EnemyGoombas = 2,
        EnemyTurtle,
        EnemyRedTurtle,
        EnemyArmoredTurtle,
        EnemyFlyingTurtle,
        EnemyArmedTurtle,
        EnemyCreatorRedTurtles,

        EnemyBowser
    };

    enum ITEMS {
       ItemCoin = 2,
       SmallGun,
       BigGun,
       ItemTrampoline,
       ItemWaterWaves,
       ItemLavaWaves,
       ItemLavaBullet,

       ItemLineWithFlag

    };

    enum LANDSCAPES {
        LandscapeCloud = 2,
        PinkFlower,
        BlueFlower,
        FirstSmallBush,
        SecondSmallBush,
        FirstBigBush,
        SecondBigBush,
        FirstHurdle,
        SecondHurdle,
        SmallTree,
        BigTree,
        LandscapeWater,

        LandscapeLava
    };

    const static float sLoadingDuration;

    std::unique_ptr<sf::Sprite> mMarioImage;

    std::unique_ptr<sf::Text> mLevelTextName;
    std::unique_ptr<sf::Text> mNumberLives;
    std::unique_ptr<sf::Text> mInformationAboutActivateMenu;

    const std::string mLevelName;
    const std::string mInformationHowActivateMenu = "PRESS \"ESC\" TO ACTIVATE MENU IN DURING GAME";
    const std::string mKindsObjectsFilesNames[4] = {"Blocks", "Enemies", "Items", "Landscape"};

    const std::string mMusicFileName = "Music name";
    const std::string mBackgroundFileName = "Background name";
    const std::string mLevelTimeFileName = "Lvl time";
    const std::string mSettingSwitchingPipesFileName = "Underground levels names";

    std::fstream mFile;

    /// DATA TO LOAD
    std::string mBackgroundName;
    std::string mMusicName;
    int mLevelTime;

    std::shared_ptr<std::vector<Landscape>> mLandscapes;
    std::shared_ptr<std::list<std::unique_ptr<Block>>> mBlocks;
    std::shared_ptr<std::list<std::unique_ptr<GameObject>>> mNotPhysicalObjs;
    std::shared_ptr<std::list<std::unique_ptr<PhysicalObject>>> mPhysicalObjs;

    std::vector<std::pair<sf::Vector2f, Pipe::KindPipe>> mPotentialsEnteringPipes;
    std::vector<Pipe*> mAllPipes;

    ///-- METHODS
    void StartLoadingLevel();
    void DefineLoadingObject(int& pId, int& pHowMany);
    void LoadObjects();
    void CreateObject(int pId, sf::Vector2f pPos);
    void SetAllPipes();

public:

    LoadingGameScene(std::string pLevelName, const EnteringPipe* pEnteringPipe = nullptr);
    ~LoadingGameScene() {}

    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;
    void Update() override;
};
