#include "Enemies.h"
#include "../../Scens/GameScen.h"
#include "../Items/Shell.h"

#include <vector>

Turtle::Turtle(sf::Vector2f pPos, bool pCreateByFlyingTurtle, KindMovement pDirection)
    : PhysicalObject(new Animations, Parentage::Enemy, pPos, pDirection, 200)
    , mCreatedByFlyingTurtle(pCreateByFlyingTurtle)
{
    mValueAcceleration = 0.5f;

    std::vector<sf::IntRect> frameAnimation;

    frameAnimation.push_back({480, 35, 32, 45});
    frameAnimation.push_back({450, 33, 30, 47});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 2.0f, kScaleToTile);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back(sf::IntRect(0, 35, 32, 45));
    frameAnimation.push_back(sf::IntRect(32, 33, 30, 47));

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.0f, kScaleToTile);

    if (mIsRightDirReversal)
        mAnimations->Play(Animations::RightMove, mCurrentPosition);
    else
        mAnimations->Play(Animations::LeftMove, mCurrentPosition);
}

///----
void Turtle::Update()
{
    if (!mCreatedByFlyingTurtle)
        DontFall();

    PhysicalObject::Update();
}

///----
inline void Turtle::DontFall()
{
    mFuturePosition = GetBounds();

    if (mKindMovement == KindMovement::LeftRun)
        mFuturePosition.left -= GetBounds().width;
    else
        mFuturePosition.left += GetBounds().width;

    const auto gameSceneBlocks = MarioGame::Instance().GetScene<GameScene>().GetBlocks();

    for (const auto& block : *gameSceneBlocks)
    {
        if (!block->IsVisible()) continue;

        if (mFuturePosition.intersects(block->GetBounds()))
        {
            mCloseToFall = false;
            break;
        }
    }

    if (mCloseToFall && !mIsJump)
    {
        if (mIsRightDirReversal)
        {
            mIsRightDirReversal = false;
            mKindMovement = KindMovement::LeftRun;
            mAnimations->Play(Animations::LeftMove, mCurrentPosition);
        } else
        {
            mIsRightDirReversal = true;
            mKindMovement = KindMovement::RightRun;
            mAnimations->Play(Animations::RightMove, mCurrentPosition);
        }
    } else
        mCloseToFall = true;
}

///-----
void Turtle::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        if (pHowCollision == KindCollision::Bottom)
        {
            this->ActOnMe(KindAction::Crumpled);
            this->CorrectObjectPositionOnMe(*pObj, pHowCollision);

            pObj->ActOnMe(KindAction::Hop);
        }
        else
            pObj->ActOnMe(KindAction::Hit);
}

///-----
void Turtle::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    switch (pWhichAction)
    {
    case KindAction::Crumpled:
        {
            AddNewObject(new Shell(mCurrentPosition));

            CreatePoints();

            MarioGame::sSoundManager.play("stomp");

            RemoveObject(this);

            break;
        }
    case KindAction::Hit:
        {
            CreateBeatObjectEffect();
            break;
        }
    }
}
