#include "Trampoline.h"

#include <vector>

Trampoline::Trampoline(sf::Vector2f pPos)
    : GameObject(new Animations, Parentage::Item, pPos)
{
    std::vector<sf::IntRect> frameAnimation;

    frameAnimation.push_back(sf::IntRect(32, 36, 32, 48));
    frameAnimation.push_back(sf::IntRect(64, 52, 32, 32));
    frameAnimation.push_back(sf::IntRect(32, 36, 32, 48));

    mAnimations->Create(MyAnimations::TrampolineActivation, MarioGame::sTextureManager["Items"], frameAnimation, mSpeed, kScaleToTile);
    mAnimations->Create(Animations::Standard, MarioGame::sTextureManager["Items"], {0, 20, 32, 64}, kScaleToTile);

    mAnimations->Play(Animations::Standard, mCurrentPosition);
}

///------
void Trampoline::Update()
{
    if (mEnabled)
    {
        if (mAnimations->IsLastFrame())
        {
            mAnimations->Play(Animations::Standard, mCurrentPosition);
            mEnabled = false;
        }
        else
            mAnimations->Update(mCurrentPosition);
    }
}

///------
void Trampoline::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pCollision == KindCollision::Bottom)
    {
        PhysicalObject* objOnMe = dynamic_cast<PhysicalObject*>(pObject);

        if (objOnMe)
        {
            this->CorrectObjectPositionOnMe(*pObject, pCollision);
            Activate(objOnMe);
        }
    }
}

///--------
inline void Trampoline::Activate(PhysicalObject* pObjectOnMe)
{
    if (!mEnabled)
    {
        mEnabled = true;
        pObjectOnMe->Hop(mJumpForce);
        mAnimations->Play(MyAnimations::TrampolineActivation, mCurrentPosition);

    } else
    switch (mAnimations->GetIndexFrame())
    {
        case 0:
        pObjectOnMe->Hop(mJumpForce * 0.7);
        break;

        case 1:
        pObjectOnMe->Hop(mJumpForce * 0.5);
        break;

        case 2:
        pObjectOnMe->Hop(mJumpForce * 0.7);
        break;
    }
}
