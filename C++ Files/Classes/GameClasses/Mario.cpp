#include "Mario.h"
#include "../Scens/GameScen.h"
#include "Blocks/MoveableBlock.h"

#include <math.h>

const short CMario::s_basic_number_lives_mario=5;
short CMario::s_lives_mario=CMario::s_basic_number_lives_mario;

CMario::LevelMario CMario::s_level_mario=CMario::LevelMario::SMALL_MARIO;

CMario::~CMario()
{
    CMoveableBlock::removeDeadObjOnMoveableBlock(this);
}

///-----
CMario::CMario(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::MARIO,pos,KindMovement::STANDING)
,CShield(1500)
,m_deactivate_decisions(CEventHandler::getDeactivateDecisions())
,m_enabled_decisions(CEventHandler::getEnabledDecisions())
{
    CEventHandler::resetDecisions();

    m_jump_force=-1100.0f;
    m_right_dir_reversal=true;

    if(s_level_mario==LevelMario::SMALL_MARIO)
        createSmalMarioAnimations();
    else
        createBigMarioAnimations();

    m_animations->play(CAnimations::R_STANDING,m_current_position);
}

///--------------------
void CMario::update()
{
    handleMarioControll();

    m_previous_position=m_current_position;
    
    if(m_in_bottom_collision)
        m_previous_position.y-=s_correction_to_bottom_collision;

    this->run();

    if(m_jump)
        this->makeJump();

    if(m_iam_shooting&&m_shooting_iterativity_timer<CScen::getDurationScen())
    {
        m_iam_shooting=false;
        animationUpdtate();
    }

    collisionWithBoundsMap();

    m_animations->update(m_current_position);

    if(m_shield_pointer)
    {
        m_shield_pointer->update();

        if(m_shield_pointer->isTheEndShield())
            this->remove();
    }
}

///---------------------
void CMario::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    switch(which_action)
    {
    case KindAction::LVL_UP:
    {
        if(m_flashing_mario||s_level_mario!=LevelMario::SMALL_MARIO)
            return;

        if(CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::CHANGING_LVL_MARIO))
        {
            const sf::FloatRect prev_small_mario_bounds(getPreviousBounds());

            if(!m_right_dir_reversal)
                m_animations->play(MyKindsAnimations::L_LVL_UP,m_current_position);
            else
                m_animations->play(MyKindsAnimations::R_LVL_UP,m_current_position);

            this->corectPositionWhenChangingOnBigMario(prev_small_mario_bounds);

            m_kind_movement=KindMovement::STANDING;
            m_curbing=m_stop=false;
            s_level_mario=LevelMario::BIG_MARIO;

            m_horizontal_velocity=0.0f;
            if(m_jump)m_force=0.0f;

            m_changeing_lvl_timer=CScen::getDurationScen()+m_changing_lvl_time;

            CMarioGame::s_sound_manager.play("powerup");
        }

        break;
    }

    case KindAction::HOP:
    {
        hop(-600.0f);
        break;
    }

    case KindAction::UNDER_MAP:
    {
        m_current_position.y=CMarioGame::s_size_window.y+CScen::s_tile_size;
        remove();
        break;
    }

    case KindAction::HIT:
    {
        //return;

        if(m_flashing_mario)
            return;

        if(m_shield_pointer)
        {
            reduceShield();

            if(getShieldValue()>0)
                setFlashing(m_flashing_time*0.5f);

        }
        else if(s_level_mario!=LevelMario::SMALL_MARIO)
        {
            if(CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::CHANGING_LVL_MARIO))
            {
                if(!m_right_dir_reversal)
                    m_animations->play(MyKindsAnimations::L_LVL_DOWN,m_current_position);
                else
                    m_animations->play(MyKindsAnimations::R_LVL_DOWN,m_current_position);

                m_changeing_lvl_timer=CScen::getDurationScen()+m_changing_lvl_time;
                s_level_mario=LevelMario::SMALL_MARIO;

                m_kind_movement=KindMovement::STANDING;
                m_curbing=m_stop=false;

                m_horizontal_velocity=0.0f;
                if(m_jump)m_force=0.0f;

                CMarioGame::s_sound_manager.play("pipe");
            }
        }
        else
            this->remove();

        break;
    }
    }
}

