#include "SpecialPipe.h"

#include "../../Scens/GameScen.h"
#include "../../Scens/LoadingGameScen.h"

#include "../Mario.h"

#include <algorithm>
#include <string>

std::vector<EnteringPipe*> EnteringPipe::sActivateEnteringPipes;

///-----------EnteringPipe---------------///
EnteringPipe::EnteringPipe(const std::pair<sf::Vector2f, Pipe::KindPipe>& pDataPipe, std::string pUndergroundLvlName, Pipe* pReturnPipe)
    : Pipe(pDataPipe.first, pDataPipe.second)
    , mUndergroundLvlName(pUndergroundLvlName)
    , mReturnPipe(pReturnPipe)
{

}

///----
EnteringPipe::EnteringPipe(const std::pair<sf::Vector2f, Pipe::KindPipe>& pDataPipe, std::string pUndergroundLvlName)
    : Pipe(pDataPipe.first, pDataPipe.second)
    , mUndergroundLvlName(pUndergroundLvlName)
{
    mReturnPipe = this;
}

///-----
void EnteringPipe::Update()
{
    if (!mActive)
        return;

    GameScene& gameScene = MarioGame::Instance().GetScene<GameScene>();

    /// MUST DO IT THIS WAY OTHERWISE IT DOESN'T WORK
    const float entryPos = mCurrentPosition.y - GetBounds().height;

    if (!gameScene.IsMarioDead())
    if (gameScene.mMario->IsCrouching())
    if (gameScene.GetMarioPosition().y > entryPos &&
       gameScene.GetMarioPosition().x >= mCurrentPosition.x - mEnteringRange &&
       gameScene.GetMarioPosition().x <= mCurrentPosition.x + mEnteringRange)
    if (gameScene.SetGamePlayState(GameScene::GamePlayStates::EnteringToPipe))
    {
        SetEnteringToPipeAnimation(false);
        sActivateEnteringPipes.push_back(this);
    }
}

///------
void EnteringPipe::SetBeforeWorld()
{
    GameScene& gameScene = MarioGame::Instance().GetScene<GameScene>();

    assert(gameScene.mGamePlayState == GameScene::GamePlayStates::LeavingUndergroundWorldByPipe);

    /// RESTORE OLD OBJECTS FROM THE PARENT LEVEL AND REMOVE THOSE FROM THE CURRENT ONE

    mLandscapes.swap(gameScene.mLandscapes);
    mLandscapes.reset();

    ///------
    mBlocks.swap(gameScene.mBlocks);
    mBlocks.reset();

    ///----
    mNotPhysicalObjs.swap(gameScene.mNotPhysicalObjs);
    mNotPhysicalObjs.reset();

    ///----
    mPhysicalObjs.swap(gameScene.mPhysicalObjs);
    mPhysicalObjs.reset();

    ///---
    gameScene.mBackground.setTexture(MarioGame::sTextureManager[mBackgroundLvlName]);
    gameScene.mTextureBackgroundName = mBackgroundLvlName;

    ///- SET THE VIEW AND VIEW BOUNDARIES
    float newXCenter;

    if (mReturnPipe->GetCurrentPosition().x > MarioGame::sSizeWindow.x / 2.0f)
        newXCenter = mReturnPipe->GetCurrentPosition().x;
    else
        newXCenter = MarioGame::sSizeWindow.x / 2.0f;

    gameScene.mView.setCenter(newXCenter, MarioGame::sSizeWindow.y / 2.0f);
    gameScene.mBoundsView.left = newXCenter - gameScene.mBoundsView.width / 2.0f;
    gameScene.mBackground.setPosition(newXCenter - MarioGame::sSizeWindow.x / 2.0f, 0);

    Gui::SetPositionMainLabels(newXCenter);

    /// DOING IT THIS WAY INSTEAD OF USING THE METHOD BECAUSE THERE IS A CONDITION THAT WOULD PREVENT THE STATE CHANGE
    gameScene.mGamePlayState = GameScene::GamePlayStates::ReturnToBeforeWorld;
    MarioGame::sMusicManager.Resume();

    SetLeavingPipeAnimation();

    /// IMPORTANT: SET MY POSITION SO IT IS VISIBLE IN THE WINDOW
    /// TO BE ABLE TO DRAW THE EXIT ANIMATION. LATER RESTORE THE ORIGINAL POSITION
    if (mReturnPipe != this && !MarioGame::Instance().GetScene<GameScene>().IsObjectVisible(GetBounds()))
    {
        mCurrentPosition.x = mReturnPipe->GetCurrentPosition().x;
        mCurrentPosition.y = 0;
        mAnimator->SetPosition(mCurrentPosition);
    }

    /// SO THE FLOWER IS NOT DRAWN
    mReturnPipe->ResetFlower();
}

