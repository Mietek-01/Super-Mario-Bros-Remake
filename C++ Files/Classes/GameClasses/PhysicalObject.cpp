#include "PhysicalObject.h"
#include <math.h>

const float CPhysicaltObject::s_gravitation=2600.0f;
const float CPhysicaltObject::s_correction_to_bottom_collision=0.2f;

CPhysicaltObject::CPhysicaltObject(CAnimator * animator,Parentage parentage,const sf::Vector2f& pos,KindMovement how_movement,int points)
:CGameObject(animator,parentage,pos,points)
,m_kind_movement(how_movement)
{
    m_current_position.y+=0.1f;

    if(how_movement==KindMovement::LEFT_RUN||how_movement==KindMovement::STANDING)
        m_right_dir_reversal=false;
    else
        m_right_dir_reversal=true;
}

///-----------
void CPhysicaltObject::update()
{
    m_previous_position=m_current_position;

    /// ZAPEWNIA PRAWIDLOWE DZIALANIE SYSTEMU OKRESLANIA KOLIZJI
    if(m_in_bottom_collision)
        m_previous_position.y-=s_correction_to_bottom_collision;

    move();

    if(m_jump)
        makeJump();

    m_animator->update(m_current_position);
}

///-----
void CPhysicaltObject::updateForCollisionWithBlock(KindCollision how_collision ,CBlock* block)
{
    /// TUTAJ OKRESLAM WLASNIE FIZYKE GRY CZYLI REAGOWANIE NA BLOKI
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
            {
                m_jump=false;
                m_in_bottom_collision=true;
            }

            break;
        }

        /// JEST TO OKRESLONE POD WROGOW
        case KindCollision::RIGHT_SIDE:
        {
            m_right_dir_reversal=false;
            m_kind_movement=KindMovement::LEFT_RUN;

            if(m_animations)
                m_animations->play(CAnimations::L_MOVE,m_current_position);

            break;
        }

        case KindCollision::LEFT_SIDE:
        {
            m_right_dir_reversal=true;
            m_kind_movement=KindMovement::RIGHT_RUN;

            if(m_animations)
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

///---------------------------
void CPhysicaltObject::physicGameWithBlock(map<KindCollision,CBlock*>& collisions,bool none_collision)
{
        if(!m_jump&&none_collision)
        {
            falling();
            return;
        }

        ///---------
        if(collisions[KindCollision::RIGHT_SIDE])
            updateForCollisionWithBlock(KindCollision::RIGHT_SIDE,collisions[KindCollision::RIGHT_SIDE]);

        ///--------
        if(collisions[KindCollision::LEFT_SIDE])
            updateForCollisionWithBlock(KindCollision::LEFT_SIDE,collisions[KindCollision::LEFT_SIDE]);

        ///----------
        if(collisions[KindCollision::TOP])
            if(collisions[KindCollision::LEFT_SIDE]||collisions[KindCollision::RIGHT_SIDE])
            {
                /// SPRAWDZAM CZY DALEJ JESTEM W TEJ KOLIZJI PO SKORYGOWANIU JUZ DLA KOL BOCZNYCH
                if(KindCollision::TOP==this->howCollision(*collisions[KindCollision::TOP]))
                    updateForCollisionWithBlock(KindCollision::TOP,collisions[KindCollision::TOP]);
            }else
                updateForCollisionWithBlock(KindCollision::TOP,collisions[KindCollision::TOP]);

        ///-----------------
        if(collisions[KindCollision::BOTTOM])
            if(collisions[KindCollision::LEFT_SIDE]||collisions[KindCollision::RIGHT_SIDE])
            {
                /// SPRAWDZAM CZY DALEJ JESTEM W TEJ KOLIZJI PO SKORYGOWANIU JUZ DLA KOL BOCZNYCH
                if(KindCollision::BOTTOM==this->howCollision(*collisions[KindCollision::BOTTOM]))
                    updateForCollisionWithBlock(KindCollision::BOTTOM,collisions[KindCollision::BOTTOM]);
            }else
                updateForCollisionWithBlock(KindCollision::BOTTOM,collisions[KindCollision::BOTTOM]);
}

///----------------------
void CPhysicaltObject::move()
{
    switch(m_kind_movement)
    {
        case KindMovement::RIGHT_RUN: m_current_position.x+=m_value_acceleration;break;

        case KindMovement::LEFT_RUN: m_current_position.x-=m_value_acceleration;break;
    }
}

///--------------------------
void CPhysicaltObject::makeJump()
{
    m_force+=s_gravitation*CScen::getFrameTime();
    m_current_position.y+=m_force*CScen::getFrameTime();

    isUnderMap();
}

///-------------------------------
void CPhysicaltObject::falling()
{
    m_in_bottom_collision=false;
    m_jump=true;
    m_force=m_falling_force;
}

///-----------------------------
void CPhysicaltObject::jump()
{
    m_force=m_jump_force;
    m_jump=true;
    m_in_bottom_collision=false;
}

///--------------------
void CPhysicaltObject::hop(float force)
{
    m_force=force;
    m_jump=true;
    m_in_bottom_collision=false;
}
