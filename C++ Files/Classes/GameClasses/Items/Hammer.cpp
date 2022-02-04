#include "../Enemies/ArmedTurtle.h"
#include "../../Scens/GameScen.h"

CArmedTurtle::CHammer::CHammer(sf::Vector2f pos,bool right_dir)
:CGameObject(new CSprite(nullptr),Parentage::ITEM,pos)
,m_right_dir_reversal(!right_dir)
{
    changeReversal(pos);
}

///-------
void CArmedTurtle::CHammer::changeReversal(sf::Vector2f new_pos)
{
    m_previous_position=m_current_position=new_pos;
    m_right_dir_reversal=!m_right_dir_reversal;

    if(m_right_dir_reversal)
        m_animator.reset(new CSprite(CGUI::createSprite("Enemies_left",{102,113,22,35},m_current_position,m_scale_to_tile,true)));
    else
        m_animator.reset(new CSprite(CGUI::createSprite("Enemies_right",{387,113,22,35},m_current_position,m_scale_to_tile,true)));
}

///-------
void CArmedTurtle::CHammer::update()
{
    if(m_enabled)
    {
        m_previous_position=m_current_position;

        m_current_position.x+=m_horizontal_acceleration;

        m_force+=CPhysicaltObject::s_gravitation*CScen::getFrameTime();
        m_current_position.y+=m_force*CScen::getFrameTime();

        m_animator->getSprite().rotate(m_value_rotate);

        m_animator->update(m_current_position);

        isUnderMap();
    }
}

///------
void CArmedTurtle::CHammer::setEnabled(bool step_up_force)
{
    m_enabled=true;

    /// POZWALA ZWIEKSZYC SILE GDY ZOLW SKACZE
    if(step_up_force)
        m_force*=1.2f;

    /// BO DANE SA USTAWIONE DLA LEWEGO ZWROTU
    if(m_right_dir_reversal)
    {
        m_horizontal_acceleration=-m_horizontal_acceleration;
        m_value_rotate=-m_value_rotate;
    }
}

///----
void CArmedTurtle::CHammer::actOnObject(CGameObject* obj,KindCollision kind_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        obj->actOnMe(KindAction::HIT);
}



