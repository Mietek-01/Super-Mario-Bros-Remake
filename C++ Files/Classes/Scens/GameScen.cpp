#include "GameScen.h"
#include "LoadingGameScen.h"
#include "MenuScen.h"

#include "../GameClasses/Items/Coin.h"
#include "../GameClasses/Items/WaterWaves.h"
#include "../GameClasses/Items/LavaWaves.h"
#include "../GameClasses/Blocks/MysterBox.h"
#include "../GameClasses/Blocks/MoveableBlock.h"

#include "../GUIClasses/FlowText.h"
#include "../GUIClasses/GUI.h"
#include "../MarioGame.h"
#include "../GUIClasses/SpecialEffects.h"

#include <algorithm>
#include <functional>
#include <string>

int GameScene::mPoints = 0;

///------------------
GameScene::GameScene(int pLevelTime, std::string pMusicName, std::string pBackgroundName,
    std::shared_ptr<std::vector<Landscape>> pLandscapes,
    std::shared_ptr<std::list<std::unique_ptr<Block>>> pBlocks,
    std::shared_ptr<std::list<std::unique_ptr<PhysicalObject>>> pPhysicalObjs,
    std::shared_ptr<std::list<std::unique_ptr<GameObject>>> pNotPhysicalObjs)
    : Scene(pBackgroundName)
    , mView(sf::FloatRect(0, 0, MarioGame::sSizeWindow.x, MarioGame::sSizeWindow.y))
    , mMyRemovedObjects(GameObject::GetRemovedObjects())
    , mMyNewObjects(GameObject::GetNewObjects())
    , mWhenActivateWarning(pLevelTime / 4.0f)
    , mGameTime(pLevelTime)
    , mMario(new Mario(sf::Vector2f(sTileSize * 2.5f, MarioGame::sSizeWindow.y - (sTileSize * 2))))
    , mTextureBackgroundName(pBackgroundName)
{
    /// ASSIGN CREATED OBJECTS
    mLandscapes = pLandscapes;
    mBlocks = pBlocks;
    mPhysicalObjs = pPhysicalObjs;
    mNotPhysicalObjs = pNotPhysicalObjs;

    /// SET VIEW BOUNDS
    mBoundsView.left -= static_cast<int>(sTileSize * 2);
    mBoundsView.top -= static_cast<int>(sTileSize * 10);
    mBoundsView.width = MarioGame::sSizeWindow.x + sTileSize * 4;
    mBoundsView.height = MarioGame::sSizeWindow.y + sTileSize * 20;

    MarioGame::sMusicManager.play(pMusicName);

    UpdateNewGameObjects();
}

///------
GameScene::~GameScene()
{
    for (auto obj : mMyRemovedObjects) delete obj;
        mMyRemovedObjects.clear();

    Gui::SetPositionMainLabels(MarioGame::sSizeWindow.x / 2.0f);
    Gui::ResetGuiObjects();
    Gui::SetVisibleMainLabels(true);

    MarioGame::Instance().ResetView();

    MarioGame::sSoundManager.stop();
}

///------
void GameScene::SetGameState(GameStates pState)
{
    mGameState = pState;

    switch (pState)
    {
    ///----
    case GameStates::GameOver:
    {
        SetTimer(mGameoverLengthTime);

        MarioGame::sSoundManager.play("game_over");
        this->CreateExtinguishingEffect("GAME OVER");

        break;
    }

    ///----
    case GameStates::WinGame:
        {
            SetTimer(mWinGameLengthTime);

            MarioGame::sMusicManager.resetAllMusic();
            this->CreateExtinguishingEffect("YOU WIN !!!");
            Mario::ResetLevelMario();
            break;
        }
    }
}

