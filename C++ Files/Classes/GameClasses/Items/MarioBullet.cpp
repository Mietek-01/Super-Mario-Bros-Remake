#include "../Mario.h"
#include "../Shield.h"
#include "../Enemies/Enemies.h"

CMario::CBullet::CBullet(sf::Vector2f pos,bool right_dir,bool doble_speed)
:CPhysicaltObject(new CAnimations,Parentage::ITEM,pos)
{
    m_jump_force=-400.0f;
    m_range=pos.x;

    if(right_dir)
    {
        m_range+=m_lenght_range;
        m_kind_movement=KindMovement::RIGHT_RUN;
        m_right_dir_reversal=true;

    }else
    {
        m_range-=m_lenght_range;
        m_kind_movement=KindMovement::LEFT_RUN;
        m_right_dir_reversal=false;
    }

    if(doble_speed)
    {
        m_value_acceleration=5.0f;
        m_range*=1.2f;
    }
    else
        m_value_acceleration=3.5f;

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({0,1,16,14});
    m_frame_animation.push_back({17,0,14,16});

    m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.3f,m_scale_to_tile);

    m_animations->setPosition(m_current_position);
}

///----------
void CMario::CBullet::update()
{
    if((m_right_dir_reversal&&m_current_position.x>m_range)||
       (!m_right_dir_reversal&&m_current_position.x<m_range))
        removeObject(this);
    else
        CPhysicaltObject::update();
}

///--------
void CMario::CBullet::actOnMe(KindAction how_action)
{
    if(m_dead)return;

    if(how_action==KindAction::HIT)
    {
        CAnimations *hit_animation=new CAnimations;

        vector<sf::IntRect> m_frame_animation;
        m_frame_animation.push_back({35,4,8,8});
        m_frame_animation.push_back({49,1,13,14});
        m_frame_animation.push_back({62,0,17,16});

        hit_animation->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.5f,m_scale_to_tile);
        hit_animation->setPosition(m_current_position);

        CGuiObject *obj=new CSpecialEffects(hit_animation,CSpecialEffects::ONE_LOOP_ANIMATION,0.0f);
        CGUI::addGuiObject(obj);

        removeObject(this);
    }
}

///------------------
void CMario::CBullet::updateForCollisionWithBlock(KindCollision how_collision,CBlock* block)
{
    switch(how_collision)
    {
        case KindCollision::TOP:
        {
            break;
        }

        case KindCollision::BOTTOM:
        {
            jump();
            break;
        }

        case KindCollision::RIGHT_SIDE:
        {
            this->actOnMe(KindAction::HIT);
            return;
        }

        case KindCollision::LEFT_SIDE:
        {
            this->actOnMe(KindAction::HIT);
            return;
        }

        case KindCollision::NONE:
        {
            return;
        }

    }

    block->actOnObject(this,how_collision);
}

///----------
void CMario::CBullet::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::ENEMY)
    {
        CShield *obj_with_shield=dynamic_cast<CShield* >(obj);

        if(obj_with_shield)
            obj_with_shield->setDamage(m_damage_value);

        if(!dynamic_cast<CArmoredTurtle*>(obj))
            obj->actOnMe(KindAction::HIT);

        this->actOnMe(KindAction::HIT);
    }
}

