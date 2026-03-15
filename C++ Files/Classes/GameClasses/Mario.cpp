#include "Mario.h"
#include "../Scens/GameScen.h"
#include "Blocks/MoveableBlock.h"

#include <cmath>

const short Mario::sBasicNumberLivesMario = 5;
short Mario::sLivesMario = Mario::sBasicNumberLivesMario;

Mario::LevelMario Mario::sLevelMario = Mario::LevelMario::SmallMario;

Mario::~Mario()
{
    MoveableBlock::RemoveDeadObjOnMoveableBlock(this);
}

///-----
Mario::Mario(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Mario, pPos, KindMovement::Standing)
    , Shield(1500)
    , mDeactivateDecisions(EventHandler::GetDeactivateDecisions())
    , mEnabledDecisions(EventHandler::GetEnabledDecisions())
{
    EventHandler::ResetDecisions();

    mJumpForce = -1100.0f;
    mIsRightDirReversal = true;

    if (sLevelMario == LevelMario::SmallMario)
        CreateSmallMarioAnimations();
    else
        CreateBigMarioAnimations();

    mAnimations->Play(Animations::RightStanding, mCurrentPosition);
}

///--------------------
void Mario::Update()
{
    HandleMarioControl();

    mPreviousPosition = mCurrentPosition;

    if (mIsInBottomCollision)
        mPreviousPosition.y -= sCorrectionToBottomCollision;

    this->Run();

    if (mIsJump)
        this->MakeJump();

    if (mIsShooting && mShootingIterativityTimer < Scene::GetDurationScene())
    {
        mIsShooting = false;
        AnimationUpdate();
    }

    CollisionWithBoundsMap();

    mAnimations->Update(mCurrentPosition);

    if (mShieldPointer)
    {
        mShieldPointer->Update();

        if (mShieldPointer->IsShieldDepleted())
            this->Remove();
    }
}

///---------------------
void Mario::ActOnMe(KindAction pAction)
{
    if (mIsDead) return;

    switch (pAction)
    {
    case KindAction::LevelUp:
    {
        if (mFlashingMario || sLevelMario != LevelMario::SmallMario)
            return;

        if (MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::ChangingLevelMario))
        {
            const sf::FloatRect prevSmallMarioBounds(GetPreviousBounds());

            if (!mIsRightDirReversal)
                mAnimations->Play(MyKindsAnimations::LeftLevelUp, mCurrentPosition);
            else
                mAnimations->Play(MyKindsAnimations::RightLevelUp, mCurrentPosition);

            this->CorrectPositionWhenChangingOnBigMario(prevSmallMarioBounds);

            mKindMovement = KindMovement::Standing;
            mCurbing = mStop = false;
            sLevelMario = LevelMario::BigMario;

            mHorizontalVelocity = 0.0f;
            if (mIsJump) mForce = 0.0f;

            mChangingLevelTimer = Scene::GetDurationScene() + mChangingLevelTime;

            MarioGame::sSoundManager.play("powerup");
        }

        break;
    }

    case KindAction::Hop:
    {
        Hop(-600.0f);
        break;
    }

    case KindAction::UnderMap:
    {
        mCurrentPosition.y = MarioGame::sSizeWindow.y + Scene::sTileSize;
        Remove();
        break;
    }

    case KindAction::Hit:
    {
        if (mFlashingMario)
            return;

        if (mShieldPointer)
        {
            ReduceShield();

            if (GetShieldValue() > 0)
                SetFlashing(mFlashingTime * 0.5f);

        }
        else if (sLevelMario != LevelMario::SmallMario)
        {
            if (MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::ChangingLevelMario))
            {
                if (!mIsRightDirReversal)
                    mAnimations->Play(MyKindsAnimations::LeftLevelDown, mCurrentPosition);
                else
                    mAnimations->Play(MyKindsAnimations::RightLevelDown, mCurrentPosition);

                mChangingLevelTimer = Scene::GetDurationScene() + mChangingLevelTime;
                sLevelMario = LevelMario::SmallMario;

                mKindMovement = KindMovement::Standing;
                mCurbing = mStop = false;

                mHorizontalVelocity = 0.0f;
                if (mIsJump) mForce = 0.0f;

                MarioGame::sSoundManager.play("pipe");
            }
        }
        else
            this->Remove();

        break;
    }
    }
}

