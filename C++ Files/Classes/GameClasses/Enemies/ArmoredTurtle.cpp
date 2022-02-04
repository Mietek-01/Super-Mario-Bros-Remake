#include "Enemies.h"

CArmoredTurtle::CArmoredTurtle(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,pos)
{
    m_value_acceleration=0.3f;

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({352,0,30,32});
    m_frame_animation.push_back({384,2,32,30});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,2.5f,m_scale_to_tile);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({96,2,32,30});
    m_frame_animation.push_back({130,0,30,32});

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.5f,m_scale_to_tile);

    m_animations->play(CAnimations::L_MOVE,m_current_position);
}

///-----
void CArmoredTurtle::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        if(how_collision==KindCollision::BOTTOM)
        {
            this->corectObjectPositionOnMe(*obj,how_collision);
            obj->actOnMe(KindAction::HOP);

        }else
            obj->actOnMe(KindAction::HIT);
}

///-----
void CArmoredTurtle::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    if(which_action==KindAction::HIT)
        createBeatsObjSpecialEfect();
}

