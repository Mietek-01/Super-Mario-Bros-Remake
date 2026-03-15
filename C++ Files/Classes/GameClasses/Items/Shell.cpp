#include "Shell.h"

#include <vector>

Shell::Shell(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Item, {pPos.x, pPos.y - PhysicalObject::sCorrectionToBottomCollision}, KindMovement::RightRun)
{
    mValueAcceleration = 2.0f;

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back(sf::IntRect(65, 48, 30, 26));
    frameAnimation.push_back(sf::IntRect(97, 48, 32, 26));
    frameAnimation.push_back(sf::IntRect(128, 48, 30, 26));
    frameAnimation.push_back(sf::IntRect(159, 48, 32, 26));

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);
    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);

    mAnimations->SetPosition(mCurrentPosition);
}

///------
void Shell::Update()
{
    if (mActive)
        PhysicalObject::Update();
    else
    {
        if (mIsJump)
        {
            MakeJump();
            mAnimator->SetPosition(mCurrentPosition);
        }
    }
}

///----------
void Shell::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    switch (pObject->GetParentage())
    {
        case Parentage::Mario:
        {
            if (pCollision == KindCollision::Bottom)
            {
                mActive = !mActive;
                this->CorrectObjectPositionOnMe(*pObject, pCollision);
                pObject->ActOnMe(KindAction::Hop);
            } else
                if (mActive)
                    pObject->ActOnMe(KindAction::Hit);

            break;
        }
        case Parentage::Item:
        {
            if (mActive)
            {
                Shell* shell = dynamic_cast<Shell*>(pObject);

                if (shell)
                    shell->ActOnMe(KindAction::Hit);
            }

            break;
        }
        case Parentage::Enemy:
        {
            if (mActive)
                pObject->ActOnMe(KindAction::Hit);
            break;
        }
    }
}

///---------
void Shell::ActOnMe(KindAction pAction)
{
    if (mIsDead) return;

    if (pAction == KindAction::Hit)
        this->CreateBeatObjectEffect();
}
