#include "Enemies.h"

#include <vector>

const float FlyingTurtle::sMyGravitation = 1000.0f;

FlyingTurtle::FlyingTurtle(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Enemy, pPos, PhysicalObject::KindMovement::LeftRun, 300)
{
    mValueAcceleration = 0.8f;
    mJumpForce = -550;

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({224, 34, 32, 46});
    frameAnimation.push_back({257, 32, 31, 48});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 1.5f, kScaleToTile);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({224, 32, 31, 48});
    frameAnimation.push_back({256, 34, 32, 46});

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);

    mAnimations->Play(Animations::LeftMove, mCurrentPosition);
}

///------
void FlyingTurtle::MakeJump()
{
    mForce += sMyGravitation * Scene::GetFrameTime();
    mCurrentPosition.y += mForce * Scene::GetFrameTime();

    CheckUnderMap();
}

///----------
void FlyingTurtle::UpdateForCollisionWithBlock(KindCollision pHowCollision, Block* pBlock)
{
    switch (pHowCollision)
    {
    case KindCollision::Top:
        {
            mForce = 0.0f;
            break;
        }

    case KindCollision::Bottom:
        {
            if (pBlock->IsHit() || pBlock->IsDead())
                ActOnMe(KindAction::Hit);
            else
                Jump();

            break;
        }

    case KindCollision::RightSide:
        {
            mKindMovement = KindMovement::LeftRun;
            mIsRightDirReversal = false;
            mAnimations->Play(Animations::LeftMove, mCurrentPosition);
            break;
        }

    case KindCollision::LeftSide:
        {
            mKindMovement = KindMovement::RightRun;
            mIsRightDirReversal = true;
            mAnimations->Play(Animations::RightMove, mCurrentPosition);
            break;
        }

    case KindCollision::None:
        {
            return;
        }
    }

    pBlock->ActOnObject(this, pHowCollision);
}

///-----
void FlyingTurtle::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        if (pHowCollision == KindCollision::Bottom)
        {
            RemoveObject(this);

            Turtle* newTurtle = new Turtle(mCurrentPosition, true, mKindMovement);
            AddNewObject(newTurtle);

            this->CorrectObjectPositionOnMe(*pObj, pHowCollision);
            pObj->ActOnMe(KindAction::Hop);
        }
        else
            pObj->ActOnMe(KindAction::Hit);
}

///-----
void FlyingTurtle::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    if (pWhichAction == KindAction::Hit)
        CreateBeatObjectEffect();
}