///------------------
void CMario::updateForCollisionWithBlock(KindCollision how_collision,CBlock* block)
{
    switch(how_collision)
    {
        case KindCollision::TOP:
        {
            if(s_level_mario==LevelMario::SMALL_MARIO)
                block->actOnMe(KindAction::HIT);
            else
                block->actOnMe(KindAction::DESTROYED);

            m_force=0.0;
            break;
        }

        case KindCollision::BOTTOM:
        {
            m_jump=false;
            m_in_bottom_collision=true;

            animationUpdtate();

            break;
        }

        case KindCollision::RIGHT_SIDE:
        {
            if(m_right_dir_reversal)
            {
                /// NIE BLOKUJE RUCHU GDY PODCZAS SKOKU CHCE ISC DALEJ
                if(m_curbing)
                {
                    m_kind_movement=KindMovement::STANDING;
                    m_curbing=m_stop=false;
                }

                if(!m_jump)
                    if(m_right_dir_reversal)
                        m_animations->play(CAnimations::R_STANDING,m_current_position);
                    else
                        m_animations->play(CAnimations::L_STANDING,m_current_position);

                /// BY SZYBCIEJ WYSTARTOWAL
                m_horizontal_velocity=1.0f;

            }else
            {
                if(m_curbing)
                {
                    m_curbing=m_stop=false;
                    m_horizontal_velocity=0.0f;
                }
            }

            break;
        }

        case KindCollision::LEFT_SIDE:
        {
            if(!m_right_dir_reversal)
            {
                if(m_curbing)
                {
                    m_kind_movement=KindMovement::STANDING;
                    m_curbing=m_stop=false;
                }

                if(!m_jump)
                    if(m_right_dir_reversal)
                        m_animations->play(CAnimations::R_STANDING,m_current_position);
                    else
                        m_animations->play(CAnimations::L_STANDING,m_current_position);

                m_horizontal_velocity=-1.0f;

            }else
            {
                if(m_curbing)
                {
                    m_curbing=m_stop=false;
                    m_horizontal_velocity=0.0f;
                }
            }

            break;
        }

        case KindCollision::NONE:
        {
            return;
        }

    }

    block->actOnObject(this,how_collision);
}

///------------------------
void CMario::run()
{
    switch(m_kind_movement)
    {
    case KindMovement::RIGHT_RUN:
        {
            if(!m_curbing)
            {
                if(m_horizontal_velocity<m_max_speed)
                    m_horizontal_velocity+=m_value_acceleration;
                else
                    m_horizontal_velocity=m_max_speed;

            }else curbing();

            m_current_position.x+=m_horizontal_velocity;

            break;
        }
    case KindMovement::LEFT_RUN:
        {
            if(!m_curbing)
            {
                if(-m_horizontal_velocity<m_max_speed)
                    m_horizontal_velocity-=m_value_acceleration;
                else
                    m_horizontal_velocity=-m_max_speed;

            }else curbing();

            m_current_position.x+=m_horizontal_velocity;

            break;
        }
    }
}

///-------------------------
void CMario::curbing()
{
    if(fabs(0.3f-fabs(m_horizontal_velocity))<0.3f)
    {
        m_curbing=false;
        m_horizontal_velocity=0.0f;

        if(m_stop)
        {
            m_kind_movement=KindMovement::STANDING;

            if(!m_jump)
            {
                if(m_right_dir_reversal)
                    m_animations->play(CAnimations::R_STANDING,m_current_position);
                else
                    m_animations->play(CAnimations::L_STANDING,m_current_position);
            }

            m_stop=false;
        }
    }
    else
        if(m_horizontal_velocity>0)
            m_horizontal_velocity-=m_value_acceleration*1.8f;
        else
            if(m_horizontal_velocity<0)
                m_horizontal_velocity+=m_value_acceleration*1.8f;
}

