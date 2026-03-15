#include "Enemies.h"
#include "../../Scens/GameScen.h"

#include <cmath>
#include <vector>

///-------------RedTurtleShell--------------///

CreatorRedTurtles::RedTurtleShell::RedTurtleShell(sf::Vector2f pPos, bool pRightDir)
    : PhysicalObject(new SpriteAnimator(Gui::CreateSprite("Enemies_right", {355, 81, 26, 30}, {pPos.x, pPos.y - 1}, kScaleToTile, true)), Parentage::Item, pPos, static_cast<PhysicalObject::KindMovement>((int)!pRightDir))
    , mBasicYPos(pPos.y)
{
    mValueAcceleration = 1;
    Hop(-500);
}

///--------
void CreatorRedTurtles::RedTurtleShell::UpdateForCollisionWithBlock(KindCollision pHowCollision, Block* pBlock)
{
    if (mCurrentPosition.y < mBasicYPos)
        return;

    switch (pHowCollision)
    {
    case KindCollision::Bottom:
        {
            pBlock->ActOnObject(this, pHowCollision);

            mCurrentPosition.y -= 0.1f;
            AddNewObject(new RedTurtle(mCurrentPosition, mKindMovement));

            RemoveObject(this);
            return;
        }

    case KindCollision::None:
        {
            return;
        }
    }

    pBlock->ActOnObject(this, pHowCollision);
}

///--------
void CreatorRedTurtles::RedTurtleShell::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        pObj->ActOnMe(KindAction::Hit);
}

///-------------CreatorRedTurtles--------------///
CreatorRedTurtles::CreatorRedTurtles(sf::Vector2f pPos)
    : GameObject(new Animations, Parentage::Enemy, pPos, 1000)
    , mActivationPosition(pPos)
    , mLeavePosition(pPos.x + 2300)
{
    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_right"], {448, 127, 32, 49}, kScaleToTile);
    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], {32, 127, 32, 49}, kScaleToTile);
    mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Enemies_right"], {480, 143, 32, 32}, kScaleToTile);

    mCurrentPosition.x -= MarioGame::sSizeWindow.x / 2.0f + 100;
    mCurrentPosition.y = -500;
    mAnimations->Play(Animations::RightMove, mCurrentPosition);

    SetWhenLoadShoot();
}

///------
void CreatorRedTurtles::Update()
{
    if (!mEnable)
    {
        if (MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x > mActivationPosition.x)
        {
            mCurrentPosition.y = mActivationPosition.y;
            mEnable = true;
        }
    } else
    {
        Shoot();
        Move();
    }

    mAnimator->Update(mCurrentPosition);
}

///-----
void CreatorRedTurtles::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        if (pHowCollision == KindCollision::Bottom)
        {
            CorrectObjectPositionOnMe(*pObj, pHowCollision);
            pObj->ActOnMe(KindAction::Hop);

            this->ActOnMe(KindAction::Hit);
        }
        else
            pObj->ActOnMe(KindAction::Hit);
}

///-------
void CreatorRedTurtles::ActOnMe(KindAction pHowAction)
{
    if (pHowAction == KindAction::Hit)
        CreateBeatObjectEffect(-100.0f);
}

///-----
inline void CreatorRedTurtles::Shoot()
{
    if (mLeaveMap && !mLoadingShoot) return;

    if (mWhenLoadShoot < Scene::GetDurationScene())
    {
        if (!mLoadingShoot)
        {
            mAnimations->Play(Animations::Standard, mCurrentPosition);
            mLoadingShoot = true;
            SetWhenShoot();
        } else if (mWhenShoot < Scene::GetDurationScene())
        {
            mLoadingShoot = false;
            SetWhenLoadShoot();

            const bool reversalToMario = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x < mCurrentPosition.x;

            AddNewObject(new RedTurtleShell(mCurrentPosition, reversalToMario));

            if (reversalToMario)
                mAnimations->Play(Animations::LeftMove, mCurrentPosition);
            else
                mAnimations->Play(Animations::RightMove, mCurrentPosition);
        }
    }
}

///------
inline void CreatorRedTurtles::Move()
{
    const float marioPosition = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x;

    if (fabs(mCurrentPosition.x - marioPosition) > 400)
        mDoubleSpeed = 1.5f;
    else
        mDoubleSpeed = 1.0f;

    if (mRightDir)
    {
        if (mCurrentPosition.x >= mLeavePosition && mCurrentPosition.x < marioPosition)
        {
            mLeaveMap = true;
            mCurbing = true;
        }

        if (!mCurbing)
        {
            mSpeed += mValueAcceleration;
            if (mSpeed > mMaxSpeed * mDoubleSpeed)
                mSpeed = mMaxSpeed * mDoubleSpeed;

            if (mSetChangingDirPos && marioPosition < mCurrentPosition.x)
                SetChangeDirPosition();

            if (!mSetChangingDirPos && mCurrentPosition.x > mChangeDirPosition - mWhenCurbing)
                mCurbing = true;
        } else
        {
            mSpeed -= mValueAcceleration * 0.8;
            if (mSpeed < 0)
            {
                ChangeDirection();
                mSpeed = 0.0f;
            }
        }
    } else
    {
        if (!mCurbing)
        {
            mSpeed -= mValueAcceleration;
            if (mSpeed < -mMaxSpeed * mDoubleSpeed)
                mSpeed = -mMaxSpeed * mDoubleSpeed;

            if (mLeaveMap)
            {
                mCurrentPosition.y -= 0.5f;
                if (mCurrentPosition.y < -10)
                    RemoveObject(this);
            } else
            {
                if (mSetChangingDirPos && marioPosition > mCurrentPosition.x)
                    SetChangeDirPosition();

                if (!mSetChangingDirPos && mCurrentPosition.x < mChangeDirPosition + mWhenCurbing)
                    mCurbing = true;
            }
        } else
        {
            mSpeed += mValueAcceleration * 0.8;
            if (mSpeed > 0)
            {
                ChangeDirection();
                mSpeed = 0.0f;
            }
        }
    }

    mCurrentPosition.x += mSpeed;
}

///-------
inline void CreatorRedTurtles::SetChangeDirPosition()
{
    mSetChangingDirPos = false;

    if (mRightDir)
        mChangeDirPosition = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x + mRange;
    else
        mChangeDirPosition = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x - mRange;

    if (!mLoadingShoot)
        if (mRightDir)
            mAnimations->Play(Animations::LeftMove, mCurrentPosition);
        else
            mAnimations->Play(Animations::RightMove, mCurrentPosition);
}

///-------
inline void CreatorRedTurtles::ChangeDirection()
{
    mRightDir = !mRightDir;
    mCurbing = false;
    mSetChangingDirPos = true;
}

///-------
inline void CreatorRedTurtles::SetWhenLoadShoot()
{
    mWhenLoadShoot = Scene::GetDurationScene() + mIterativityLoadingShoot;
}

///-------
inline void CreatorRedTurtles::SetWhenShoot()
{
    mWhenShoot = Scene::GetDurationScene() + mLoadingShootTime;
}

///--------
void CreatorRedTurtles::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    if (mEnable)
        mAnimator->Draw(pWindow);
}
