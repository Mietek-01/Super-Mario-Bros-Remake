#include "../Blocks/Pipe.h"
#include <time.h>

const float CPipe::CFlower::s_how_long_gnawing=3.5f;
const float CPipe::CFlower::s_speed=0.9;

CPipe::CFlower::CFlower(CPipe *pipe,sf::Vector2f pos)
:CGameObject(new CAnimations,Parentage::ENEMY,pos)
,m_how_long_hiding(rand()%4+3)
,m_my_pipe(pipe)
,m_pos_gnawing(pipe->getBounds().top)
{
    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({0,80,32,48});
    m_frame_animation.push_back({32,80,32,48});
    
    m_animations->create(MyKindsAnimations::GNAWING,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.1f,m_scale_to_tile);
    m_animations->create(MyKindsAnimations::MOVE_DOWN,CMarioGame::s_texture_manager["Enemies_left"],{0,80,32,48},m_scale_to_tile);
    m_animations->create(MyKindsAnimations::MOVE_UP,CMarioGame::s_texture_manager["Enemies_left"],{32,80,32,48},m_scale_to_tile);

    m_current_position.y += getBounds().height;
    m_animations->play(MyKindsAnimations::MOVE_UP, m_current_position);

    m_when_move_upper=rand()%3+1;
}

///-------------
void CPipe::CFlower::update()
{
    switch(m_state)
    {
        case State::GNAWING:
            {
                if(m_time_gnawing<CScen::getDurationScen()&&m_animations->islastFrame())
                {
                    m_state=State::MOVE_DOWN;
                    m_animations->play(MyKindsAnimations::MOVE_DOWN,m_current_position);
                }

                break;
            }
        case State::HIDING:
            {
                if(m_when_move_upper<CScen::getDurationScen())
                {
                    m_state=State::MOVE_UP;
                    m_enabled=true;
                }

                break;
            }

        case State::MOVE_DOWN:
            {
                m_current_position.y+=s_speed;

                if (getBounds().top > m_pos_gnawing)
                    reset();

                break;
            }

        case State::MOVE_UP:
            {
                m_current_position.y-=s_speed;

                if(m_current_position.y<m_pos_gnawing)
                {
                    m_current_position.y = m_pos_gnawing;
                    m_state=State::GNAWING;
                    m_time_gnawing=CScen::getDurationScen()+s_how_long_gnawing;
                    m_animations->play(MyKindsAnimations::GNAWING,m_current_position);

                }

                break;
            }
    }

    m_animations->update(m_current_position);
}

///-------------
void CPipe::CFlower::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(!m_enabled)
        return;

    if(obj->getParentage()==Parentage::MARIO)
      obj->actOnMe(KindAction::HIT);
}

///-----
void CPipe::CFlower::actOnMe(KindAction how_action)
{
    if(!m_enabled)
        return;

    if(how_action==KindAction::HIT)
    {
        createPoints();
        removeObject(this);
    }
}

///--------
void CPipe::CFlower::reset()
{
    m_current_position.y=m_pos_gnawing+getBounds().height;
    m_animations->play(MyKindsAnimations::MOVE_UP, m_current_position);
    m_state=State::HIDING;
    m_enabled=false;

    setWhenMoveUpper();
}

///---------
inline void CPipe::CFlower::setWhenMoveUpper()
{
    m_when_move_upper=CScen::getDurationScen()+m_how_long_hiding;
}

///-----------
void CPipe::CFlower::draw(const unique_ptr<sf::RenderWindow>&window)
{
    m_animator->draw(window);
    m_my_pipe->draw(window);
}

