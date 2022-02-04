#include "Shell.h"

CShell::CShell(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::ITEM,{pos.x,pos.y-CPhysicaltObject::s_correction_to_bottom_collision},KindMovement::RIGHT_RUN)
{
    m_value_acceleration=2.0f;

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back(sf::IntRect(65,48,30,26));
    m_frame_animation.push_back(sf::IntRect(97,48,32,26));
    m_frame_animation.push_back(sf::IntRect(128,48,30,26));
    m_frame_animation.push_back(sf::IntRect(159,48,32,26));

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);
    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);

    m_animations->setPosition(m_current_position);
}

///------
void CShell::update()
{
    if(m_active)
        CPhysicaltObject::update();
    else
    {
        if(m_jump)
        {
            makeJump();
            m_animator->setPosition(m_current_position);
        }
    }
}


///----------
void CShell::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    switch(obj->getParentage())
    {
        case Parentage::MARIO:
            {
                if(how_collision==KindCollision::BOTTOM)
                {
                    m_active=!m_active;
                    this->corectObjectPositionOnMe(*obj,how_collision);
                    obj->actOnMe(KindAction::HOP);
                }else
                    if(m_active)
                        obj->actOnMe(KindAction::HIT);

                break;
            }
        case Parentage::ITEM:
            {
                if(m_active)
                {
                    CShell * shell=dynamic_cast<CShell*>(obj);

                    if(shell)
                        shell->actOnMe(KindAction::HIT);
                }

                break;
            }
        case Parentage::ENEMY:
            {
                if(m_active)
                    obj->actOnMe(KindAction::HIT);
                break;
            }
    }
}

///---------
void CShell::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    if(which_action==KindAction::HIT)
        this->createBeatsObjSpecialEfect();
}
