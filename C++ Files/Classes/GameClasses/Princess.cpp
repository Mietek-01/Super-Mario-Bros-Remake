#include "Enemies/Bowser.h"
#include "../Scens/GameScen.h"

///---------------- CPrincess------------------///
CBowser::CPrincess::CPrincess(sf::Vector2f pos)
:CGameObject(new CSprite(CGUI::createSprite("Items",{222,106,32,54},pos,m_scale_to_tile,true)),Parentage::ITEM,pos)
,m_bot_mario(new CBotMario(CMarioGame::instance().getScen<CGameScen>().getMarioPosition()))
{
    addNewObject(m_bot_mario);
}

///------
void CBowser::CPrincess::update()
{
    if(!m_bot_mario_stoped)
    {
        if(m_bot_mario->getCurrentPosition().x>=m_current_position.x-getSize().x/2.0f-m_stop_mario_position)
        {
            m_bot_mario->stop();
            m_bot_mario_stoped=true;
            m_timer=CScen::getDurationScen()+m_when_show_message;
        }
    }else
    {
        if(!m_text)
        {
            if(m_timer<CScen::getDurationScen())
            {
                m_timer=CScen::getDurationScen()+m_when_state_win_game;
                m_text.reset(CGUI::createText(m_message_to_mario,CMarioGame::instance().getViewPosition(),sf::Color::White
                    ,"arial",true,70));
            }
        }else
        {
            /// WYKONA SIE RAZ BO W STANIE WIN_GAME NIE WYKONUJE JUZ UPDATOW
            if(m_timer<CScen::getDurationScen())
                CMarioGame::instance().getScen<CGameScen>().setGameState(CGameScen::GameStates::WIN_GAME);
        }
    }
}

///--------
void CBowser::CPrincess::draw(const unique_ptr<sf::RenderWindow>&window)
{
   m_animator->draw(window);

   if(m_text)
    window->draw(*m_text);
}

///-----------------------------CBotMario---------------------///
CBowser::CPrincess::CBotMario::CBotMario(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::ITEM,pos,KindMovement::RIGHT_RUN)
{
    m_value_acceleration=3.0f;
    /// TWORZE ANIMACJE
    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({32,36,32,60});
    m_frame_animation.push_back({65,36,30,60});
    m_frame_animation.push_back({96,38,32,58});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.0f,m_scale_to_tile);

    ///------
    m_animations->create(CAnimations::R_STANDING,CMarioGame::s_texture_manager["Mario_right"],{0,38,32,58},m_scale_to_tile);
    m_animations->create(CAnimations::R_JUMP,CMarioGame::s_texture_manager["Mario_right"],{160,35,32,60},m_scale_to_tile);

    m_animations->play(CAnimations::R_MOVE,m_current_position);
}

///-----
void CBowser::CPrincess::CBotMario::update()
{
    if(!m_iam_blocking&&m_change_animation)
    {
        if(m_jump)
            m_animations->play(CAnimations::R_JUMP,m_current_position);
        else
            m_animations->play(CAnimations::R_MOVE,m_current_position);

        m_change_animation=false;
    }

    if(!m_iam_blocking&&m_kind_movement!=KindMovement::STANDING)
        CMarioGame::instance().getScen<CGameScen>().changePositionView(m_value_acceleration);

    CPhysicaltObject::update();
    
    m_iam_blocking=false;
}

///----
void CBowser::CPrincess::CBotMario::updateForCollisionWithBlock(KindCollision how_collision,CBlock* block)
{
    switch(how_collision)
    {
        case KindCollision::TOP:
        {
            return;
        }

        case KindCollision::BOTTOM:
        {
            m_jump=false;
            m_in_bottom_collision=true;

            m_animations->play(CAnimations::R_MOVE,m_current_position);
            break;
        }

        case KindCollision::RIGHT_SIDE:
        {
            if(!m_iam_blocking)
            {
                m_change_animation=true;
                if(m_jump)
                    m_animations->play(CAnimations::R_JUMP,m_current_position);
                else
                    m_animations->play(CAnimations::R_STANDING,m_current_position);
            }

            m_iam_blocking=true;
            break;
        }

        case KindCollision::LEFT_SIDE:
        {
            return;
        }

        case KindCollision::NONE:
        {
            return;
        }

    }

    block->actOnObject(this,how_collision);
}

///---
void CBowser::CPrincess::CBotMario::stop()
{
    m_kind_movement=KindMovement::STANDING;
    m_animations->play(CAnimations::R_STANDING,m_current_position);
}

///------
void CBowser::CPrincess::CBotMario::falling()
{
    CPhysicaltObject::falling();
    m_animations->play(CAnimations::R_JUMP,m_current_position);
}