///------------------
void Mario::UpdateForCollisionWithBlock(KindCollision pCollision, Block* pBlock)
{
    switch (pCollision)
    {
        case KindCollision::Top:
        {
            if (sLevelMario == LevelMario::SmallMario)
                pBlock->ActOnMe(KindAction::Hit);
            else
                pBlock->ActOnMe(KindAction::Destroyed);

            mForce = 0.0;
            break;
        }

        case KindCollision::Bottom:
        {
            mIsJump = false;
            mIsInBottomCollision = true;

            AnimationUpdate();

            break;
        }

        case KindCollision::RightSide:
        {
            if (mIsRightDirReversal)
            {
                if (mCurbing)
                {
                    mKindMovement = KindMovement::Standing;
                    mCurbing = mStop = false;
                }

                if (!mIsJump)
                    if (mIsRightDirReversal)
                        mAnimations->Play(Animations::RightStanding, mCurrentPosition);
                    else
                        mAnimations->Play(Animations::LeftStanding, mCurrentPosition);

                mHorizontalVelocity = 1.0f;

            } else
            {
                if (mCurbing)
                {
                    mCurbing = mStop = false;
                    mHorizontalVelocity = 0.0f;
                }
            }

            break;
        }

        case KindCollision::LeftSide:
        {
            if (!mIsRightDirReversal)
            {
                if (mCurbing)
                {
                    mKindMovement = KindMovement::Standing;
                    mCurbing = mStop = false;
                }

                if (!mIsJump)
                    if (mIsRightDirReversal)
                        mAnimations->Play(Animations::RightStanding, mCurrentPosition);
                    else
                        mAnimations->Play(Animations::LeftStanding, mCurrentPosition);

                mHorizontalVelocity = -1.0f;

            } else
            {
                if (mCurbing)
                {
                    mCurbing = mStop = false;
                    mHorizontalVelocity = 0.0f;
                }
            }

            break;
        }

        case KindCollision::None:
        {
            return;
        }

    }

    pBlock->ActOnObject(this, pCollision);
}

///------------------------
void Mario::Run()
{
    switch (mKindMovement)
    {
    case KindMovement::RightRun:
        {
            if (!mCurbing)
            {
                if (mHorizontalVelocity < mMaxSpeed)
                    mHorizontalVelocity += mValueAcceleration;
                else
                    mHorizontalVelocity = mMaxSpeed;

            } else Curbing();

            mCurrentPosition.x += mHorizontalVelocity;

            break;
        }
    case KindMovement::LeftRun:
        {
            if (!mCurbing)
            {
                if (-mHorizontalVelocity < mMaxSpeed)
                    mHorizontalVelocity -= mValueAcceleration;
                else
                    mHorizontalVelocity = -mMaxSpeed;

            } else Curbing();

            mCurrentPosition.x += mHorizontalVelocity;

            break;
        }
    }
}

///-------------------------
void Mario::Curbing()
{
    if (fabs(0.3f - fabs(mHorizontalVelocity)) < 0.3f)
    {
        mCurbing = false;
        mHorizontalVelocity = 0.0f;

        if (mStop)
        {
            mKindMovement = KindMovement::Standing;

            if (!mIsJump)
            {
                if (mIsRightDirReversal)
                    mAnimations->Play(Animations::RightStanding, mCurrentPosition);
                else
                    mAnimations->Play(Animations::LeftStanding, mCurrentPosition);
            }

            mStop = false;
        }
    }
    else
        if (mHorizontalVelocity > 0)
            mHorizontalVelocity -= mValueAcceleration * 1.8f;
        else
            if (mHorizontalVelocity < 0)
                mHorizontalVelocity += mValueAcceleration * 1.8f;
}

///------------------
inline void Mario::HandleMarioControl()
{
    if (mEnabledDecisions[EventHandler::DecisionsPlayer::ClickJump])
        Jump();

    ///----
    if (mEnabledDecisions[EventHandler::DecisionsPlayer::ClickShoot])
        Shoot();

    ///------
    if (mEnabledDecisions[EventHandler::DecisionsPlayer::ClickCrouch])
        Crouch();

    ///--------
    if (mEnabledDecisions[EventHandler::DecisionsPlayer::ClickLeftRun])
    {
        if (mKindMovement == KindMovement::LeftRun) mCurbing = false;
        if (mKindMovement == KindMovement::RightRun) mCurbing = true;

        mKindMovement = KindMovement::LeftRun;
        mIsRightDirReversal = false;
        mStop = false;

        AnimationUpdate();
    }

    ///-------
    if (mEnabledDecisions[EventHandler::DecisionsPlayer::ClickRightRun])
    {
        if (mKindMovement == KindMovement::RightRun) mCurbing = false;
        if (mKindMovement == KindMovement::LeftRun) mCurbing = true;

        mKindMovement = KindMovement::RightRun;
        mIsRightDirReversal = true;
        mStop = false;

        AnimationUpdate();
    }

    ///----------------- FOR RELEASED KEYS -----------------///

    if (mDeactivateDecisions[EventHandler::DecisionsPlayer::ClickLeftRun] && mKindMovement == KindMovement::LeftRun)
        mStop = mCurbing = true;

    ///----
    if (mDeactivateDecisions[EventHandler::DecisionsPlayer::ClickRightRun] && mKindMovement == KindMovement::RightRun)
        mStop = mCurbing = true;

    ///----
    if (mDeactivateDecisions[EventHandler::DecisionsPlayer::ClickCrouch])
    {
        mIsCrouching = false;

        if (sLevelMario == LevelMario::BigMario)
            AnimationUpdate();
    }

    /// RESET
    EventHandler::ResetDecisions();
}