///----
void EnteringPipe::SetUndergroundLvl()
{
    GameScene& gameScene = MarioGame::Instance().GetScene<GameScene>();

    assert(gameScene.mGamePlayState == GameScene::GamePlayStates::EnteringToPipe);

    /// SAVE THE MAIN LEVEL OBJECTS
    gameScene.mLandscapes.swap(mLandscapes);
    gameScene.mBlocks.swap(mBlocks);
    gameScene.mNotPhysicalObjs.swap(mNotPhysicalObjs);
    gameScene.mPhysicalObjs.swap(mPhysicalObjs);

    const float gameTime = gameScene.mGameTime;
    mDurationScene = Scene::sDurationScene;
    mBackgroundLvlName = gameScene.mTextureBackgroundName;

    /// LOAD THE UNDERGROUND LEVEL AND SET NEW DATA IN THE GAME SCENE

    std::string lvlName = MarioGame::Instance().GetCurrentLevelName() + "/";

    for (auto pPipe : sActivateEnteringPipes)
        lvlName += (pPipe->mUndergroundLvlName) + "/";

    LoadingGameScene(lvlName, this);/// CREATE A TEMPORARY OBJECT BECAUSE I ONLY WANT IT TO LOAD THE OBJECTS

    /// SET THE REMAINING DATA

    MarioGame::Instance().GetScene<GameScene>().mMario->SetPosition({static_cast<float>(Scene::sTileSize) * 2.5f, 0});
    MarioGame::Instance().GetScene<GameScene>().mGameTime = gameTime;

    mMarioEnteringAnimation.reset();
}

///------
void EnteringPipe::ResumeGame()
{
    GameScene& gameScene = MarioGame::Instance().GetScene<GameScene>();

    assert(gameScene.mGamePlayState == GameScene::GamePlayStates::ReturnToBeforeWorld);

    Scene::sDurationScene = mDurationScene;

    gameScene.mMario.reset(new Mario({mReturnPipe->GetCurrentPosition().x, mReturnPipe->GetBounds().top}));

    gameScene.mGamePlayState = GameScene::GamePlayStates::MainGame;

    mMarioEnteringAnimation.reset();

    mActive = false;

    sActivateEnteringPipes.pop_back();

    /// SO THE FLOWER DOESN'T HIT MARIO
    mReturnPipe->ResetFlower();

    /// RESTORE MY PREVIOUS ORIGINAL POSITION
    if (mReturnPipe != this)
    {
        mCurrentPosition = mPreviousPosition;
        mAnimator->SetPosition(mCurrentPosition);
    }
}

///-------
void EnteringPipe::SetLeavingPipeAnimation()
{
    if (Mario::GetLevelMario() == Mario::LevelMario::BigMario)
        mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_right", {0, 38, 32, 58}, {0, 0}, GameObject::kScaleToTile, true));
    else
        mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_right", {4, 96, 24, 32}, {0, 0}, GameObject::kScaleToTile, true));

    mMarioEnteringAnimation->setPosition(mReturnPipe->GetCurrentPosition().x, mReturnPipe->GetBounds().top + mMarioEnteringAnimation->getGlobalBounds().height);
    mEndEnteringAnimation = false;
    mUpEnteringAnimation = true;

    mEndAnimationPos = mReturnPipe->GetBounds().top;

}

///------
void EnteringPipe::SetEnteringToPipeAnimation(bool pUpAnimation)
{
    const Mario* mario = MarioGame::Instance().GetScene<GameScene>().mMario.get();

    const sf::Vector2f marioPos = mario->GetCurrentPosition();
    const bool rightReversalMario = mario->IsInRightReversal();

    if (Mario::GetLevelMario() == Mario::LevelMario::BigMario)
        if (!pUpAnimation)
        {
            if (rightReversalMario)
                mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_right", {192, 50, 32, 46}, marioPos, GameObject::kScaleToTile, true));
            else
                mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_left", {287, 50, 32, 46}, marioPos, GameObject::kScaleToTile, true));
        }
        else
        {
            if (rightReversalMario)
                mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_right", {0, 38, 32, 58}, marioPos, GameObject::kScaleToTile, true));
            else
                mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_left", {480, 38, 32, 58}, marioPos, GameObject::kScaleToTile, true));
        }
    else
        if (rightReversalMario)
            mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_right", {4, 96, 24, 32}, marioPos, GameObject::kScaleToTile, true));
        else
            mMarioEnteringAnimation.reset(Gui::CreateSprite("Mario_left", {484, 96, 24, 32}, marioPos, GameObject::kScaleToTile, true));


    RemoveObject(MarioGame::Instance().GetScene<GameScene>().mMario.get());
    mEndEnteringAnimation = false;
    mUpEnteringAnimation = pUpAnimation;

    if (pUpAnimation)
        mEndAnimationPos = mCurrentPosition.y - 1;
    else
        mEndAnimationPos = this->GetBounds().top + 1;
}

