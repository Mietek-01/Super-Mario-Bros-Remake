#include "Enemies.h"
#include "../../Scens/GameScen.h"
#include "../Items/Shell.h"

CTurtle::CTurtle(sf::Vector2f pos,bool create_by_flying_turtle,KindMovement direction)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,pos,direction,200)
,m_created_by_flying_turtle(create_by_flying_turtle)
{
    m_value_acceleration=0.5f;

    vector<sf::IntRect> m_frame_animation;

    m_frame_animation.push_back({480,35,32,45});
    m_frame_animation.push_back({450,33,30,47});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,2.0f,m_scale_to_tile);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back(sf::IntRect(0,35,32,45));
    m_frame_animation.push_back(sf::IntRect(32,33,30,47));

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.0f,m_scale_to_tile);

    if(m_right_dir_reversal)
        m_animations->play(CAnimations::R_MOVE,m_current_position);
    else
        m_animations->play(CAnimations::L_MOVE,m_current_position);
}

///----
void CTurtle::update()
{
    if(!m_created_by_flying_turtle)
        dontFall();

    CPhysicaltObject::update();
}

///----
inline void CTurtle::dontFall()
{
    m_future_position=getBounds();

    /// OKRESLAM PRZYSZLA POZYCJE
    if(m_kind_movement==KindMovement::LEFT_RUN)
       m_future_position.left-=getBounds().width;
    else
       m_future_position.left+=getBounds().width;

    /// SPRAWDZAM CZY DLA PRZYSZLEJ POZYCJI NIE BEDZIE RZADNEJ KOLIZJI

    const auto game_scen_blocks=CMarioGame::instance().getScen<CGameScen>().getBlocks();

    for(const auto &block:*game_scen_blocks)
    {
        if(!block->isVisible())continue;

        if(m_future_position.intersects(block->getBounds()))
        {
            m_close_to_fall=false;
            break;
        }
    }

    /// ZMIENIAM KIERUNEK BO WIEM ZE ZARAZ SPADNE
    if(m_close_to_fall&&!m_jump)
    {
        if(m_right_dir_reversal)
        {
            m_right_dir_reversal=false;
            m_kind_movement=KindMovement::LEFT_RUN;
            m_animations->play(CAnimations::L_MOVE,m_current_position);
        }else
        {
            m_right_dir_reversal=true;
            m_kind_movement=KindMovement::RIGHT_RUN;
            m_animations->play(CAnimations::R_MOVE,m_current_position);
        }

    }else m_close_to_fall=true;

}

///-----
void CTurtle::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        if(how_collision==KindCollision::BOTTOM)
        {
            this->actOnMe(KindAction::CRUMPLED);
            this->corectObjectPositionOnMe(*obj,how_collision);

            obj->actOnMe(KindAction::HOP);
        }
        else
            obj->actOnMe(KindAction::HIT);
}

///-----
void CTurtle::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    switch(which_action)
    {
    case KindAction::CRUMPLED:
        {
            addNewObject(new CShell(m_current_position));

            createPoints();

            CMarioGame::s_sound_manager.play("stomp");

            removeObject(this);

            break;
        }
    case KindAction::HIT:
        {
            createBeatsObjSpecialEfect();
            break;
        }
    }
}