///------------
bool GameScene::SetGamePlayState(GamePlayStates pState)
{
    if (IsMarioDead())
        return false;

    if (mGamePlayState == GamePlayStates::LevelCompleted)
        return false;

    if (mGamePlayState == GamePlayStates::BowserDefeat)
        return false;

    if (mGamePlayState == GamePlayStates::ChangingLevelMario && pState != GamePlayStates::MainGame)
        return false;

    ///------------//

    if (mGamePlayState == GamePlayStates::ChangingLevelMario)
        sDurationScene -= mMario->GetHowLongChangingLevel();

    mGamePlayState = pState;

    switch (pState)
    {
    ///-----
    case GamePlayStates::EnteringToPipe:
    {
        SetTimer(mEnteringToPipeLengthTime);
        MarioGame::sMusicManager.pause();
        MarioGame::sSoundManager.play("pipe");

        break;
    }

        ///-----
    case GamePlayStates::LeavingUndergroundWorldByPipe:
    {
        SetTimer(mEnteringToPipeLengthTime);
        MarioGame::sMusicManager.stop();
        MarioGame::sSoundManager.play("pipe");

        break;
    }

    ///-----
    case GamePlayStates::MarioDead:
    {
        Mario::CreateDeathAnimation(mMario->GetCurrentPosition());

        SetTimer(mMarioDeadLengthTime);
        MarioGame::sMusicManager.resetAllMusic();
        MarioGame::sSoundManager.play("mario_die");

        break;
    }

    ///----
    case GamePlayStates::BowserDefeat:
    {
        SetTimer(mBowserDefeatLengthTime);
        MarioGame::sMusicManager.resetAllMusic();

        mBowserIsDying = true;
        break;
    }

    ///---
    case GamePlayStates::FightWithBowser:
    {
        SetDoubleBoundsView();
        Gui::SetVisibleMainLabels(false);

        break;
    }

    ///-----
    case GamePlayStates::Warning:
    {
        MarioGame::sMusicManager.pause();
        MarioGame::sSoundManager.play("warning");
        SetTimer(mWarningLengthTime);

        mBeforeWarning = false;

        break;
    }

    ///----
    case GamePlayStates::LevelCompleted:
    {
        MarioGame::sMusicManager.resetAllMusic();
        MarioGame::sSoundManager.play("stage_clear");
        MarioGame::sSoundManager.play("flagpole");

        Mario::ResetLevelMario();
        Mario::ResetLivesMario();

        break;
    }

    }

    return true;
}

