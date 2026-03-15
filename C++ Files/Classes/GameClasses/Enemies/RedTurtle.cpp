#include "Enemies.h"
#include "../Mario.h"

#include <vector>

RedTurtle::RedTurtle(sf::Vector2f pPos, KindMovement pKindMovement, bool pCreatedByBowser)
    : PhysicalObject(new Animations, Parentage::Enemy, pPos, pKindMovement)
    , mCreatedByBowser(pCreatedByBowser)
{
    mValueAcceleration = 0.6f;

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({385, 82, 30, 30});
    frameAnimation.push_back({416, 84, 32, 28});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 2.5f, kScaleToTile);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({64, 84, 32, 28});
    frameAnimation.push_back({96, 82, 30, 30});

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.5f, kScaleToTile);

    if (mIsRightDirReversal)
        mAnimations->Play(Animations::RightMove, mCurrentPosition);
    else
        mAnimations->Play(Animations::LeftMove, mCurrentPosition);
}

///-----
void RedTurtle::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
    {
        Mario* mario = dynamic_cast<Mario*>(pObj);
        mario->SetDamage(mDamageValue);

        pObj->ActOnMe(KindAction::Hit);
    }
}

///-----
void RedTurtle::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    if (pWhichAction == KindAction::Hit)
        CreateBeatObjectEffect();
}

///---------
void RedTurtle::UpdateForCollisionWithBlock(KindCollision pHowCollision, Block* pBlock)
{
    if (mCreatedByBowser && pBlock->GetCurrentPosition().y < Scene::sTileSize * 3)
        return;

    PhysicalObject::UpdateForCollisionWithBlock(pHowCollision, pBlock);
}
