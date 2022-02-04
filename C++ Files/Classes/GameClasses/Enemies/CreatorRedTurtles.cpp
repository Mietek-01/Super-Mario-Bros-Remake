#include "Enemies.h"
#include "../../Scens/GameScen.h"
#include <math.h>

///-------------CRedTurtleShall--------------///

CCreatorRedTurltes::CRedTurtleShall::CRedTurtleShall(sf::Vector2f pos,bool right_dir)
:CPhysicaltObject(new CSprite(CGUI::createSprite("Enemies_right",{355,81,26,30},{pos.x,pos.y-1},m_scale_to_tile,true)),Parentage::ITEM,pos,static_cast<CPhysicaltObject::KindMovement>((int)!right_dir))
,m_basic_y_pos(pos.y)
{
    m_value_acceleration=1;
    hop(-500);
}

///--------
void CCreatorRedTurltes::CRedTurtleShall::updateForCollisionWithBlock(KindCollision how_collision,CBlock* block)
{
    if(m_current_position.y<m_basic_y_pos)
        return;

    switch(how_collision)
    {
        case KindCollision::BOTTOM:
        {
            block->actOnObject(this,how_collision);

            m_current_position.y-=0.1f;
            addNewObject(new CRedTurtle(m_current_position,m_kind_movement));

            removeObject(this);
            return;
        }

        case KindCollision::NONE:
        {
            return;
        }

    }

    block->actOnObject(this,how_collision);
}

///--------
void CCreatorRedTurltes::CRedTurtleShall::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
      obj->actOnMe(KindAction::HIT);
}

///-------------CCreatorRedTurltes--------------///
CCreatorRedTurltes::CCreatorRedTurltes(sf::Vector2f pos)
:CGameObject(new CAnimations,Parentage::ENEMY,pos,1000)
,m_acitivation_position(pos)
,m_leave_position(pos.x+2300)
{
    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Enemies_right"],{448,127,32,49},m_scale_to_tile);
    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Enemies_left"],{32,127,32,49},m_scale_to_tile);
    m_animations->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Enemies_right"],{480,143,32,32},m_scale_to_tile);

    m_current_position.x-=CMarioGame::s_size_window.x/2.0f+100;
    m_current_position.y = -500;/// ZAPOBIEGA PRZYJMOWANIA POCISKOW MARIA
    m_animations->play(CAnimations::R_MOVE,m_current_position);

    setWhenLoadShoot();
}

///------
void CCreatorRedTurltes::update()
{
    if(!m_enable)
    {
        if(CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x > m_acitivation_position.x)
        {
            m_current_position.y = m_acitivation_position.y;
            m_enable = true;
        }
    }else
    {
        shoot();
        move();
    }

    m_animator->update(m_current_position);
}

///-----
void CCreatorRedTurltes::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        if(how_collision==KindCollision::BOTTOM)
        {
            corectObjectPositionOnMe(*obj,how_collision);
            obj->actOnMe(KindAction::HOP);

            this->actOnMe(KindAction::HIT);
        }
        else
            obj->actOnMe(KindAction::HIT);
}

///-------
void CCreatorRedTurltes::actOnMe(KindAction how_action)
{
    if(how_action==KindAction::HIT)
        createBeatsObjSpecialEfect(-100.0f);
}

///-----
inline void CCreatorRedTurltes::shoot()
{
    if(m_leave_map&&!m_loading_shoot)return;

    if(m_when_load_shoot<CScen::getDurationScen())
    {
        if(!m_loading_shoot)
        {
            m_animations->play(CAnimations::STANDARD,m_current_position);
            m_loading_shoot=true;
            setWhenShoot();

        }else if(m_when_shoot<CScen::getDurationScen())
        {
            m_loading_shoot=false;
            setWhenLoadShoot();

            const bool reversal_to_mario=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x<m_current_position.x;

            addNewObject(new CRedTurtleShall(m_current_position,reversal_to_mario));

            if(reversal_to_mario)
                m_animations->play(CAnimations::L_MOVE,m_current_position);
            else
                m_animations->play(CAnimations::R_MOVE,m_current_position);
        }

    }
}

///------
inline void CCreatorRedTurltes::move()
{
    const float mario_position=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x;

    if(fabs(m_current_position.x-mario_position)>400)
        m_double_speed=1.5f;
    else
        m_double_speed=1.0f;

    if(m_right_dir)
    {
        if(m_current_position.x>=m_leave_position&&m_current_position.x<mario_position)
        {
            m_leave_map=true;
            m_curbing=true;
        }

        if(!m_curbing)
        {
            m_speed+=m_value_acceleration;
            if(m_speed>m_max_speed*m_double_speed)
                m_speed=m_max_speed*m_double_speed;

            // CZYLI GDY PRZESCIGNE MARIA
            if(m_set_changing_dir_pos && mario_position < m_current_position.x)
                setChangeDirPosition();

            // OKRESLAM KIEDY ZACZAC CHAMOWAC
            if(!m_set_changing_dir_pos && m_current_position.x>m_change_dir_position-m_when_curbing)
                m_curbing=true;
        }else
        {
            m_speed-=m_value_acceleration*0.8;
            if(m_speed<0)
            {
               changeDirection();
               m_speed=0.0f;
            }
        }

    }else
    {
        if(!m_curbing)
        {
            m_speed-=m_value_acceleration;
            if(m_speed<-m_max_speed*m_double_speed)
                m_speed=-m_max_speed*m_double_speed;

            if(m_leave_map)
            {
                m_current_position.y-=0.5f;
                if(m_current_position.y<-10)
                    removeObject(this);

            }else
            {
                if(m_set_changing_dir_pos && mario_position > m_current_position.x)
                    setChangeDirPosition();

                if(!m_set_changing_dir_pos&&m_current_position.x<m_change_dir_position+m_when_curbing)
                    m_curbing=true;
            }

        }else
        {
            m_speed+=m_value_acceleration*0.8;
            if(m_speed>0)
            {
                changeDirection();
                m_speed=0.0f;
            }
        }
    }

    m_current_position.x+=m_speed;
}

///-------
inline void CCreatorRedTurltes::setChangeDirPosition()
{
    // GDY PRZESCIGNE MARIA
    m_set_changing_dir_pos=false;

    if(m_right_dir)
        m_change_dir_position=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x+m_range;
    else
        m_change_dir_position=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x-m_range;

    if(!m_loading_shoot)
    if(m_right_dir)
        m_animations->play(CAnimations::L_MOVE,m_current_position);
    else
        m_animations->play(CAnimations::R_MOVE,m_current_position);
}

///-------
inline void CCreatorRedTurltes::changeDirection()
{
    /// GDY DOJDE DO POZYCJI ZMIANY KIERUNKU
    m_right_dir=!m_right_dir;
    m_curbing=false;
    m_set_changing_dir_pos=true;
}

///-------
inline void CCreatorRedTurltes::setWhenLoadShoot()
{
    m_when_load_shoot=CScen::getDurationScen()+m_iterativity_loading_shoot;
}

///-------
inline void CCreatorRedTurltes::setWhenShoot()
{
    m_when_shoot=CScen::getDurationScen()+m_loading_shoot_time;
}

///--------
void CCreatorRedTurltes::draw(const unique_ptr<sf::RenderWindow>&window)
{
    if(m_enable)
        m_animator->draw(window);
}