///----------------------
void GameScene::Update()
{
    switch (mGameState)
    {

    ///--------------------------
    case GameStates::Gameplay:
    {
        switch (mGamePlayState)
        {
        ///------
        case GamePlayStates::MainGame:
        {
            UpdateGameTime();

            GameUpdate();

            if (mBeforeWarning)
            {
                if (mGameTime <= mWhenActivateWarning)
                    GameScene::SetGamePlayState(GamePlayStates::Warning);
            }
            else if (mGameTime <= 0)
            {
                if (SetGamePlayState(GameScene::GamePlayStates::MarioDead))
                    mMario.reset();
            }

            break;
        }

        ///-----
        case GamePlayStates::Warning:
        {
            UpdateGameTime();

            GameUpdate();

            if (mGamePlayState == GamePlayStates::Warning)
            {
                if (mTimer < Scene::sDurationScene)
                {
                    MarioGame::sMusicManager.resume();

                    SetGamePlayState(GamePlayStates::MainGame);
                }

            } else
                MarioGame::sMusicManager.resume();

            break;
        }

        ///-----
        case GamePlayStates::MarioDead:
        {
            if (mTimer < Scene::sDurationScene)
            {
                if (Mario::GetLivesMario() <= 0)
                    GameScene::SetGameState(GameStates::GameOver);
                else
                {
                    RemoveNewGameObjects();
                    MarioGame::Instance().ChangeScene(new LoadingGameScene(MarioGame::Instance().GetCurrentLevelName()));
                    EnteringPipe::ResetActivatedPipes();
                    return;
                }
            } else
                GameUpdate();

            break;
        }

        ///----
        case GamePlayStates::EnteringToPipe:
        {
            if (mTimer < Scene::sDurationScene)
                EnteringPipe::GetActivatePipe()->SetUndergroundLevel();
            else
                EnteringPipe::GetActivatePipe()->EnteringAnimation();

            break;
        }

        ///----
        case GamePlayStates::LeavingUndergroundWorldByPipe:
        {
            if (mTimer < Scene::sDurationScene)
                EnteringPipe::GetActivatePipe()->SetBeforeWorld();
            else
                EnteringPipe::GetActivatePipe()->EnteringAnimation();

            break;
        }

        ///----
        case GamePlayStates::ReturnToBeforeWorld:
        {
            if (EnteringPipe::GetActivatePipe()->IsTheEndEnteringAnimation())
                EnteringPipe::GetActivatePipe()->ResumeGame();
            else
               EnteringPipe::GetActivatePipe()->EnteringAnimation();

            break;
        }

        ///----
        case GamePlayStates::ChangingLevelMario:
        {
            mMario->ChangingLevel();
            break;
        }

        ///-------
        case GamePlayStates::LevelCompleted:
        {
            if (mMarioInCastle)
            {
                switch (mLevelCompletedState)
                {
                case LevelCompletedStates::CountingPointsForLevelTime:
                {
                    const int isMinute = mGameTime - 1;
                    mGameTime -= 18.0f * sFrameTime;

                    if (mGameTime <= 0)
                    {
                        mLevelCompletedState = LevelCompletedStates::CountingPointsForLivesMario;
                        mGameTime = 0;
                        SetTimer(0.3f);
                    }

                    Gui::SetTime(mGameTime);

                    if ((int)mGameTime == isMinute)
                        this->AddPoints(50);

                    break;
                }

                case LevelCompletedStates::CountingPointsForLivesMario:
                {
                    if (mTimer < Scene::sDurationScene)
                    {
                        if (Mario::GetLivesMario() - mNumberLivesToCount > 0)
                        {
                            SetTimer(0.3f);
                            MarioGame::sSoundManager.play("kick");
                            AddPoints(1000);
                            mNumberLivesToCount++;
                        }
                        else
                        {
                            mNumberLivesToCount = 0;
                            SetTimer(2.0f);
                            mLevelCompletedState = LevelCompletedStates::NextLevel;
                        }
                    }

                    break;
                }

                case LevelCompletedStates::NextLevel:
                {
                    if (mTimer < Scene::sDurationScene)
                    {
                        if (MarioGame::Instance().IsLastLevel())
                            SetGameState(GameStates::WinGame);
                        else
                        {
                            if (MarioGame::Instance().GetCompletedLevelsNumber() == MarioGame::Instance().GetLevelIndex())
                                MarioGame::Instance().IncreaseCompletedLevelsNumber();

                            RemoveNewGameObjects();

                            MarioGame::Instance().SetNextLevel();
                            MarioGame::Instance().ChangeScene(new LoadingGameScene(MarioGame::Instance().GetCurrentLevelName()));
                            EnteringPipe::ResetActivatedPipes();

                            return;
                        }
                    }

                    break;
                }
                }
            }

            GameUpdate();
            break;
        }

        ///--------
        case GamePlayStates::BowserDefeat:
        {
            if (mBowserIsDying)
            {
                if (mTimer < Scene::sDurationScene)
                {
                    mPhysicalObjs->clear();
                    mNotPhysicalObjs->clear();

                    SetTimer(1.0f);
                    mBowserIsDying = false;

                    MarioGame::sSoundManager.play("bowser_falls");
                }
                else if (mTimer - 0.1f > Scene::sDurationScene && mSoundsTimer < Scene::sDurationScene)
                {
                    MarioGame::sSoundManager.play("bump");
                    mSoundsTimer = Scene::sDurationScene + 0.4f;
                }

            }
            else
            {
                if (mMario)
                {
                    if (mTimer < Scene::sDurationScene)
                    {
                        MarioGame::sSoundManager.play("world_clear");
                        Gui::SetVisibleMainLabels(true);

                        mMario.reset();
                    }
                }
                else GameUpdate();
            }

            break;
        }

        ///-----
        case GamePlayStates::FightWithBowser:
        {
            GameUpdate();
            break;
        }

        }

        break;
    }

    ///--------------------------
    case GameStates::GameOver:
    {
        if (mTimer < Scene::sDurationScene)
        {
            RemoveNewGameObjects();
            MarioGame::Instance().ChangeScene(new MenuScene());
            EnteringPipe::ResetActivatedPipes();
            return;

        }
        else
            GameUpdate();

        break;
    }

    ///--------------------------
    case GameStates::WinGame:
    {
        if (mTimer < Scene::sDurationScene)
        {
            RemoveNewGameObjects();
            MarioGame::Instance().ChangeScene(new MenuScene());
            EnteringPipe::ResetActivatedPipes();
            return;
        }

        break;
    }
    }
}