///------------------
inline void CMario::handleMarioControll()
{
    if(m_enabled_decisions[CEventHandler::DecisionsPlayer::CLICK_JUMP])
        jump();

    ///----
    if(m_enabled_decisions[CEventHandler::DecisionsPlayer::CLICK_SHOOT])
        shoot();

    ///------
    if(m_enabled_decisions[CEventHandler::DecisionsPlayer::CLICK_CROUCH])
        crouch();

    ///--------
    if(m_enabled_decisions[CEventHandler::DecisionsPlayer::CLICK_LEFT_RUN])
    {
        if(m_kind_movement==KindMovement::LEFT_RUN)m_curbing=false;
        if(m_kind_movement==KindMovement::RIGHT_RUN)m_curbing=true;

        m_kind_movement=KindMovement::LEFT_RUN;
        m_right_dir_reversal=false;
        m_stop=false;

        animationUpdtate();
    }

    ///-------
    if(m_enabled_decisions[CEventHandler::DecisionsPlayer::CLICK_RIGHT_RUN])
    {
        if(m_kind_movement==KindMovement::RIGHT_RUN)m_curbing=false;
        if(m_kind_movement==KindMovement::LEFT_RUN)m_curbing=true;

        m_kind_movement=KindMovement::RIGHT_RUN;
        m_right_dir_reversal=true;
        m_stop=false;

        animationUpdtate();
    }

    ///----------------- DLA PUSZCZONYCH KLAWISZY-----------------///

    if(m_deactivate_decisions[CEventHandler::DecisionsPlayer::CLICK_LEFT_RUN]&&m_kind_movement==KindMovement::LEFT_RUN)
        m_stop=m_curbing=true;

    ///----
    if(m_deactivate_decisions[CEventHandler::DecisionsPlayer::CLICK_RIGHT_RUN]&&m_kind_movement==KindMovement::RIGHT_RUN)
        m_stop=m_curbing=true;

    ///----
    if(m_deactivate_decisions[CEventHandler::DecisionsPlayer::CLICK_CROUCH])
    {
        m_iam_crouching=false;

        if(s_level_mario==LevelMario::BIG_MARIO)
            animationUpdtate();
    }

    ///RESETUJE
    CEventHandler::resetDecisions();
}

///---
void CMario::corectPositionWhenChangingOnBigMario(const sf::FloatRect& prev_small_mario_bounds)
{
    /// ZMIENIAM NA TA KLATKE KTORA BD ODPOWIADAC NOWEMU POZIOMOWI MARIA
    m_animations->setIndexFrame(1,m_current_position);

    const auto blocks=CMarioGame::instance().getScen<CGameScen>().getBlocks();

    for(const auto &block:*blocks)
    {
        if(!block->isVisible())continue;

        if(getBounds().intersects(block->getBounds()))
        {
            KindCollision how_collision;

            if(block->getCurrentPosition().y<prev_small_mario_bounds.top)
            {
                how_collision=KindCollision::TOP;
                block->corectObjectPositionOnMe(*this,how_collision);
                m_current_position.y+=5;
                continue;
            }else
                if(block->getBounds().top>prev_small_mario_bounds.top+prev_small_mario_bounds.height)
                    how_collision=KindCollision::BOTTOM;
            else
                if(block->getCurrentPosition().x<getPreviousBounds().left+getPreviousBounds().width/2.0f)
                    how_collision=KindCollision::LEFT_SIDE;
            else
                    how_collision=KindCollision::RIGHT_SIDE;

            block->corectObjectPositionOnMe(*this,how_collision);
        }
    }

    m_animations->setIndexFrame(0,m_current_position);
}

