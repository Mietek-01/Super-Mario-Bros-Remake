#include "../Enemies/ArmedTurtle.h"
#include "../../Scens/GameScen.h"

ArmedTurtle::Hammer::CHammer(sf::Vector2f pPos, bool pRightDir)
    : GameObject(new SpriteAnimator(nullptr), Parentage::Item, pPos)
    , mIsRightDirReversal(!pRightDir)
{
    changeReversal(pPos);
}

///-------
void ArmedTurtle::Hammer::ChangeReversal(sf::Vector2f pNewPos)
{
    mPreviousPosition = mCurrentPosition = pNewPos;
    mIsRightDirReversal = !mIsRightDirReversal;

    if (mIsRightDirReversal)
        mAnimator.reset(new SpriteAnimator(Gui::CreateSprite("Enemies_left", {102, 113, 22, 35}, mCurrentPosition, kScaleToTile, true)));
    else
        mAnimator.reset(new SpriteAnimator(Gui::CreateSprite("Enemies_right", {387, 113, 22, 35}, mCurrentPosition, kScaleToTile, true)));
}

///-------
void ArmedTurtle::Hammer::Update()
{
    if (mEnabled)
    {
        mPreviousPosition = mCurrentPosition;

        mCurrentPosition.x += mHorizontalAcceleration;

        mForce += PhysicalObject::sGravitation * Scene::GetFrameTime();
        mCurrentPosition.y += mForce * Scene::GetFrameTime();

        mAnimator->GetSprite().rotate(mValueRotate);

        mAnimator->Update(mCurrentPosition);

        CheckUnderMap();
    }
}

///------
void ArmedTurtle::Hammer::SetEnabled(bool pStepUpForce)
{
    mEnabled = true;

    if (pStepUpForce)
        mForce *= 1.2f;

    if (mIsRightDirReversal)
    {
        mHorizontalAcceleration = -mHorizontalAcceleration;
        mValueRotate = -mValueRotate;
    }
}

///----
void ArmedTurtle::Hammer::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
        pObject->ActOnMe(KindAction::Hit);
}
