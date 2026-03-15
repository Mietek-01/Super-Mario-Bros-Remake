#include "../Blocks/MysterBox.h"
#include "../../GUIClasses/FlowText.h"

MysteryBox::Mushroom::Mushroom(sf::Vector2f pPos)
    : PhysicalObject(new SpriteAnimator(Gui::CreateSprite("Items", {128, 150, 32, 32}, pPos, 1.3f, true)), Parentage::Item, pPos, KindMovement::Standing, 1000)
{
    mIsRightDirReversal = rand() % 2 + 0;

    if (mIsRightDirReversal)
        mKindMovement = KindMovement::RightRun;
    else
        mKindMovement = KindMovement::LeftRun;

    mValueAcceleration = 1.0f;
}

///-----
void MysteryBox::Mushroom::UpdateForCollisionWithBlock(KindCollision pCollision, Block* pBlock)
{
    switch (pCollision)
    {
        case KindCollision::Top:
        {
            mForce = 0.0;
            break;
        }

        case KindCollision::Bottom:
        {
            if (pBlock->IsHit())
                ActOnMe(KindAction::Hop);
            else
            {
                mIsJump = false;
                mIsInBottomCollision = true;
            }

            break;
        }

        case KindCollision::RightSide:
        {
            mKindMovement = KindMovement::LeftRun;
            mIsRightDirReversal = false;
            break;
        }

        case KindCollision::LeftSide:
        {
            mKindMovement = KindMovement::RightRun;
            mIsRightDirReversal = true;
            break;
        }

        case KindCollision::None:
        {
            return;
        }
    }

    pBlock->ActOnObject(this, pCollision);
}

///----
void MysteryBox::Mushroom::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
    {
        mCurrentPosition = pObject->GetCurrentPosition();
        ActOnMe(KindAction::Hit);

        pObject->ActOnMe(KindAction::LevelUp);
    }
}

///----
void MysteryBox::Mushroom::ActOnMe(KindAction pAction)
{
    switch (pAction)
    {

    case KindAction::Hit:
    {
        this->CreatePoints();
        RemoveObject(this);
        break;
    }
    case KindAction::Hop:
    {
        Hop(-500.0f);
        break;
    }
    }
}
