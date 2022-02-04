#include "Enemies.h"
#include "../Mario.h"

CGoombas::CGoombas(sf::Vector2f pos,KindMovement kind_movement,bool created_by_bowser)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,pos,kind_movement)
,m_created_by_bowser(created_by_bowser)
{
    m_value_acceleration=1.0f;

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({0,0,32,32});
    m_frame_animation.push_back({32,0,32,32});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);
    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);

    m_animations->play(CAnimations::L_MOVE,m_current_position);
}

///-----
void CGoombas::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        if(how_collision==KindCollision::BOTTOM)
        {
            this->actOnMe(KindAction::CRUMPLED);
            this->corectObjectPositionOnMe(*obj,how_collision);

            obj->actOnMe(KindAction::HOP);
        }
        else
        {
            CMario *mario=dynamic_cast<CMario* >(obj);
            mario->setDamage(m_damage_value);

            obj->actOnMe(KindAction::HIT);
        }
}

///-----
void CGoombas::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    switch(which_action)
    {
    case KindAction::CRUMPLED:
        {
            if(m_jump)
                createBeatsObjSpecialEfect(-100.0f);
            else
            {
                sf::Sprite * crumpled_goombas=CGUI::createSprite("Enemies_left",{64,16,32,16},m_current_position,m_scale_to_tile,true);

                CGuiObject* obj=new CSpecialEffects(new CSprite(crumpled_goombas),CSpecialEffects::KindUpdate::STANDIG,CScen::getDurationScen()+3.0f);
                CGUI::addGuiObject(obj);

                createPoints();

                CMarioGame::s_sound_manager.play("stomp");

                removeObject(this);
            }

            break;
        }

    case KindAction::HIT:
        {
            createBeatsObjSpecialEfect();
            break;
        }
    }
}

///---------
void CGoombas::updateForCollisionWithBlock(KindCollision how_collision ,CBlock* block)
{
    if(m_created_by_bowser&&block->getCurrentPosition().y<CScen::s_tile_size*3)
        return;

    CPhysicaltObject::updateForCollisionWithBlock(how_collision,block);
}
