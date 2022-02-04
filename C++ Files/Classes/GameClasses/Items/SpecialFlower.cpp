#include "../Blocks/MysterBox.h"
#include "../Mario.h"

CMysterBox::CSpecialFlower::CSpecialFlower(sf::Vector2f pos)
:CGameObject(new CSprite(CGUI::createSprite("Items",{32,212,32,32},pos,1.3f,true)),Parentage::ITEM,pos,1000)
{
}

///----
void CMysterBox::CSpecialFlower::actOnObject(CGameObject* obj,KindCollision kind_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
    {
        m_current_position=obj->getCurrentPosition();
        actOnMe(KindAction::HIT);

        CMario * mario=dynamic_cast<CMario*>(obj);

        mario->actOnMe(KindAction::LVL_UP);
        mario->setShield();
    }
}

///----
void CMysterBox::CSpecialFlower::actOnMe(KindAction which_action)
{
    if(which_action == KindAction::HIT)
    {
        this->createPoints();
        removeObject(this);
    }
}
