#include "LoadingGameScen.h"

#include "../GUIClasses/GUI.h"
#include "GameScen.h"
#include "../MarioGame.h"

#include "../GameClasses/Blocks/Blocks.h"
#include "../GameClasses/Enemies/Enemies.h"
#include "../GameClasses/Items/Items.h"
#include "../GameClasses/Mario.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <windows.h>

const float LoadingGameScene::sLoadingDuration = 1.5f;

LoadingGameScene::LoadingGameScene(std::string pLevelName, const EnteringPipe* pEnteringPipe)
    : Scene("LoadingScen")
    , mLevelName(pLevelName)
    , mLandscapes(new std::vector<Landscape>)
    , mBlocks(new std::list<std::unique_ptr<Block>>)
    , mPhysicalObjs(new std::list<std::unique_ptr<PhysicalObject>>)
    , mNotPhysicalObjs(new std::list<std::unique_ptr<GameObject>>)
{
    StartLoadingLevel();

    if (pEnteringPipe)
        MarioGame::Instance().ChangeScene(new GameScene(mLevelTime, mMusicName, mBackgroundName, mLandscapes, mBlocks, mPhysicalObjs, mNotPhysicalObjs));
    else
    {
        Gui::SetWorldName(pLevelName);

        mMarioImage.reset(Gui::CreateSprite("LoadingMarioImage", {0, 0, 50, 90}, {0, 0}, 1.2f, true));
        mMarioImage->setPosition({MarioGame::sSizeWindow.x / 2.0f, MarioGame::sSizeWindow.y / 2.0f + mMarioImage->getGlobalBounds().height / 2.0f});

        ///--- TEXTS

        const sf::Vector2f posTexts = {mMarioImage->getPosition().x, mMarioImage->getPosition().y - mMarioImage->getGlobalBounds().height * 1.5f};

        mInformationAboutActivateMenu.reset(Gui::CreateText(mInformationHowActivateMenu, {posTexts.x, posTexts.y + MarioGame::sSizeWindow.y - 400}, sf::Color::White, "menu_font", true, 30));

        mLevelTextName.reset(Gui::CreateText(pLevelName, posTexts, sf::Color::White, "menu_font", true, 30));

        mNumberLives.reset(Gui::CreateText("x" + Gui::ToString(Mario::GetLivesMario()), {posTexts.x + 60, posTexts.y + mMarioImage->getGlobalBounds().height + 35}, sf::Color::White, "menu_font", true));
    }
}

void LoadingGameScene::StartLoadingLevel()
{
    /// LOAD BACKGROUND NAME
    mFile.open("Maps/" + mLevelName + "/" + mBackgroundFileName + ".txt", std::ios::in);
    assert(mFile.good());
    mFile >> mBackgroundName;

    mFile.close();

    /// LOAD MUSIC NAME
    mFile.open("Maps/" + mLevelName + "/" + mMusicFileName + ".txt", std::ios::in);
    assert(mFile.good());
    mFile >> mMusicName;

    mFile.close();

    /// LOAD LEVEL TIME

    mFile.open("Maps/" + MarioGame::Instance().GetCurrentLevelName() + "/" + mLevelTimeFileName + ".txt", std::ios::in);
    assert(mFile.good());
    mFile >> mLevelTime;

    assert(mLevelTime >= 60 && mLevelTime <= 240);

    mFile.close();

    /// LOAD GAME OBJECTS
    for (int i = 0; i < 4; i++)
    {
        std::string nameFile = "Maps/" + mLevelName + "/" + mKindsObjectsFilesNames[i] + ".txt";

        mFile.open(nameFile, std::ios::in);
        assert(mFile.good());

        mCurrentKindObject = static_cast<KindsObject>(i);

        LoadObjects();

        mFile.close();

    }

    SetAllPipes();
}

///------
void LoadingGameScene::DefineLoadingObject(int& pId, int& pHowMany)
{
    mFile >> pId;
    if (pId == 0) return;

    assert(pId >= 0 && pId <= 50000);

    if (pId > 100)
    {
        for (int i = 2000; i < 100000; i += 1000)
        if (pId < i)
        {
            pHowMany = pId - (i - 1000);
            pId = i / 1000;
            pId--;
            break;
        }
    }
}

///----------
void LoadingGameScene::LoadObjects()
{
    for (int y = 0; y < 15; ++y)
    {

    int id = 0, x = 0;

    while (true)
    {
        int howMany = 1;
        DefineLoadingObject(id, howMany);

        if (id == 0) break;

        switch (id)
        {
            case 1:
            {
                for (int i = 0; i < howMany; ++i)
                    x++;

                break;
            }

            default:
            {
                for (int i = 0; i < howMany; ++i, ++x)
                    CreateObject(id, sf::Vector2f(x * sTileSize, y * sTileSize));

                break;
            }
        }
    }

    }
}

