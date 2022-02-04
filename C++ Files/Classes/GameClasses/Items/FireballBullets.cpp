#include "FireballBullets.h"
#include "../Shield.h"
#include "../../MarioGame.h"

#include <math.h>

///-----------------CBullet------------///

CBullet::CBullet(sf::Vector2f pos,bool right_dir,float speed,float scale)
:CGameObject(new CAnimations,Parentage::ITEM,pos)
,m_right_dir(right_dir)
,m_speed(speed)
,m_my_scal(scale)
,m_basic_position(pos)
{
    CMarioGame::s_sound_manager.play("bowser_fire");
}

///------
void CBullet::update()
{
    m_previous_position=m_current_position;

    if(m_right_dir)
    {
        if(m_current_position.x>m_basic_position.x+m_range&&m_animations->islastFrame())
            actOnMe(KindAction::HIT);

        m_current_position.x+=m_speed;

    }else
    {
        if(m_current_position.x<m_basic_position.x-m_range&&m_animations->islastFrame())
            actOnMe(KindAction::HIT);

        m_current_position.x-=m_speed;
    }

    this->corectPosition();

    m_animations->update(m_current_position);
}

///--------
void CBullet::actOnMe(KindAction how_action)
{
    if(how_action==KindAction::HIT)
        removeObject(this);
}

///----------
void CBullet::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
    {
        CShield *obj_with_shield=dynamic_cast<CShield* >(obj);

        if(obj_with_shield)
            obj_with_shield->setDamage(m_damage_value);

        obj->actOnMe(KindAction::HIT);
        actOnMe(KindAction::HIT);
    }
}

///---------------------CBasicBullet---------------///
CBasicBullet::CBasicBullet(sf::Vector2f pos,bool right_dir)
:CBullet(pos,right_dir,rand()%3+2,CGameObject::m_scale_to_tile*1.4f)
{
    vector<sf::IntRect> m_frame_animation;

    if(right_dir)
    {
        m_frame_animation.push_back({7,370,20,12});
        m_frame_animation.push_back({35,368,25,16});
        m_frame_animation.push_back({66,365,30,22});
        m_frame_animation.push_back({97,364,32,26});

        m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,2.5f,m_my_scal);
    }else
    {
        m_frame_animation.push_back({526,370,20,12});
        m_frame_animation.push_back({493,368,25,16});
        m_frame_animation.push_back({457,365,30,22});
        m_frame_animation.push_back({425,364,32,26});

        m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,2.5f,m_my_scal);
    }

    m_animations->play(CAnimations::STANDARD,m_current_position);
}

///---------------------CCircleBullet---------------///
CCircleBullet::CCircleBullet(sf::Vector2f pos,bool right_dir)
:CBullet(pos,right_dir,3,CGameObject::m_scale_to_tile)
,m_center_circle_x_pos(pos.x)
{
    vector<sf::IntRect> m_frame_animation;

    m_frame_animation.push_back({194,80,28,32});
    m_frame_animation.push_back({226,80,28,32});
    m_frame_animation.push_back({258,80,28,32});

    m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.5f,m_my_scal);

    corectPosition();

    m_animations->play(CAnimations::STANDARD,m_current_position);
}

///-----
void CCircleBullet::update()
{
    m_previous_position=m_current_position;

    if(m_right_dir)
        if(m_current_position.x>m_basic_position.x+m_range&&m_animations->islastFrame())
            actOnMe(KindAction::HIT);
    else
        if(m_current_position.x<m_basic_position.x-m_range&&m_animations->islastFrame())
            actOnMe(KindAction::HIT);

    corectPosition();
}

///-------
void CCircleBullet::corectPosition()
{
    m_angle+=m_spin_speed;
    m_rotate+=m_rotate_speed;

    if(m_right_dir)
        m_center_circle_x_pos+=m_speed;
    else
        m_center_circle_x_pos-=m_speed;

    m_current_position.y=m_basic_position.y-5.0f*m_radius*sin(m_angle);
    m_current_position.x=m_center_circle_x_pos-5.0f*m_radius*cos(m_angle);

    m_animations->update(m_current_position);
    m_animations->getSprite().setRotation(m_rotate);
}

///---------------------CSinBullet---------------///

CSinBullet::CSinBullet(sf::Vector2f pos,bool right_dir)
:CBullet(pos,right_dir,2.5,CGameObject::m_scale_to_tile*1.4f)
{
    vector<sf::IntRect> m_frame_animation;

    if(right_dir)
    {
        m_frame_animation.push_back({7,370,20,12});
        m_frame_animation.push_back({35,368,25,16});
        m_frame_animation.push_back({66,365,30,22});
        m_frame_animation.push_back({97,364,32,26});

        m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,2.5f,m_my_scal);
    }else
    {
        m_frame_animation.push_back({526,370,20,12});
        m_frame_animation.push_back({493,368,25,16});
        m_frame_animation.push_back({457,365,30,22});
        m_frame_animation.push_back({425,364,32,26});

        m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,2.5f,m_my_scal);
    }

    m_animations->play(CAnimations::STANDARD,m_current_position);
}

///------
void CSinBullet::corectPosition()
{
    if(m_right_dir)
        m_my_speed+=m_sin_function_speed;
    else
        m_my_speed-=m_sin_function_speed;

    m_current_position.y=m_basic_position.y+(45.0f*sin(m_my_speed));
}