///-------------------------
inline std::map<GameObject::KindCollision, Block*> GameScene::HowCollisionsWithBlocks(const PhysicalObject* pObj, bool& pNoneCollision)
{
    std::map<GameObject::KindCollision, Block*> collisions;
    collisions[GameObject::KindCollision::Top] = nullptr;
    collisions[GameObject::KindCollision::Bottom] = nullptr;
    collisions[GameObject::KindCollision::RightSide] = nullptr;
    collisions[GameObject::KindCollision::LeftSide] = nullptr;

    for (const auto& block : *mBlocks)
    {
        if (!block->IsVisible()) continue;

        const GameObject::KindCollision howCollision = pObj->HowCollision(*block);

        if (howCollision != GameObject::KindCollision::None)
        {
            pNoneCollision = false;

            switch (howCollision)
            {
                case GameObject::KindCollision::Top:
                {
                   if (!collisions[GameObject::KindCollision::Top])
                        collisions[GameObject::KindCollision::Top] = block.get();
                   else
                    {
                        if (block->IsHit())
                            break;
                        else
                        if (collisions[GameObject::KindCollision::Top]->IsHit())
                            collisions[GameObject::KindCollision::Top] = block.get();
                        else
                            collisions[GameObject::KindCollision::Top] = WhichHitBlock(collisions[GameObject::KindCollision::Top], block.get(), pObj);
                    }

                   break;
                }

                case GameObject::KindCollision::Bottom:
                {
                   if (pObj->IsInBottomCollision() && !(block->IsHit() || block->IsDead())) break;

                   if (!collisions[GameObject::KindCollision::Bottom])
                        collisions[GameObject::KindCollision::Bottom] = block.get();
                   else
                        collisions[GameObject::KindCollision::Bottom] = WhichHitBlock(collisions[GameObject::KindCollision::Bottom], block.get(), pObj);

                   break;
                }

                case GameObject::KindCollision::RightSide:
                {
                    if (!collisions[GameObject::KindCollision::RightSide])
                        collisions[GameObject::KindCollision::RightSide] = block.get();

                    break;
                }

                case GameObject::KindCollision::LeftSide:
                {
                    if (!collisions[GameObject::KindCollision::LeftSide])
                        collisions[GameObject::KindCollision::LeftSide] = block.get();

                    break;
                }

            }
        }
    }

    return collisions;
}

///-----------------
inline void GameScene::PhysicalObjBlockCollision()
{
    for (auto& obj : *mPhysicalObjs)
    {
        if (!obj->IsVisible()) continue;

        bool noneCollision = true;
        std::map<GameObject::KindCollision, Block*> collisionsWithBlocks = HowCollisionsWithBlocks(obj.get(), noneCollision);
        obj->PhysicGameWithBlock(collisionsWithBlocks, noneCollision);
    }
}

///--------------------
inline void GameScene::MarioBlockCollision()
{
    bool noneCollision = true;
    std::map<GameObject::KindCollision, Block*> collisionsWithBlocks = HowCollisionsWithBlocks(mMario.get(), noneCollision);
    mMario->PhysicGameWithBlock(collisionsWithBlocks, noneCollision);
}

///-----------------
inline void GameScene::MarioGameObjsCollision()
{
    for (auto& obj : *mNotPhysicalObjs)
    {
        if (!obj->IsVisible()) continue;

        const GameObject::KindCollision howCollision = mMario->HowCollision(*obj);

        if (howCollision != GameObject::KindCollision::None)
            obj->ActOnObject(mMario.get(), howCollision);
    }

    ///-------
    for (auto& obj : *mPhysicalObjs)
    {
        if (!obj->IsVisible()) continue;

        const GameObject::KindCollision howCollision = mMario->HowCollision(*obj);

        if (howCollision != GameObject::KindCollision::None)
            obj->ActOnObject(mMario.get(), howCollision);
    }
}

///------------

inline void GameScene::GameObjsGameObjsCollisions()
{
    /// PHYSICAL WITH PHYSICAL
    for (auto& firstPhyObj : *mPhysicalObjs)
    {
        if (!firstPhyObj->IsVisible()) continue;

        for (auto& secondPhyObj : *mPhysicalObjs)
        {
            if (!secondPhyObj->IsVisible()) continue;

            if (firstPhyObj != secondPhyObj)
            {
               GameObject::KindCollision howCollision = secondPhyObj->HowCollision(*firstPhyObj);

               if (howCollision != GameObject::KindCollision::None)
                   firstPhyObj->ActOnObject(secondPhyObj.get(), howCollision);
            }
        }
    }

    /// PHYSICAL WITH NON-PHYSICAL
    for (auto& phyObj : *mPhysicalObjs)
    {
        if (!phyObj->IsVisible()) continue;

        for (auto& notPhyObj : *mNotPhysicalObjs)
        {
            if (!notPhyObj->IsVisible()) continue;

            GameObject::KindCollision howCollision = phyObj->HowCollision(*notPhyObj);

            if (howCollision != GameObject::KindCollision::None)
                notPhyObj->ActOnObject(phyObj.get(), howCollision);

            /// REACTION WORKS BOTH WAYS
            howCollision = notPhyObj->HowCollision(*phyObj);

            if (howCollision != GameObject::KindCollision::None)
                phyObj->ActOnObject(notPhyObj.get(), howCollision);
        }
    }
}

