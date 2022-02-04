#include "LavaBullet.h"
#include "../PhysicalObject.h"

const float CLavaBullet::s_my_gravitation=1900;

CLavaBullet::CLavaBullet(sf::Vector2f pos)
:CGameObject(new CAnimations,Parentage::ITEM,{pos.x,pos.y+CScen::s_tile_size+10})
,m_basic_position(m_current_position)
{
    vector<sf::IntRect> m_frame_animation;

    m_frame_animation.push_back({194,80,28,32});
    m_frame_animation.push_back({226,80,28,32});
    m_frame_animation.push_back({258,80,28,32});

    m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.5f,m_scale_to_tile);
    m_animations->create(MyAnimations::GO_DOWN,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.5f,m_scale_to_tile);

    m_animations->getSprite().setRotation(180);
    m_animations->setIndexFrame(1,m_current_position);

    m_animations->getSprite().setRotation(180);
    m_animations->setIndexFrame(2,m_current_position);

    m_animations->getSprite().setRotation(180);

    setWhenJump();
}

///------
void CLavaBullet::update()
{
    if(m_basic_position.y>m_current_position.y||m_when_jump<CScen::getDurationScen())
        move();

    m_animator->update(m_current_position);
}

///------
void CLavaBullet::move()
{
    m_previous_position=m_current_position;

    const float vertical_velocity = m_force * CScen::getFrameTime();

    m_force+=s_my_gravitation*CScen::getFrameTime();
    m_current_position.y+=vertical_velocity;

    if(m_go_up&&vertical_velocity>0)
    {
        m_go_up=false;
        m_current_position.y-=getBounds().height;
        m_current_position.x-=3;

        m_animations->play(MyAnimations::GO_DOWN,m_current_position);
    }

    if(!m_go_up&&m_current_position.y>m_basic_position.y)
        setWhenJump();
}

///-----
void CLavaBullet::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        obj->actOnMe(KindAction::HIT);
}

///------
void CLavaBullet::setWhenJump()
{
    m_when_jump=CScen::getDurationScen()+rand()%3+1;
    m_force=m_jump_force;
    m_go_up=true;

    m_current_position=m_basic_position;

    m_animations->play(CAnimations::STANDARD,m_current_position);
}
