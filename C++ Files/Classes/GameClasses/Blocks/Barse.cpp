#include "../Enemies/Bowser.h"

CBowser::CBarse::CBarse(sf::Vector2f pos,bool active,bool go_up)
:CStaticBlock({0,0,32,720},pos,"Barse")
,m_active(active)
,m_go_up(go_up)
{
    m_current_position.y+=getBounds().height;
    m_previous_position=m_current_position;

    m_animator->setPosition(m_current_position);
}

///--------
void CBowser::CBarse::update()
{
    if(m_active)
    {
        m_previous_position=m_current_position;

        if(m_go_up)
        {
            m_current_position.y-=m_move_speed;

            if(m_current_position.y-getBounds().height<=0)
            {
                m_current_position.y=getBounds().height;
                m_active=false;
            }
        }
        else
        {
            m_current_position.y+=m_move_speed;

            if(m_current_position.y-getBounds().height>=getBounds().height)
            {
                m_current_position.y=getBounds().height*2.0f;
                m_active=false;
            }
        }

        m_animator->setPosition(m_current_position);
    }
}
