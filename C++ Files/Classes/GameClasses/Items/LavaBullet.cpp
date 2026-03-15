#include "LavaBullet.h"
#include "../PhysicalObject.h"

#include <vector>

const float LavaBullet::sMyGravitation = 1900;

LavaBullet::LavaBullet(sf::Vector2f pPos)
    : GameObject(new Animations, Parentage::Item, {pPos.x, pPos.y + Scene::sTileSize + 10})
    , mBasicPosition(mCurrentPosition)
{
    std::vector<sf::IntRect> frameAnimation;

    frameAnimation.push_back({194, 80, 28, 32});
    frameAnimation.push_back({226, 80, 28, 32});
    frameAnimation.push_back({258, 80, 28, 32});

    mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.5f, kScaleToTile);
    mAnimations->Create(MyAnimations::GoDown, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.5f, kScaleToTile);

    mAnimations->GetSprite().setRotation(180);
    mAnimations->SetIndexFrame(1, mCurrentPosition);

    mAnimations->GetSprite().setRotation(180);
    mAnimations->SetIndexFrame(2, mCurrentPosition);

    mAnimations->GetSprite().setRotation(180);

    SetWhenJump();
}

///------
void LavaBullet::Update()
{
    if (mBasicPosition.y > mCurrentPosition.y || mWhenJump < Scene::GetDurationScene())
        Move();

    mAnimator->Update(mCurrentPosition);
}

///------
void LavaBullet::Move()
{
    mPreviousPosition = mCurrentPosition;

    const float verticalVelocity = mForce * Scene::GetFrameTime();

    mForce += sMyGravitation * Scene::GetFrameTime();
    mCurrentPosition.y += verticalVelocity;

    if (mGoUp && verticalVelocity > 0)
    {
        mGoUp = false;
        mCurrentPosition.y -= GetBounds().height;
        mCurrentPosition.x -= 3;

        mAnimations->Play(MyAnimations::GoDown, mCurrentPosition);
    }

    if (!mGoUp && mCurrentPosition.y > mBasicPosition.y)
        SetWhenJump();
}

///-----
void LavaBullet::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
        pObject->ActOnMe(KindAction::Hit);
}

///------
void LavaBullet::SetWhenJump()
{
    mWhenJump = Scene::GetDurationScene() + rand() % 3 + 1;
    mForce = mJumpForce;
    mGoUp = true;

    mCurrentPosition = mBasicPosition;

    mAnimations->Play(Animations::Standard, mCurrentPosition);
}