///--------
void LoadingGameScene::SetAllPipes()
{
    for (auto pipe : mAllPipes)
        mBlocks->push_back(std::unique_ptr<Pipe>(pipe));

    if (mPotentialsEnteringPipes.size() == 0)
        return;

    mFile.open("Maps/" + mLevelName + "/" + mSettingSwitchingPipesFileName + ".txt", std::ios::in);

    if (mFile.good())
    {
        if (mAllPipes.size() >= 2)
            std::sort(mAllPipes.begin(), mAllPipes.end(), [](const Pipe* pLeftPipe, const Pipe* pRightPipe)
                {return pLeftPipe->GetCurrentPosition().x < pRightPipe->GetCurrentPosition().x; });

        auto whichEnteringPipe = mPotentialsEnteringPipes.begin();

        while (!mFile.eof() && whichEnteringPipe != mPotentialsEnteringPipes.end())
        {
            std::string undergroundLevelName;
            int returnPipe;

            mFile >> undergroundLevelName;
            mFile >> returnPipe;

            if (returnPipe == -1)
                mBlocks->push_back(std::unique_ptr<Block>(new EnteringPipe(*whichEnteringPipe, undergroundLevelName)));
            else
            if (returnPipe < 0 || returnPipe >= mAllPipes.size())
            {
                std::cout << " ---- INVALID DATA DEFINING PIPES" << std::endl;
                assert(0);
            } else
                mBlocks->push_back(std::unique_ptr<Block>(new EnteringPipe(*whichEnteringPipe, undergroundLevelName, mAllPipes[returnPipe])));

            whichEnteringPipe++;
        }
    } else
    {
        std::cout << "--- MISSING FILE DEFINING TRANSPORT PIPES -------" << std::endl;
        assert(0);
    }

    mFile.close();
}

///------
void LoadingGameScene::Update()
{
    if (sDurationScene >= sLoadingDuration)
        MarioGame::Instance().ChangeScene(new GameScene(mLevelTime, mMusicName, mBackgroundName, mLandscapes, mBlocks, mPhysicalObjs, mNotPhysicalObjs));
}

///-----
void LoadingGameScene::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    pWindow->draw(mBackground);
    pWindow->draw(*mLevelTextName);
    pWindow->draw(*mNumberLives);
    pWindow->draw(*mInformationAboutActivateMenu);
    pWindow->draw(*mMarioImage);
}

