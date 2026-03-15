#include "FireballBullets.h"
#include "../Shield.h"
#include "../../MarioGame.h"

#include <cmath>
#include <vector>

///-----------------Bullet------------///

Bullet::Bullet(sf::Vector2f pPos, bool pRightDir, float pSpeed, float pScale)
    : GameObject(new Animations, Parentage::Item, pPos)
    , mRightDir(pRightDir)
    , mSpeed(pSpeed)
    , mMyScale(pScale)
    , mBasicPosition(pPos)
{
    MarioGame::sSoundManager.play("bowser_fire");
}

///------
void Bullet::Update()
{
    mPreviousPosition = mCurrentPosition;

    if (mRightDir)
    {
        if (mCurrentPosition.x > mBasicPosition.x + mRange && mAnimations->IsLastFrame())
            ActOnMe(KindAction::Hit);

        mCurrentPosition.x += mSpeed;

    } else
    {
        if (mCurrentPosition.x < mBasicPosition.x - mRange && mAnimations->IsLastFrame())
            ActOnMe(KindAction::Hit);

        mCurrentPosition.x -= mSpeed;
    }

    this->CorrectPosition();

    mAnimations->Update(mCurrentPosition);
}

///--------
void Bullet::ActOnMe(KindAction pAction)
{
    if (pAction == KindAction::Hit)
        RemoveObject(this);
}

///----------
void Bullet::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
    {
        Shield* objWithShield = dynamic_cast<Shield*>(pObject);

        if (objWithShield)
            objWithShield->SetDamage(mDamageValue);

        pObject->ActOnMe(KindAction::Hit);
        ActOnMe(KindAction::Hit);
    }
}

///---------------------BasicBullet---------------///
BasicBullet::BasicBullet(sf::Vector2f pPos, bool pRightDir)
    : Bullet(pPos, pRightDir, rand() % 3 + 2, GameObject::kScaleToTile * 1.4f)
{
    std::vector<sf::IntRect> frameAnimation;

    if (pRightDir)
    {
        frameAnimation.push_back({7, 370, 20, 12});
        frameAnimation.push_back({35, 368, 25, 16});
        frameAnimation.push_back({66, 365, 30, 22});
        frameAnimation.push_back({97, 364, 32, 26});

        mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 2.5f, mMyScale);
    } else
    {
        frameAnimation.push_back({526, 370, 20, 12});
        frameAnimation.push_back({493, 368, 25, 16});
        frameAnimation.push_back({457, 365, 30, 22});
        frameAnimation.push_back({425, 364, 32, 26});

        mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 2.5f, mMyScale);
    }

    mAnimations->Play(Animations::Standard, mCurrentPosition);
}

///---------------------CircleBullet---------------///
CircleBullet::CircleBullet(sf::Vector2f pPos, bool pRightDir)
    : Bullet(pPos, pRightDir, 3, GameObject::kScaleToTile)
    , mCenterCircleXPos(pPos.x)
{
    std::vector<sf::IntRect> frameAnimation;

    frameAnimation.push_back({194, 80, 28, 32});
    frameAnimation.push_back({226, 80, 28, 32});
    frameAnimation.push_back({258, 80, 28, 32});

    mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.5f, mMyScale);

    CorrectPosition();

    mAnimations->Play(Animations::Standard, mCurrentPosition);
}

///-----
void CircleBullet::Update()
{
    mPreviousPosition = mCurrentPosition;

    if (mRightDir)
        if (mCurrentPosition.x > mBasicPosition.x + mRange && mAnimations->IsLastFrame())
            ActOnMe(KindAction::Hit);
    else
        if (mCurrentPosition.x < mBasicPosition.x - mRange && mAnimations->IsLastFrame())
            ActOnMe(KindAction::Hit);

    CorrectPosition();
}

///-------
void CircleBullet::CorrectPosition()
{
    mAngle += mSpinSpeed;
    mRotate += mRotateSpeed;

    if (mRightDir)
        mCenterCircleXPos += mSpeed;
    else
        mCenterCircleXPos -= mSpeed;

    mCurrentPosition.y = mBasicPosition.y - 5.0f * mRadius * sin(mAngle);
    mCurrentPosition.x = mCenterCircleXPos - 5.0f * mRadius * cos(mAngle);

    mAnimations->Update(mCurrentPosition);
    mAnimations->GetSprite().setRotation(mRotate);
}

///---------------------SinBullet---------------///

SinBullet::SinBullet(sf::Vector2f pPos, bool pRightDir)
    : Bullet(pPos, pRightDir, 2.5, GameObject::kScaleToTile * 1.4f)
{
    std::vector<sf::IntRect> frameAnimation;

    if (pRightDir)
    {
        frameAnimation.push_back({7, 370, 20, 12});
        frameAnimation.push_back({35, 368, 25, 16});
        frameAnimation.push_back({66, 365, 30, 22});
        frameAnimation.push_back({97, 364, 32, 26});

        mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 2.5f, mMyScale);
    } else
    {
        frameAnimation.push_back({526, 370, 20, 12});
        frameAnimation.push_back({493, 368, 25, 16});
        frameAnimation.push_back({457, 365, 30, 22});
        frameAnimation.push_back({425, 364, 32, 26});

        mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 2.5f, mMyScale);
    }

    mAnimations->Play(Animations::Standard, mCurrentPosition);
}

///------
void SinBullet::CorrectPosition()
{
    if (mRightDir)
        mMySpeed += mSinFunctionSpeed;
    else
        mMySpeed -= mSinFunctionSpeed;

    mCurrentPosition.y = mBasicPosition.y + (45.0f * sin(mMySpeed));
}