///---
void CMario::shoot()
{
    if(s_level_mario==LevelMario::BIG_MARIO&&!m_iam_shooting)
    {
        if(m_right_dir_reversal)
            m_animations->play(MyKindsAnimations::R_SHOOT,m_current_position);
        else
            m_animations->play(MyKindsAnimations::L_SHOOT,m_current_position);

        m_shooting_iterativity_timer=CScen::getDurationScen()+m_shooting_iterativity;
        m_iam_shooting=true;
        CMarioGame::s_sound_manager.play("fireball");

        /// TWORZE POCISK
        sf::Vector2f pos_bullet;

        if(m_right_dir_reversal)
            pos_bullet.x=m_current_position.x+getSize().x/2.0f;
        else
            pos_bullet.x=m_current_position.x-getSize().x/2.0f;

        pos_bullet.y=m_current_position.y-getSize().y/2.0f;

        if(fabs(m_horizontal_velocity)>3.5f)
        {
            pos_bullet.x+=3.5f;
            addNewObject(new CBullet(pos_bullet,m_right_dir_reversal,true));
        }
        else
            addNewObject(new CBullet(pos_bullet,m_right_dir_reversal));
    }
}

///-----------------
void CMario::changingLvl()
{
    m_animations->update(m_current_position);

    if(m_changeing_lvl_timer<CScen::getDurationScen()&&!m_animations->islastFrame())
    {
        switch(s_level_mario)
        {
            case LevelMario::BIG_MARIO:
            {
                createBigMarioAnimations();
                break;
            }

            case LevelMario::SMALL_MARIO:
            {
                createSmalMarioAnimations();
                setFlashing(m_flashing_time);
                break;
            }
        }

        animationUpdtate();

        CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::MAIN_GAME);

        /// RESET KLAWISZY
        CEventHandler::resetDecisions();
    }
}

///-----
void CMario::draw(const unique_ptr<sf::RenderWindow>& window)
{
    if(m_shield_pointer)
        m_shield_pointer->draw(window,{CMarioGame::instance().getViewPosition().x-CMarioGame::s_size_window.x/2.0f+70,20});

    if(m_flashing_mario&&flashing())
        return;

    m_animations->draw(window);
}

///------------
inline void CMario::remove()
{
    if(CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::MARIO_DEAD))
    {
        removeObject(this);
        s_lives_mario--;
    }
}

///------
void CMario::setShield()
{
    if(m_shield_pointer||m_dead)
        return;

    CGUI::setVisibleMainLables(false);

    createShieldPointer(CGUI::createRectangleShape({0,0,500,30},sf::Color::Black,false,true)
            ,sf::Color::Blue,CShieldPointer::KindsOrigin::LEFT,CGUI::createSprite("Mario_right",{0,96,32,32},{0,0},1.1f,true));
    
    // ZWIEKSZAM SILE SKOKU
    m_jump_force *= 1.2f;
}

///-------
inline void CMario::setFlashing(float how_long)
{
    m_flashing_timer=CScen::getDurationScen()+how_long;
    m_flashing_iterativity_timer=0.0f;
    m_flashing_mario=true;
}

///-----------
inline bool CMario::flashing()
{
    if(m_flashing_timer<CGameScen::getDurationScen())
    {
        m_flashing_mario=false;
        return true;
    }else
    {
        if(m_flashing_iterativity_timer<CScen::getDurationScen())
        {
            m_visible_while_flashing=!m_visible_while_flashing;
            m_flashing_iterativity_timer=CScen::getDurationScen()+m_flashing_iterativity;
        }

        return m_visible_while_flashing;
    }
}

///----------
inline void CMario::crouch()
{
    if(!m_jump&&m_kind_movement==KindMovement::STANDING)
    {
        if(s_level_mario==LevelMario::BIG_MARIO)
            if(m_right_dir_reversal)
                m_animations->play(MyKindsAnimations::R_CROUCH,m_current_position);
            else
                m_animations->play(MyKindsAnimations::L_CROUCH,m_current_position);

        m_iam_crouching=true;
    }
}