///---
void Mario::CorrectPositionWhenChangingOnBigMario(const sf::FloatRect& pPrevSmallMarioBounds)
{
    mAnimations->SetIndexFrame(1, mCurrentPosition);

    const auto blocks = MarioGame::Instance().GetScene<GameScene>().GetBlocks();

    for (const auto& block : *blocks)
    {
        if (!block->IsVisible()) continue;

        if (GetBounds().intersects(block->GetBounds()))
        {
            KindCollision howCollision;

            if (block->GetCurrentPosition().y < pPrevSmallMarioBounds.top)
            {
                howCollision = KindCollision::Top;
                block->CorrectObjectPositionOnMe(*this, howCollision);
                mCurrentPosition.y += 5;
                continue;
            } else
                if (block->GetBounds().top > pPrevSmallMarioBounds.top + pPrevSmallMarioBounds.height)
                    howCollision = KindCollision::Bottom;
            else
                if (block->GetCurrentPosition().x < GetPreviousBounds().left + GetPreviousBounds().width / 2.0f)
                    howCollision = KindCollision::LeftSide;
            else
                    howCollision = KindCollision::RightSide;

            block->CorrectObjectPositionOnMe(*this, howCollision);
        }
    }

    mAnimations->SetIndexFrame(0, mCurrentPosition);
}

///---
void Mario::Shoot()
{
    if (sLevelMario == LevelMario::BigMario && !mIsShooting)
    {
        if (mIsRightDirReversal)
            mAnimations->Play(MyKindsAnimations::RightShoot, mCurrentPosition);
        else
            mAnimations->Play(MyKindsAnimations::LeftShoot, mCurrentPosition);

        mShootingIterativityTimer = Scene::GetDurationScene() + mShootingIterativity;
        mIsShooting = true;
        MarioGame::sSoundManager.play("fireball");

        /// CREATE BULLET
        sf::Vector2f posBullet;

        if (mIsRightDirReversal)
            posBullet.x = mCurrentPosition.x + GetSize().x / 2.0f;
        else
            posBullet.x = mCurrentPosition.x - GetSize().x / 2.0f;

        posBullet.y = mCurrentPosition.y - GetSize().y / 2.0f;

        if (fabs(mHorizontalVelocity) > 3.5f)
        {
            posBullet.x += 3.5f;
            AddNewObject(new Bullet(posBullet, mIsRightDirReversal, true));
        }
        else
            AddNewObject(new Bullet(posBullet, mIsRightDirReversal));
    }
}

///-----------------
void Mario::ChangingLevel()
{
    mAnimations->Update(mCurrentPosition);

    if (mChangingLevelTimer < Scene::GetDurationScene() && !mAnimations->IsLastFrame())
    {
        switch (sLevelMario)
        {
            case LevelMario::BigMario:
            {
                CreateBigMarioAnimations();
                break;
            }

            case LevelMario::SmallMario:
            {
                CreateSmallMarioAnimations();
                SetFlashing(mFlashingTime);
                break;
            }
        }

        AnimationUpdate();

        MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::MainGame);

        /// RESET KEYS
        EventHandler::ResetDecisions();
    }
}

///-----
void Mario::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    if (mShieldPointer)
        mShieldPointer->Draw(pWindow, {MarioGame::Instance().GetViewPosition().x - MarioGame::sSizeWindow.x / 2.0f + 70, 20});

    if (mFlashingMario && Flashing())
        return;

    mAnimations->Draw(pWindow);
}

///------------
inline void Mario::Remove()
{
    if (MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::MarioDead))
    {
        RemoveObject(this);
        sLivesMario--;
    }
}

