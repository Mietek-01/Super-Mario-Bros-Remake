#include "Gun.h"
#include "../Mario.h"
#include "../../MarioGame.h"

#include <vector>

Gun::GunBullet::GunBullet(sf::Vector2f pPos, bool pRightDir)
    : GameObject(new Animations, Parentage::Item, pPos, 100)
    , mRightDir(pRightDir)
{
    std::vector<sf::IntRect> frameAnimation;
    const int additionalLength = 100 * rand() % 5 + 2;

    if (pRightDir)
    {
        frameAnimation.push_back({416, 114, 32, 29});
        frameAnimation.push_back({416, 146, 32, 29});
        frameAnimation.push_back({416, 178, 32, 29});

        mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 1.5f, kScaleToTile);

        mRange += mCurrentPosition.x + additionalLength;

    } else
    {
        frameAnimation.push_back({64, 114, 32, 29});
        frameAnimation.push_back({64, 146, 32, 29});
        frameAnimation.push_back({64, 178, 32, 29});

        mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);

        mRange = mCurrentPosition.x - mRange - additionalLength;
    }

    mCurrentPosition.y += GetBounds().height + 2;

    mAnimations->SetPosition(mCurrentPosition);
}

///----------
void Gun::GunBullet::Update()
{
    mPreviousPosition = mCurrentPosition;

    if ((mRightDir && mCurrentPosition.x > mRange) ||
        (!mRightDir && mCurrentPosition.x < mRange))
    {
        mTheEndRange = true;
        ActOnMe(KindAction::Hit);
    }
    else
    {
        if (mRightDir)
            mCurrentPosition.x += mSpeed;
        else
            mCurrentPosition.x -= mSpeed;
    }

    mAnimations->Update(mCurrentPosition);
}

///--------
void Gun::GunBullet::ActOnMe(KindAction pAction)
{
    if (pAction == KindAction::Hit)
    {
        sf::Sprite* sprite = new sf::Sprite(mAnimations->GetSprite());

        GuiObject* obj = new SpecialEffects(new SpriteAnimator(sprite), SpecialEffects::Jump, MarioGame::sSizeWindow.y + Scene::sTileSize, {0, 10});
        Gui::AddGuiObject(obj);

        if (!mTheEndRange)
            CreatePoints();

        RemoveObject(this);
    }
}

///----------
void Gun::GunBullet::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
        if (pCollision == KindCollision::Bottom)
        {
            CorrectObjectPositionOnMe(*pObject, pCollision);
            pObject->ActOnMe(KindAction::Hop);

            this->ActOnMe(KindAction::Hit);

        }
        else
            pObject->ActOnMe(KindAction::Hit);
}
