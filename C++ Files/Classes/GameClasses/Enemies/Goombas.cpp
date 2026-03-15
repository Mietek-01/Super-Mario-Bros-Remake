#include "Enemies.h"
#include "../Mario.h"

#include <vector>

Goombas::Goombas(sf::Vector2f pPos, KindMovement pKindMovement, bool pCreatedByBowser)
    : PhysicalObject(new Animations, Parentage::Enemy, pPos, pKindMovement)
    , mCreatedByBowser(pCreatedByBowser)
{
    mValueAcceleration = 1.0f;

    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({0, 0, 32, 32});
    frameAnimation.push_back({32, 0, 32, 32});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);
    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);

    mAnimations->Play(Animations::LeftMove, mCurrentPosition);
}

///-----
void Goombas::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        if (pHowCollision == KindCollision::Bottom)
        {
            this->ActOnMe(KindAction::Crumpled);
            this->CorrectObjectPositionOnMe(*pObj, pHowCollision);

            pObj->ActOnMe(KindAction::Hop);
        }
        else
        {
            Mario* mario = dynamic_cast<Mario*>(pObj);
            mario->SetDamage(mDamageValue);

            pObj->ActOnMe(KindAction::Hit);
        }
}

///-----
void Goombas::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    switch (pWhichAction)
    {
    case KindAction::Crumpled:
        {
            if (mIsJump)
                CreateBeatObjectEffect(-100.0f);
            else
            {
                sf::Sprite* crumpledGoombas = Gui::CreateSprite("Enemies_left", {64, 16, 32, 16}, mCurrentPosition, kScaleToTile, true);

                GuiObject* obj = new SpecialEffects(new SpriteAnimator(crumpledGoombas), SpecialEffects::KindUpdate::STANDING, Scene::GetDurationScene() + 3.0f);
                Gui::AddGuiObject(obj);

                CreatePoints();

                MarioGame::sSoundManager.play("stomp");

                RemoveObject(this);
            }

            break;
        }

    case KindAction::Hit:
        {
            CreateBeatObjectEffect();
            break;
        }
    }
}

///---------
void Goombas::UpdateForCollisionWithBlock(KindCollision pHowCollision, Block* pBlock)
{
    if (mCreatedByBowser && pBlock->GetCurrentPosition().y < Scene::sTileSize * 3)
        return;

    PhysicalObject::UpdateForCollisionWithBlock(pHowCollision, pBlock);
}
