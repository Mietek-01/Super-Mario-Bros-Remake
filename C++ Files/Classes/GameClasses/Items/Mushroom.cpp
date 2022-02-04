#include "../Blocks/MysterBox.h"
#include "../../GUIClasses/FlowText.h"

CMysterBox::CMushroom::CMushroom(sf::Vector2f pos)
:CPhysicaltObject(new CSprite(CGUI::createSprite("Items",{128,150,32,32},pos,1.3f,true)),Parentage::ITEM,pos,KindMovement::STANDING,1000)
{
    m_right_dir_reversal=rand()%2+0;

    if(m_right_dir_reversal)
        m_kind_movement=KindMovement::RIGHT_RUN;
    else
        m_kind_movement=KindMovement::LEFT_RUN;

    m_value_acceleration=1.0f;
}

///-----
void CMysterBox::CMushroom::updateForCollisionWithBlock(KindCollision how_collision,CBlock* block)
{
    switch(how_collision)
    {
        case KindCollision::TOP:
        {
            m_force=0.0;
            break;
        }

        case KindCollision::BOTTOM:
        {
            if(block->isHit())
                actOnMe(KindAction::HOP);
            else
            {
                m_jump=false;
                m_in_bottom_collision=true;
            }

            break;
        }

        case KindCollision::RIGHT_SIDE:
        {
            m_kind_movement=KindMovement::LEFT_RUN;
            m_right_dir_reversal=false;
            break;
        }

        case KindCollision::LEFT_SIDE:
        {
            m_kind_movement=KindMovement::RIGHT_RUN;
            m_right_dir_reversal=true;
            break;
        }

        case KindCollision::NONE:
            {
                return;
            }
    }

    block->actOnObject(this,how_collision);
}

///----
void CMysterBox::CMushroom::actOnObject(CGameObject* obj,KindCollision kind_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
    {
        m_current_position=obj->getCurrentPosition();
        actOnMe(KindAction::HIT);

        obj->actOnMe(KindAction::LVL_UP);
    }
}

///----
void CMysterBox::CMushroom::actOnMe(KindAction which_action)
{
    switch(which_action)
    {

    case KindAction::HIT:
        {
            this->createPoints();
            removeObject(this);
            break;
        }
    case KindAction::HOP:
        {
            hop(-500.0f);
            break;
        }
    }
}
