#include "Gun.h"
#include "../Mario.h"
#include"../../MarioGame.h"

CGun::CBullet::CBullet(sf::Vector2f pos,bool right_dir)
:CGameObject(new CAnimations,Parentage::ITEM,pos,100)
,m_right_dir(right_dir)
{
    vector<sf::IntRect> m_frame_animation;
    const int additional_length=100*rand()%5+2;

    if(right_dir)
    {
        m_frame_animation.push_back({416,114,32,29});
        m_frame_animation.push_back({416,146,32,29});
        m_frame_animation.push_back({416,178,32,29});

        m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,1.5f,m_scale_to_tile);

        m_range+=m_current_position.x+additional_length;

    }else
    {
        m_frame_animation.push_back({64,114,32,29});
        m_frame_animation.push_back({64,146,32,29});
        m_frame_animation.push_back({64,178,32,29});

        m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);

        m_range=m_current_position.x-m_range-additional_length;
    }

    m_current_position.y+=getBounds().height+2;

    m_animations->setPosition(m_current_position);
}

///----------
void CGun::CBullet::update()
{
    m_previous_position=m_current_position;

    if((m_right_dir&&m_current_position.x>m_range)||
       (!m_right_dir&&m_current_position.x<m_range))
    {
        m_the_end_range=true;
        actOnMe(KindAction::HIT);
    }
    else
    {
        if(m_right_dir)
            m_current_position.x+=m_speed;
        else
            m_current_position.x-=m_speed;
    }

    m_animations->update(m_current_position);
}

///--------
void CGun::CBullet::actOnMe(KindAction how_action)
{
    if(how_action==KindAction::HIT)
    {
        sf::Sprite *sprite=new sf::Sprite(m_animations->getSprite());

        CGuiObject *obj=new CSpecialEffects(new CSprite(sprite),CSpecialEffects::JUMP,CMarioGame::s_size_window.y+CScen::s_tile_size,{0,10});
        CGUI::addGuiObject(obj);

        if(!m_the_end_range)
            createPoints();

        removeObject(this);
    }
}

///----------
void CGun::CBullet::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        if(how_collision==KindCollision::BOTTOM)
        {
            corectObjectPositionOnMe(*obj,how_collision);
            obj->actOnMe(KindAction::HOP);

            this->actOnMe(KindAction::HIT);

        }
        else
            obj->actOnMe(KindAction::HIT);
}