///------
void Mario::SetShield()
{
    if (mShieldPointer || mIsDead)
        return;

    Gui::SetVisibleMainLabels(false);

    CreateShieldPointer(Gui::CreateRectangleShape({0, 0, 500, 30}, sf::Color::Black, false, true)
            , sf::Color::Blue, ShieldPointer::KindsOrigin::Left, Gui::CreateSprite("Mario_right", {0, 96, 32, 32}, {0, 0}, 1.1f, true));

    // INCREASE JUMP FORCE
    mJumpForce *= 1.2f;
}

///-------
inline void Mario::SetFlashing(float pHowLong)
{
    mFlashingTimer = Scene::GetDurationScene() + pHowLong;
    mFlashingIterativityTimer = 0.0f;
    mFlashingMario = true;
}

///-----------
inline bool Mario::Flashing()
{
    if (mFlashingTimer < GameScene::GetDurationScene())
    {
        mFlashingMario = false;
        return true;
    } else
    {
        if (mFlashingIterativityTimer < Scene::GetDurationScene())
        {
            mVisibleWhileFlashing = !mVisibleWhileFlashing;
            mFlashingIterativityTimer = Scene::GetDurationScene() + mFlashingIterativity;
        }

        return mVisibleWhileFlashing;
    }
}

///----------
inline void Mario::Crouch()
{
    if (!mIsJump && mKindMovement == KindMovement::Standing)
    {
        if (sLevelMario == LevelMario::BigMario)
            if (mIsRightDirReversal)
                mAnimations->Play(MyKindsAnimations::RightCrouch, mCurrentPosition);
            else
                mAnimations->Play(MyKindsAnimations::LeftCrouch, mCurrentPosition);

        mIsCrouching = true;
    }
}

///---------
inline void Mario::CollisionWithBoundsMap()
{
    if (GetBounds().left <= 0 && mHorizontalVelocity < 0)
    {
        mStop = mCurbing = false;
        mHorizontalVelocity = 0.0f;

        if (mKindMovement != KindMovement::RightRun)
        {
            mKindMovement = KindMovement::Standing;
            AnimationUpdate();
        }

        mCurrentPosition = {GetBounds().width / 2.0f, mCurrentPosition.y};
    }
}

///-------
void Mario::Falling()
{
    PhysicalObject::Falling();

    const float previousWidth = mAnimations->GetGlobalBounds().width;

    AnimationUpdate();

    if (GetBounds().width > previousWidth)
    {
        if (mHorizontalVelocity > 0)
            mCurrentPosition.x += GetBounds().width - previousWidth;
        else
            mCurrentPosition.x -= GetBounds().width - previousWidth;

        mAnimations->SetPosition(mCurrentPosition);
    }
}

///-----------------------------
void Mario::Jump()
{
    if (!mIsJump)
    {
        PhysicalObject::Jump();

        AnimationUpdate();
        MarioGame::sSoundManager.play("jump_super");
    }
}

///--------------------
void Mario::Hop(float pForce)
{
    PhysicalObject::Hop(pForce);
    AnimationUpdate();
}

///-------
inline void Mario::AnimationUpdate()
{
    if (mIsJump)
        if (mIsRightDirReversal)
            mAnimations->Play(Animations::RightJump, mCurrentPosition);
        else
            mAnimations->Play(Animations::LeftJump, mCurrentPosition);
    else
    {
        if (mKindMovement == KindMovement::Standing)
            if (mIsRightDirReversal)
                mAnimations->Play(Animations::RightStanding, mCurrentPosition);
            else
                mAnimations->Play(Animations::LeftStanding, mCurrentPosition);
        else
            if (mIsRightDirReversal)
                mAnimations->Play(Animations::RightMove, mCurrentPosition);
            else
                mAnimations->Play(Animations::LeftMove, mCurrentPosition);
    }
}

///-------
void Mario::CreateDeathAnimation(const sf::Vector2f& pPos)
{
    sf::Sprite* deadMario = Gui::CreateSprite("Mario_right", {192, 96, 32, 32}, pPos, kScaleToTile, true);

    sf::Vector2f forceJumpDeathMario;

    if (pPos.y > MarioGame::sSizeWindow.y)
        forceJumpDeathMario = {0, -1400.0f};
    else
        forceJumpDeathMario = {0, -1000.0f};

    GuiObject* obj = new SpecialEffects(new SpriteAnimator(deadMario), SpecialEffects::KindUpdate::Jump, MarioGame::sSizeWindow.y + Scene::sTileSize, forceJumpDeathMario);
    Gui::AddGuiObject(obj);

    ResetLevelMario();
}

