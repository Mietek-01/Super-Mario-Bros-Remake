#include "../Mario.h"
#include "../Shield.h"
#include "../Enemies/Enemies.h"

#include <vector>

Mario::Bullet::Bullet(sf::Vector2f pPos, bool pIsRight, bool pIsFast)
    : PhysicalObject(new Animations, Parentage::Item, pPos)
{
    mJumpForce = -400.0f;
    mRange = pPos.x;

    if (pIsRight)
    {
        mRange += mLengthRange;
        mKindMovement = KindMovement::RightRun;
        mIsRightDirReversal = true;

    } else
    {
        mRange -= mLengthRange;
        mKindMovement = KindMovement::LeftRun;
        mIsRightDirReversal = false;
    }

    if (pIsFast)
    {
        mValueAcceleration = 5.0f;
        mRange *= 1.2f;
    }
    else
        mValueAcceleration = 3.5f;

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({0, 1, 16, 14});
    frameAnimation.push_back({17, 0, 14, 16});

    mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.3f, kScaleToTile);

    mAnimations->SetPosition(mCurrentPosition);
}

///----------
void Mario::Bullet::Update()
{
    if ((mIsRightDirReversal && mCurrentPosition.x > mRange) ||
        (!mIsRightDirReversal && mCurrentPosition.x < mRange))
        RemoveObject(this);
    else
        PhysicalObject::Update();
}

///--------
void Mario::Bullet::ActOnMe(KindAction pAction)
{
    if (mIsDead) return;

    if (pAction == KindAction::Hit)
    {
        Animations* hitAnimation = new Animations;

        std::vector<sf::IntRect> frameAnimation;
        frameAnimation.push_back({35, 4, 8, 8});
        frameAnimation.push_back({49, 1, 13, 14});
        frameAnimation.push_back({62, 0, 17, 16});

        hitAnimation->Create(Animations::Standard, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.5f, kScaleToTile);
        hitAnimation->SetPosition(mCurrentPosition);

        GuiObject* obj = new SpecialEffects(hitAnimation, SpecialEffects::OneLoopAnimation, 0.0f);
        Gui::AddGuiObject(obj);

        RemoveObject(this);
    }
}

///------------------
void Mario::Bullet::UpdateForCollisionWithBlock(KindCollision pCollision, Block* pBlock)
{
    switch (pCollision)
    {
        case KindCollision::Top:
        {
            break;
        }

        case KindCollision::Bottom:
        {
            Jump();
            break;
        }

        case KindCollision::RightSide:
        {
            this->ActOnMe(KindAction::Hit);
            return;
        }

        case KindCollision::LeftSide:
        {
            this->ActOnMe(KindAction::Hit);
            return;
        }

        case KindCollision::None:
        {
            return;
        }
    }

    pBlock->ActOnObject(this, pCollision);
}

///----------
void Mario::Bullet::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Enemy)
    {
        Shield* objWithShield = dynamic_cast<Shield*>(pObject);

        if (objWithShield)
            objWithShield->SetDamage(mDamageValue);

        if (!dynamic_cast<ArmoredTurtle*>(pObject))
            pObject->ActOnMe(KindAction::Hit);

        this->ActOnMe(KindAction::Hit);
    }
}
