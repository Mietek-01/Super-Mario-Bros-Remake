#include "Enemies.h"

#include <vector>

ArmoredTurtle::ArmoredTurtle(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Enemy, pPos)
{
    mValueAcceleration = 0.3f;

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({352, 0, 30, 32});
    frameAnimation.push_back({384, 2, 32, 30});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 2.5f, kScaleToTile);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({96, 2, 32, 30});
    frameAnimation.push_back({130, 0, 30, 32});

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.5f, kScaleToTile);

    mAnimations->Play(Animations::LeftMove, mCurrentPosition);
}

///-----
void ArmoredTurtle::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        if (pHowCollision == KindCollision::Bottom)
        {
            this->CorrectObjectPositionOnMe(*pObj, pHowCollision);
            pObj->ActOnMe(KindAction::Hop);
        } else
            pObj->ActOnMe(KindAction::Hit);
}

///-----
void ArmoredTurtle::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    if (pWhichAction == KindAction::Hit)
        CreateBeatObjectEffect();
}
