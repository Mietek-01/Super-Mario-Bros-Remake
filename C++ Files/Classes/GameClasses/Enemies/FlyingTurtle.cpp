#include "Enemies.h"

const float CFlyingTurtle::s_my_gravitation = 1000.0f;

CFlyingTurtle::CFlyingTurtle(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,pos,CPhysicaltObject::KindMovement::LEFT_RUN,300)
{
    m_value_acceleration=0.8f;
    m_jump_force=-550;

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({224,34,32,46});
    m_frame_animation.push_back({257,32,31,48});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,1.5f,m_scale_to_tile);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({224,32,31,48});
    m_frame_animation.push_back({256,34,32,46});

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);

    m_animations->play(CAnimations::L_MOVE,m_current_position);
}

///------
void CFlyingTurtle::makeJump()
{
    m_force+= s_my_gravitation *CScen::getFrameTime();
    m_current_position.y+=m_force*CScen::getFrameTime();

    isUnderMap();
}

///----------
void CFlyingTurtle::updateForCollisionWithBlock(KindCollision how_collision ,CBlock* block)
{
    switch(how_collision)
    {
        case KindCollision::TOP:
        {
            m_force=0.0f;
            break;
        }

        case KindCollision::BOTTOM:
        {
            if(block->isHit()||block->iamDead())
                actOnMe(KindAction::HIT);
            else
                jump();

            break;
        }

        case KindCollision::RIGHT_SIDE:
        {
            m_kind_movement=KindMovement::LEFT_RUN;
            m_right_dir_reversal=false;
            m_animations->play(CAnimations::L_MOVE,m_current_position);
            break;
        }

        case KindCollision::LEFT_SIDE:
        {
            m_kind_movement=KindMovement::RIGHT_RUN;
            m_right_dir_reversal=true;
            m_animations->play(CAnimations::R_MOVE,m_current_position);
            break;
        }

        case KindCollision::NONE:
            {
                return;
            }
    }

    block->actOnObject(this,how_collision);
}

///-----
void CFlyingTurtle::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        if(how_collision==KindCollision::BOTTOM)
        {
            removeObject(this);

            CTurtle *new_turtle=new CTurtle(m_current_position,true,m_kind_movement);
            addNewObject(new_turtle);

            this->corectObjectPositionOnMe(*obj,how_collision);
            obj->actOnMe(KindAction::HOP);
        }
        else
            obj->actOnMe(KindAction::HIT);
}

///-----
void CFlyingTurtle::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    if(which_action==KindAction::HIT)
        createBeatsObjSpecialEfect();
}