///----------------------
inline void Mario::CreateBigMarioAnimations()
{
    mAnimations->Reset();

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({32, 36, 32, 60});
    frameAnimation.push_back({65, 36, 30, 60});
    frameAnimation.push_back({96, 38, 32, 58});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Mario_right"], frameAnimation, mMovementAnimationSpeed, kScaleToTile);

    ///----------
    frameAnimation.clear();
    frameAnimation.push_back({448, 36, 32, 60});
    frameAnimation.push_back({418, 36, 30, 60});
    frameAnimation.push_back({384, 38, 32, 58});

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Mario_left"], frameAnimation, mMovementAnimationSpeed, kScaleToTile);

    ///----------
    frameAnimation.clear();
    frameAnimation.push_back({0, 38, 32, 58});
    frameAnimation.push_back({4, 96, 24, 32});

    mAnimations->Create(MyKindsAnimations::RightLevelDown, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.5f, kScaleToTile);

    ///----------
    frameAnimation.clear();
    frameAnimation.push_back({480, 38, 32, 58});
    frameAnimation.push_back({484, 96, 24, 32});

    mAnimations->Create(MyKindsAnimations::LeftLevelDown, MarioGame::sTextureManager["Mario_left"], frameAnimation, 1.5f, kScaleToTile);

    ///---------------- SINGLE FRAME ANIMATIONS ------------////////////
    mAnimations->Create(Animations::LeftStanding, MarioGame::sTextureManager["Mario_left"], {480, 38, 32, 58}, kScaleToTile);
    mAnimations->Create(Animations::RightStanding, MarioGame::sTextureManager["Mario_right"], {0, 38, 32, 58}, kScaleToTile);
    mAnimations->Create(Animations::RightJump, MarioGame::sTextureManager["Mario_right"], {160, 35, 32, 60}, kScaleToTile);
    mAnimations->Create(Animations::LeftJump, MarioGame::sTextureManager["Mario_left"], {320, 35, 32, 60}, kScaleToTile);

    mAnimations->Create(MyKindsAnimations::LeftShoot, MarioGame::sTextureManager["Mario_left"], {258, 36, 29, 60}, kScaleToTile);
    mAnimations->Create(MyKindsAnimations::RightShoot, MarioGame::sTextureManager["Mario_right"], {225, 36, 29, 60}, kScaleToTile);

    mAnimations->Create(MyKindsAnimations::LeftCrouch, MarioGame::sTextureManager["Mario_left"], {287, 50, 32, 46}, kScaleToTile);
    mAnimations->Create(MyKindsAnimations::RightCrouch, MarioGame::sTextureManager["Mario_right"], {192, 50, 32, 46}, kScaleToTile);

}

///--------------
inline void Mario::CreateSmallMarioAnimations()
{
    mAnimations->Reset();

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({36, 96, 24, 32});
    frameAnimation.push_back({68, 96, 24, 32});
    frameAnimation.push_back({97, 96, 30, 32});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Mario_right"], frameAnimation, mMovementAnimationSpeed, kScaleToTile);

    ///---------
    frameAnimation.clear();
    frameAnimation.push_back(sf::IntRect(452, 96, 24, 32));
    frameAnimation.push_back(sf::IntRect(420, 96, 24, 32));
    frameAnimation.push_back(sf::IntRect(385, 96, 30, 32));

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Mario_left"], frameAnimation, mMovementAnimationSpeed, kScaleToTile);

    ///---------
    frameAnimation.clear();
    frameAnimation.push_back({4, 96, 24, 32});
    frameAnimation.push_back({0, 38, 32, 58});

    mAnimations->Create(MyKindsAnimations::RightLevelUp, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.5f, kScaleToTile);

    ///---------
    frameAnimation.clear();
    frameAnimation.push_back({484, 96, 24, 32});
    frameAnimation.push_back({480, 38, 32, 58});

    mAnimations->Create(MyKindsAnimations::LeftLevelUp, MarioGame::sTextureManager["Mario_left"], frameAnimation, 1.5f, kScaleToTile);

    ///---------------- SINGLE FRAME ANIMATIONS ------------////////////
    mAnimations->Create(Animations::LeftStanding, MarioGame::sTextureManager["Mario_left"], {484, 96, 24, 32}, kScaleToTile);
    mAnimations->Create(Animations::RightStanding, MarioGame::sTextureManager["Mario_right"], {4, 96, 24, 32}, kScaleToTile);
    mAnimations->Create(Animations::RightJump, MarioGame::sTextureManager["Mario_right"], {160, 96, 32, 32}, kScaleToTile - 0.1f);
    mAnimations->Create(Animations::LeftJump, MarioGame::sTextureManager["Mario_left"], {320, 96, 32, 32}, kScaleToTile - 0.1f);
}