///---------
inline void CMario::collisionWithBoundsMap()
{
    if(getBounds().left<=0&&m_horizontal_velocity<0)
    {
        m_stop=m_curbing=false;
        m_horizontal_velocity=0.0f;

        if(m_kind_movement!=KindMovement::RIGHT_RUN)
        {
            m_kind_movement=KindMovement::STANDING;
            animationUpdtate();
        }

        m_current_position={getBounds().width/2.0f,m_current_position.y};
    }
}

///-------
void CMario::falling()
{
    CPhysicaltObject::falling();

    const float previous_width=m_animations->getGlobalBounds().width;

    animationUpdtate();

    /// GDY KLATKA DLA SKOKSU JEST WIEKSZA OD POPRZEDNIEJ MUSZE SKORYGOWAC POZYCJE
    /// BY MOC SPASC
    if(getBounds().width>previous_width)
    {
        if(m_horizontal_velocity>0)
            m_current_position.x+=getBounds().width-previous_width;
        else
            m_current_position.x-=getBounds().width-previous_width;

        m_animations->setPosition(m_current_position);
    }
}

///-----------------------------
void CMario::jump()
{
    if(!m_jump)
    {
        CPhysicaltObject::jump();

        animationUpdtate();
        CMarioGame::s_sound_manager.play("jump_super");
    }
}

///--------------------
void CMario::hop(float force)
{
    CPhysicaltObject::hop(force);
    animationUpdtate();
}

///-------
inline void CMario::animationUpdtate()
{
    if(m_jump)
        if(m_right_dir_reversal)
            m_animations->play(CAnimations::R_JUMP,m_current_position);
        else
            m_animations->play(CAnimations::L_JUMP,m_current_position);
    else
    {
        if(m_kind_movement==KindMovement::STANDING)
            if(m_right_dir_reversal)
                m_animations->play(CAnimations::R_STANDING,m_current_position);
            else
                m_animations->play(CAnimations::L_STANDING,m_current_position);
        else
            if(m_right_dir_reversal)
                m_animations->play(CAnimations::R_MOVE,m_current_position);
            else
                m_animations->play(CAnimations::L_MOVE,m_current_position);
    }
}

///-------
void CMario::createDeathAnimation(const sf::Vector2f &pos)
{
    sf::Sprite * dead_mario=CGUI::createSprite("Mario_right",{192,96,32,32},pos,m_scale_to_tile,true);

    sf::Vector2f force_jump_death_mario;

    if(pos.y>CMarioGame::s_size_window.y)
        force_jump_death_mario={0,-1400.0f};
    else
        force_jump_death_mario={0,-1000.0f};

    CGuiObject* obj=new CSpecialEffects(new CSprite(dead_mario),CSpecialEffects::KindUpdate::JUMP,CMarioGame::s_size_window.y+CScen::s_tile_size,force_jump_death_mario);
    CGUI::addGuiObject(obj);

    resetLvlMario();
}

