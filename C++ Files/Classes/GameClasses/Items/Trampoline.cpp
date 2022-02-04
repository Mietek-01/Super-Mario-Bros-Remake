#include "Trampoline.h"

CTrampoline::CTrampoline(sf::Vector2f pos)
:CGameObject(new CAnimations,Parentage::ITEM,pos)
{
    vector<sf::IntRect> m_frame_animation;

    m_frame_animation.push_back(sf::IntRect(32,36,32,48));
    m_frame_animation.push_back(sf::IntRect(64,52,32,32));
    m_frame_animation.push_back(sf::IntRect(32,36,32,48));

    m_animations->create(MyAnimations::TRAMPOLINE_ACTIVATION,CMarioGame::s_texture_manager["Items"],m_frame_animation,m_speed,m_scale_to_tile);
    m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Items"],{0,20,32,64},m_scale_to_tile);

    m_animations->play(CAnimations::STANDARD,m_current_position);
}

///------
void CTrampoline::update()
{
    if(m_enable)
    {
        if(m_animations->islastFrame())
        {
            m_animations->play(CAnimations::STANDARD,m_current_position);
            m_enable=false;
        }
        else
            m_animations->update(m_current_position);
    }
}

///------
void CTrampoline::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(how_collision==KindCollision::BOTTOM)
    {
        CPhysicaltObject* obj_on_me=dynamic_cast<CPhysicaltObject*>(obj);

        if(obj_on_me)
        {
            this->corectObjectPositionOnMe(*obj,how_collision);
            activate(obj_on_me);
        }
    }
}

///--------
inline void CTrampoline::activate(CPhysicaltObject* obj_on_me)
{
    if(!m_enable)
    {
        m_enable=true;
        obj_on_me->hop(m_jump_force);
        m_animations->play(MyAnimations::TRAMPOLINE_ACTIVATION,m_current_position);

    }else
    switch(m_animations->getIndexFrame())
    {
        case 0:
        obj_on_me->hop(m_jump_force*0.7);
        break;

        case 1:
        obj_on_me->hop(m_jump_force*0.5);
        break;

        case 2:
        obj_on_me->hop(m_jump_force*0.7);
        break;
    }
}