///------------------------
void GameScene::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    pWindow->setView(mView);
    pWindow->draw(mBackground);

    for (auto& obj : *mBlocks)
    if (IsObjectVisible(obj->GetBounds(), true))
    {
        obj->SetVisible(true);
        obj->Draw(pWindow);

    } else obj->SetVisible(false);

    for (auto& obj : *mLandscapes)
    if (IsObjectVisible(obj.GetBounds()))
        obj.Draw(pWindow);

    for (auto& obj : *mNotPhysicalObjs)
    if (IsObjectVisible(obj->GetBounds()))
    {
        obj->SetVisible(true);
        obj->Draw(pWindow);

    } else obj->SetVisible(false);

    for (auto& obj : *mPhysicalObjs)
    if (IsObjectVisible(obj->GetBounds()))
    {
        obj->SetVisible(true);
        obj->Draw(pWindow);

    } else obj->SetVisible(false);

    if (mMario)
        mMario->Draw(pWindow);
}

///--------------------
inline void GameScene::GameUpdate()
{
    /// STATIC METHOD CALLS
    MysteryBox::UpdateStaticAnimation();
    Coin::UpdateStaticAnimation();
    WaterWaves::UpdateStaticAnimation();
    LavaWaves::UpdateStaticAnimation();

    /// UPDATE OBJECTS

    if (mMario)
    {
        MoveCamera();
        mMario->Update();
    }

    for (auto& obj : *mBlocks)
    if (obj->IsVisible()) obj->Update();

    for (auto& obj : *mPhysicalObjs)
    if (obj->IsVisible()) obj->Update();

    for (auto& obj : *mNotPhysicalObjs)
    if (obj->IsVisible()) obj->Update();

    /// CHECK COLLISIONS WITH BLOCKS
    if (mMario)
        MarioBlockCollision();

    PhysicalObjBlockCollision();

    /// CHECK COLLISIONS BETWEEN OBJECTS
    if (mMario)
        MarioGameObjsCollision();

    GameObjsGameObjsCollisions();

    /// REMOVE AND ADD OBJECTS
    if (mMyRemovedObjects.size() != 0)
    UpdateRemovedGameObjects();

    if (mMyNewObjects.size() != 0)
    UpdateNewGameObjects();
}

///-----------------------
inline void GameScene::UpdateNewGameObjects()
{
    /// DETERMINE WHICH CONTAINER TO ADD OBJECT TO
    for (auto* newObj : mMyNewObjects)
    {
        Block* newBlock = dynamic_cast<Block*>(newObj);

        if (newBlock)
            mBlocks->push_front(std::unique_ptr<Block>(newBlock));
        else
        {
            PhysicalObject* newPhyObj = dynamic_cast<PhysicalObject*>(newObj);

            if (newPhyObj)
                mPhysicalObjs->push_back(std::unique_ptr<PhysicalObject>(newPhyObj));
            else
                mNotPhysicalObjs->push_back(std::unique_ptr<GameObject>(newObj));
        }
    }

    mMyNewObjects.clear();
}

///------------------------
inline void GameScene::UpdateRemovedGameObjects()
{
    /// DETERMINE WHICH CONTAINER TO REMOVE FROM
    for (auto* removedObj : mMyRemovedObjects)
    {
        Block* removedBlock = dynamic_cast<Block*>(removedObj);

        if (removedBlock)
            mBlocks->erase(std::find_if(mBlocks->begin(), mBlocks->end(), [removedBlock](const std::unique_ptr<Block>& obj)
            {return removedBlock == obj.get(); }));
        else
        {
            Mario* removedMario = dynamic_cast<Mario*>(removedObj);

            if (removedMario)
                mMario.reset();
            else
            {
                PhysicalObject* removedPhysObj = dynamic_cast<PhysicalObject*>(removedObj);

                if (removedPhysObj)
                {
                    MoveableBlock::RemoveDeadObjOnMoveableBlock(removedPhysObj);

                    mPhysicalObjs->erase(std::find_if(mPhysicalObjs->begin(), mPhysicalObjs->end(), [removedPhysObj](const std::unique_ptr<PhysicalObject>& obj)
                    {return removedPhysObj == obj.get(); }));

                } else
                    mNotPhysicalObjs->erase(std::find_if(mNotPhysicalObjs->begin(), mNotPhysicalObjs->end(), [removedObj](const std::unique_ptr<GameObject>& obj)
                    {return removedObj == obj.get(); }));
            }
        }
    }

    mMyRemovedObjects.clear();
}