///----------------------
inline void CMario::createBigMarioAnimations()
{
    m_animations->reset();

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({32,36,32,60});
    m_frame_animation.push_back({65,36,30,60});
    m_frame_animation.push_back({96,38,32,58});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,m_movement_animation_speed,m_scale_to_tile);

    ///----------
    m_frame_animation.clear();
    m_frame_animation.push_back({448,36,32,60});
    m_frame_animation.push_back({418,36,30,60});
    m_frame_animation.push_back({384,38,32,58});

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Mario_left"],m_frame_animation,m_movement_animation_speed,m_scale_to_tile);

    ///----------
    m_frame_animation.clear();
    m_frame_animation.push_back({0,38,32,58});
    m_frame_animation.push_back({4,96,24,32});

    m_animations->create(MyKindsAnimations::R_LVL_DOWN,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.5f,m_scale_to_tile);

    ///----------
    m_frame_animation.clear();
    m_frame_animation.push_back({480,38,32,58});
    m_frame_animation.push_back({484,96,24,32});

    m_animations->create(MyKindsAnimations::L_LVL_DOWN,CMarioGame::s_texture_manager["Mario_left"],m_frame_animation,1.5f,m_scale_to_tile);

    ///---------------- ANIMACJE JEDNO KLATKOWE------------////////////
    m_animations->create(CAnimations::L_STANDING,CMarioGame::s_texture_manager["Mario_left"],{480,38,32,58},m_scale_to_tile);
    m_animations->create(CAnimations::R_STANDING,CMarioGame::s_texture_manager["Mario_right"],{0,38,32,58},m_scale_to_tile);
    m_animations->create(CAnimations::R_JUMP,CMarioGame::s_texture_manager["Mario_right"],{160,35,32,60},m_scale_to_tile);
    m_animations->create(CAnimations::L_JUMP,CMarioGame::s_texture_manager["Mario_left"],{320,35,32,60},m_scale_to_tile);

    m_animations->create(MyKindsAnimations::L_SHOOT,CMarioGame::s_texture_manager["Mario_left"],{258,36,29,60},m_scale_to_tile);
    m_animations->create(MyKindsAnimations::R_SHOOT,CMarioGame::s_texture_manager["Mario_right"],{225,36,29,60},m_scale_to_tile);

    m_animations->create(MyKindsAnimations::L_CROUCH,CMarioGame::s_texture_manager["Mario_left"],{287,50,32,46},m_scale_to_tile);
    m_animations->create(MyKindsAnimations::R_CROUCH,CMarioGame::s_texture_manager["Mario_right"],{192,50,32,46},m_scale_to_tile);

}

///--------------
inline void CMario::createSmalMarioAnimations()
{
    m_animations->reset();

    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({36,96,24,32});
    m_frame_animation.push_back({68,96,24,32});
    m_frame_animation.push_back({97,96,30,32});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,m_movement_animation_speed,m_scale_to_tile);

    ///---------
    m_frame_animation.clear();
    m_frame_animation.push_back(sf::IntRect(452,96,24,32));
    m_frame_animation.push_back(sf::IntRect(420,96,24,32));
    m_frame_animation.push_back(sf::IntRect(385,96,30,32));

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Mario_left"],m_frame_animation,m_movement_animation_speed,m_scale_to_tile);

    ///---------
    m_frame_animation.clear();
    m_frame_animation.push_back({4,96,24,32});
    m_frame_animation.push_back({0,38,32,58});

    m_animations->create(MyKindsAnimations::R_LVL_UP,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.5f,m_scale_to_tile);

    ///---------
    m_frame_animation.clear();
    m_frame_animation.push_back({484,96,24,32});
    m_frame_animation.push_back({480,38,32,58});

    m_animations->create(MyKindsAnimations::L_LVL_UP,CMarioGame::s_texture_manager["Mario_left"],m_frame_animation,1.5f,m_scale_to_tile);

    ///---------------- ANIMACJE JEDNO KLATKOWE------------////////////
    m_animations->create(CAnimations::L_STANDING,CMarioGame::s_texture_manager["Mario_left"],{484,96,24,32},m_scale_to_tile);
    m_animations->create(CAnimations::R_STANDING,CMarioGame::s_texture_manager["Mario_right"],{4,96,24,32},m_scale_to_tile);
    m_animations->create(CAnimations::R_JUMP,CMarioGame::s_texture_manager["Mario_right"],{160,96,32,32},m_scale_to_tile-0.1f);
    m_animations->create(CAnimations::L_JUMP,CMarioGame::s_texture_manager["Mario_left"],{320,96,32,32},m_scale_to_tile-0.1f);
}
