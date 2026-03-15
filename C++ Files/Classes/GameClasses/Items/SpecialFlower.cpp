#include "../Blocks/MysterBox.h"
#include "../Mario.h"

MysteryBox::SpecialFlower::SpecialFlower(sf::Vector2f pPos)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("Items", {32, 212, 32, 32}, pPos, 1.3f, true)), Parentage::Item, pPos, 1000)
{
}

///----
void MysteryBox::SpecialFlower::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
    {
        mCurrentPosition = pObject->GetCurrentPosition();
        ActOnMe(KindAction::Hit);

        Mario* mario = dynamic_cast<Mario*>(pObject);

        mario->ActOnMe(KindAction::LevelUp);
        mario->SetShield();
    }
}

///----
void MysteryBox::SpecialFlower::ActOnMe(KindAction pAction)
{
    if (pAction == KindAction::Hit)
    {
        this->CreatePoints();
        RemoveObject(this);
    }
}