///------
void EnteringPipe::EnteringAnimation()
{
    if (!mMarioEnteringAnimation || mEndEnteringAnimation)
        return;

    if (mUpEnteringAnimation)
    {
        if (mMarioEnteringAnimation->getPosition().y > mEndAnimationPos)
            mMarioEnteringAnimation->move(0, -mEnteringSpeed);
        else
            mEndEnteringAnimation = true;
    }
    else
    {
        if (mMarioEnteringAnimation->getGlobalBounds().top < mEndAnimationPos)
            mMarioEnteringAnimation->move(0, mEnteringSpeed);
        else
            mEndEnteringAnimation = true;
    }
}

///------
EnteringPipe* EnteringPipe::GetActivatePipe()
{
    if (sActivateEnteringPipes.size() == 0)
        return nullptr;

    return sActivateEnteringPipes[sActivateEnteringPipes.size() - 1];
}

///------
void EnteringPipe::ResetActivatedPipes()
{
    for (auto pActivatePipe : sActivateEnteringPipes)
    {
        auto& blocksContainer = pActivatePipe->mBlocks;

        /// THIS IS THE SITUATION WHEN DELETING WHILE ENTERING THE PIPE
        /// THE PIPE IS ACTIVATED BUT IT DOESN'T HAVE A SAVED LEVEL YET
        if (!blocksContainer)
            continue;

        auto myPosInContainer = find_if(blocksContainer->begin(), blocksContainer->end(),
                             [pActivatePipe](const std::unique_ptr<Block>& pBlock) { return pBlock.get() == pActivatePipe; });

        /// VERY IMPORTANT BECAUSE IT PREVENTS DOUBLE DELETION
        myPosInContainer->release();
        delete pActivatePipe;/// DELETING THE PIPE ALSO DELETES THE LEVEL SAVED IN IT!!!
    }

    sActivateEnteringPipes.clear();
}

///------
void EnteringPipe::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    if (mMarioEnteringAnimation)
    {
        pWindow->draw(*mMarioEnteringAnimation);

        /// MUST DRAW THE PIPE OVER THE ANIMATION
        if (mReturnPipe != this && MarioGame::Instance().GetScene<GameScene>().mGamePlayState == GameScene::GamePlayStates::ReturnToBeforeWorld)
            mReturnPipe->Draw(pWindow);
    }

    mAnimator->Draw(pWindow);
}

///-----------ReturnPipe---------------///
ReturnPipe::ReturnPipe(sf::Vector2f pPos)
    : StaticBlock({187, 366, 64, 128}, {pPos.x - static_cast<float>(Scene::sTileSize) / 2.0f, pPos.y})
{
}

///-----
void ReturnPipe::Update()
{
    GameScene& gameScene = MarioGame::Instance().GetScene<GameScene>();

    if (EnteringPipe::GetActivatePipe())
    if (!gameScene.IsMarioDead())
    if (gameScene.mMario->IsJumping())
    if (gameScene.mMario->GetBounds().top - 1 < mCurrentPosition.y &&
       gameScene.GetMarioPosition().x >= mCurrentPosition.x - mEnteringRange &&
       gameScene.GetMarioPosition().x <= mCurrentPosition.x + mEnteringRange)
    if (gameScene.SetGamePlayState(GameScene::GamePlayStates::LeavingUndergroundWorldByPipe))
    {
        /// USING THE ANIMATION MECHANISM FROM THE ENTERING PIPE
        EnteringPipe::GetActivatePipe()->SetEnteringToPipeAnimation(true);
    }
}

///--------
void ReturnPipe::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    if (EnteringPipe::GetActivatePipe())/// USING THE ANIMATION MECHANISM FROM THE ENTERING PIPE, HERE ONLY DRAWING
    if (EnteringPipe::GetActivatePipe()->mMarioEnteringAnimation)
        pWindow->draw(*(EnteringPipe::GetActivatePipe()->mMarioEnteringAnimation));

    mAnimator->Draw(pWindow);
}
