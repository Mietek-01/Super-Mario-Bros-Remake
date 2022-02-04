#include "Enemies.h"
#include "../Mario.h"


CRedTurtle::CRedTurtle(sf::Vector2f pos,KindMovement kind_movement,bool created_by_bowser)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,pos,kind_movement)
,m_created_by_bowser(created_by_bowser)
{
    m_value_acceleration=0.6f;

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({385,82,30,30});
    m_frame_animation.push_back({416,84,32,28});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,2.5f,m_scale_to_tile);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({64,84,32,28});
    m_frame_animation.push_back({96,82,30,30});

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.5f,m_scale_to_tile);

    if(m_right_dir_reversal)
        m_animations->play(CAnimations::R_MOVE,m_current_position);
    else
        m_animations->play(CAnimations::L_MOVE,m_current_position);
}

///-----
void CRedTurtle::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
    {
        CMario *mario=dynamic_cast<CMario* >(obj);
        mario->setDamage(m_damage_value);

        obj->actOnMe(KindAction::HIT);
    }

}

///-----
void CRedTurtle::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    if(which_action==KindAction::HIT)
        createBeatsObjSpecialEfect();
}

///---------
void CRedTurtle::updateForCollisionWithBlock(KindCollision how_collision ,CBlock* block)
{
    if(m_created_by_bowser&&block->getCurrentPosition().y<CScen::s_tile_size*3)
        return;

    CPhysicaltObject::updateForCollisionWithBlock(how_collision,block);
}