///-------
void LoadingGameScene::CreateObject(int pId, sf::Vector2f pPos)
{
    pPos = {pPos.x + Scene::sTileSize / 2.0f, pPos.y + Scene::sTileSize};

    switch (mCurrentKindObject)
    {
    case KindsObject::BLOCKS:
        {
            switch (pId)
            {

            case BLOCKS::Ground:
            mBlocks->push_back(std::unique_ptr<Block>(new StaticBlock({0, 0, 32, 32}, pPos))); break;

            case BLOCKS::DarkGround:
            mBlocks->push_back(std::unique_ptr<Block>(new StaticBlock({58, 477, 32, 32}, pPos))); break;

            case BLOCKS::Brick:
            mBlocks->push_back(std::unique_ptr<Block>(new DynamicBlock({32, 0, 32, 32}, pPos))); break;

            case BLOCKS::DarkBrick:
            mBlocks->push_back(std::unique_ptr<Block>(new DynamicBlock({15, 521, 32, 32}, pPos))); break;

            case BLOCKS::StaticBrick:
            mBlocks->push_back(std::unique_ptr<Block>(new StaticBlock({96, 0, 32, 32}, pPos))); break;

            case BLOCKS::DarkStaticBrick:
            mBlocks->push_back(std::unique_ptr<Block>(new StaticBlock({13, 477, 32, 32}, pPos))); break;

            case BLOCKS::Bridge:
            mBlocks->push_back(std::unique_ptr<Block>(new StaticBlock({224, 0, 32, 32}, pPos))); break;

            case BLOCKS::MetalBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new StaticBlock({32, 32, 32, 32}, pPos))); break;

            case BLOCKS::MysterBoxCoin:
            mBlocks->push_back(std::unique_ptr<Block>(new MysteryBox(pPos, MysteryBox::MyItem::COIN))); break;

            case BLOCKS::MysterBoxMushroom:
            mBlocks->push_back(std::unique_ptr<Block>(new MysteryBox(pPos, MysteryBox::MyItem::MUSHROOM))); break;

            case BLOCKS::BigPipe:
            mAllPipes.push_back(new Pipe(pPos, Pipe::KindPipe::BIG)); break;

            case BLOCKS::SmallPipe:
            mAllPipes.push_back(new Pipe(pPos, Pipe::KindPipe::SMALL)); break;

            case BLOCKS::EnteringBigPipe:
            mPotentialsEnteringPipes.push_back({pPos, Pipe::KindPipe::BIG}); break;

            case BLOCKS::EnteringSmallPipe:
            mPotentialsEnteringPipes.push_back({pPos, Pipe::KindPipe::SMALL}); break;

            case BLOCKS::ReturnPipeBlock:
            mBlocks->push_front(std::unique_ptr<Block>(new ReturnPipe(pPos))); break;

            case BLOCKS::LeftFieryBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new FieryBlock(pPos, false))); break;

            case BLOCKS::RightFieryBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new FieryBlock(pPos, true))); break;

            case BLOCKS::LeftMoveableBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new MoveableBlock(pPos, MoveableBlock::Direction::LEFT))); break;

            case BLOCKS::RightMoveableBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new MoveableBlock(pPos, MoveableBlock::Direction::RIGHT))); break;

            case BLOCKS::UpMoveableBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new MoveableBlock(pPos, MoveableBlock::Direction::UP))); break;

            case BLOCKS::DownMoveableBlock:
            mBlocks->push_back(std::unique_ptr<Block>(new MoveableBlock(pPos, MoveableBlock::Direction::DOWN))); break;

            }

            break;
        }

    case KindsObject::ENEMIES:
        {
            switch (pId)
            {

            case ENEMIES::EnemyGoombas:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new Goombas(pPos))); break;

            case ENEMIES::EnemyTurtle:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new Turtle(pPos))); break;

            case ENEMIES::EnemyRedTurtle:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new RedTurtle(pPos))); break;

            case ENEMIES::EnemyArmoredTurtle:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new ArmoredTurtle(pPos))); break;

            case ENEMIES::EnemyFlyingTurtle:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new FlyingTurtle(pPos))); break;

            case ENEMIES::EnemyArmedTurtle:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new ArmedTurtle(pPos))); break;

            case ENEMIES::EnemyBowser:
            mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(new Bowser(pPos))); break;

            case ENEMIES::EnemyCreatorRedTurtles:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new CreatorRedTurtles(pPos))); break;

            }

            break;
        }
    case KindsObject::ITEMS:
        {
            switch (pId)
            {
            case ITEMS::ItemCoin:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new Coin(pPos))); break;

            case ITEMS::ItemLineWithFlag:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new LineWithFlag(pPos))); break;

            case ITEMS::SmallGun:
            mBlocks->push_back(std::unique_ptr<Block>(new Gun(pPos, Gun::KindGun::SMALL))); break;

            case ITEMS::BigGun:
            mBlocks->push_back(std::unique_ptr<Block>(new Gun(pPos, Gun::KindGun::BIG))); break;

            case ITEMS::ItemTrampoline:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new Trampoline(pPos))); break;

            case ITEMS::ItemWaterWaves:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new WaterWaves(pPos))); break;

            case ITEMS::ItemLavaWaves:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new LavaWaves(pPos))); break;

            case ITEMS::ItemLavaBullet:
            mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(new LavaBullet(pPos))); break;

            }

            break;
        }

    case KindsObject::LANDSCAPES:
        {
            switch (pId)
            {

            case LANDSCAPES::LandscapeCloud:
            mLandscapes->push_back(Landscape(pPos, {82, 160, 65, 42})); break;

            case LANDSCAPES::BigTree:
            mLandscapes->push_back(Landscape(pPos, {0, 160, 32, 96}, 2)); break;

            case LANDSCAPES::SmallTree:
            mLandscapes->push_back(Landscape(pPos, {32, 160, 32, 32})); break;

            case LANDSCAPES::PinkFlower:
            mLandscapes->push_back(Landscape(pPos, {160, 225, 30, 30})); break;

            case LANDSCAPES::BlueFlower:
            mLandscapes->push_back(Landscape(pPos, {225, 225, 30, 30})); break;

            case LANDSCAPES::FirstHurdle:
            mLandscapes->push_back(Landscape(pPos, {224, 160, 32, 32})); break;

            case LANDSCAPES::SecondHurdle:
            mLandscapes->push_back(Landscape(pPos, {160, 175, 32, 16})); break;

            case LANDSCAPES::FirstBigBush:
            mLandscapes->push_back(Landscape(pPos, {1, 362, 97, 42})); break;

            case LANDSCAPES::SecondBigBush:
            mLandscapes->push_back(Landscape(pPos, {7, 414, 89, 45})); break;

            case LANDSCAPES::FirstSmallBush:
            mLandscapes->push_back(Landscape(pPos, {82, 225, 61, 30})); break;

            case LANDSCAPES::SecondSmallBush:
            mLandscapes->push_back(Landscape(pPos, {192, 262, 32, 26})); break;

            case LANDSCAPES::LandscapeWater:
            mLandscapes->push_back(Landscape(pPos, {32, 220, 32, 32})); break;

            case LANDSCAPES::LandscapeLava:
            mLandscapes->push_back(Landscape(pPos, {64, 320, 32, 32})); break;

            }

            break;
        }
    }
}