///-------- LESS IMPORTANT METHODS ---------///

inline void GameScene::MoveCamera()
{
    if (mMario->GetCurrentPosition().x >= MarioGame::sSizeWindow.x / 2)
    {
        float road = mMario->GetCurrentPosition().x - mMario->GetPreviousPosition().x;

        mBackground.move(road, 0);
        mBoundsView.left += road;
        mView.move(road, 0);

        Gui::MoveMainLabels(road);

    } else
    {
        mView.setCenter(sf::Vector2f(MarioGame::sSizeWindow.x / 2, MarioGame::sSizeWindow.y / 2));
        mBackground.setPosition(0, 0);
        Gui::SetPositionMainLabels(MarioGame::sSizeWindow.x / 2.0f);
    }
}

///-----------
inline Block* GameScene::WhichHitBlock(Block* pFirstBlock, Block* pSecondBlock, const PhysicalObject* pObj)
{
    if (pFirstBlock->GetCurrentPosition().x > pSecondBlock->GetCurrentPosition().x)
        std::swap(pFirstBlock, pSecondBlock);

    if (pObj->GetCurrentPosition().x <= pFirstBlock->GetCurrentPosition().x + pFirstBlock->GetSize().x / 2.0f)
        return pFirstBlock;
    else
        return pSecondBlock;
}

///----------
inline bool GameScene::IsObjectVisible(const sf::FloatRect& pBounds, bool pLargerBounds)
{
    if (pLargerBounds)
        return pBounds.intersects(sf::FloatRect(mBoundsView.left - Scene::sTileSize * 2, mBoundsView.top, mBoundsView.width + Scene::sTileSize * 4, mBoundsView.height));
    else
        return pBounds.intersects(mBoundsView);
}

///----
void GameScene::SetDoubleBoundsView()
{
    mBoundsView.left -= static_cast<int>(MarioGame::sSizeWindow.x);
    mBoundsView.width += MarioGame::sSizeWindow.x * 2;
}

///-----------
void GameScene::AddPoints(int pPoints)
{
    mPoints += pPoints;
    Gui::SetPoints(mPoints);
}

///-----
inline void GameScene::UpdateGameTime()
{
    mGameTime -= sFrameTime;
    Gui::SetTime(mGameTime);
}

///--------
void GameScene::ChangePositionView(float pValue)
{
    mBackground.move(pValue, 0);
    mView.move(pValue, 0);
    mBoundsView.left += pValue;
    Gui::MoveMainLabels(pValue);
}

///-------------
const sf::Vector2f& GameScene::GetMarioPosition() const
{
    if (mMario)
        return mMario->GetCurrentPosition();
    else
        return mView.getCenter();
}

///--------
inline void GameScene::CreateExtinguishingEffect(std::string pName)
{
    /// DARKENING BACKGROUND
    sf::Sprite* sprite = Gui::CreateSprite("LoadingScen", sf::Vector2f(MarioGame::Instance().GetWindow()->getView().getCenter().x,
    MarioGame::sSizeWindow.y), 1, true);

    sprite->setColor(sf::Color::Black);

    std::function<bool(std::unique_ptr<Animator>&)> spriteAction = [valueTransparency = 0.0f](std::unique_ptr<Animator>& pAnimator) mutable
    {
        sf::Color color(pAnimator->GetSprite().getColor());

        if (valueTransparency < 255)
        {
            color.a = 0 + valueTransparency;
            valueTransparency += 0.5f;
        }
        else
            color.a = 255;

        pAnimator->GetSprite().setColor(color);

        return (!MarioGame::Instance().IsThisScene<GameScene>());
    };

    Gui::AddGuiObject(new SpecialEffects(new SpriteAnimator(sprite), spriteAction));

    /// APPEARING TEXT
    sf::Text* text = Gui::CreateText(pName, mView.getCenter(), sf::Color::White, "menu_font", true, 50);

    std::function<bool(std::unique_ptr<sf::Text>&)> textAction = [](std::unique_ptr<sf::Text>& pText)
    {
        return (!MarioGame::Instance().IsThisScene<GameScene>());
    };

    Gui::AddGuiObject(new FlowText(text, textAction, 0.5f));
}

///------------
inline void GameScene::RemoveNewGameObjects()
{
    for (auto obj : mMyNewObjects) delete obj;
    mMyNewObjects.clear();
}
