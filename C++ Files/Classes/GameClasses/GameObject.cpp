#include "GameObject.h"
#include "../Scens/GameScen.h"
#include "../GUIClasses/FlowText.h"

#include <math.h>

vector<CGameObject* > CGameObject::s_removed_objects;
vector<CGameObject* > CGameObject::s_new_objects;

const float CGameObject::m_scale_to_tile=48.0f/32.0f;

void CGameObject::addNewObject(CGameObject* obj)
{
    s_new_objects.push_back(obj);
}

///------
void CGameObject::removeObject(CGameObject* obj_to_remove)
{
    /// ZAPOBIEGA WIELOKROTNEMU DODAWANIU
   for(auto obj:s_removed_objects)
    if(obj==obj_to_remove)
        return;

    obj_to_remove->m_dead=true;
    s_removed_objects.push_back(obj_to_remove);
}

///------------------CGameObject------------------///
CGameObject::CGameObject(CAnimator* animator,Parentage parantage,const sf::Vector2f &pos,int points)
:
m_animator(animator)
,m_my_value_points(points)
,m_parentage(parantage)
,m_animations(dynamic_cast<CAnimations*>(animator))
{
    m_previous_position=m_current_position=pos;
}

///---
void CGameObject::corectObjectPositionOnMe(CGameObject& obj,KindCollision how_collision)const
{
    switch(how_collision)
    {
        case KindCollision::BOTTOM:
        {
            obj.setPosition({obj.getCurrentPosition().x,m_current_position.y-getSize().y+0.1f});
            break;
        }

        case KindCollision::TOP:
        {
            obj.setPosition({obj.getCurrentPosition().x,m_current_position.y+obj.getBounds().height+0.5f});
            break;
        }

        case KindCollision::RIGHT_SIDE:
        {
            obj.setPosition({((m_current_position.x-getSize().x/2.0f)-obj.getBounds().width/2.0f)-0.2f,obj.getCurrentPosition().y});
            break;
        }

        case KindCollision::LEFT_SIDE:
        {
            obj.setPosition({(m_current_position.x+getSize().x/2.0f+obj.getBounds().width/2.0f)+0.2f,obj.getCurrentPosition().y});
            break;
        }
    }
}

///------
CGameObject::KindCollision CGameObject::specialCheckingHowCollision(const CGameObject& obj)const
{
    const sf::FloatRect &obj_prev_bounds=obj.getPreviousBounds();

    /// GDY POPRZEDNIA POZYCJA OBJ NAKLADA SIE Z MOJA OBECNA POZ
    if(this->getBounds().intersects(obj_prev_bounds))
    {
        if(this->m_previous_position.y<obj_prev_bounds.top)
            return KindCollision::BOTTOM;
        else
        if((this->m_previous_position.y-this->getSize().y)>obj_prev_bounds.top+obj_prev_bounds.height)
            return KindCollision::TOP;
        else
        {
            if(this->m_previous_position.x<obj_prev_bounds.left)
                return KindCollision::RIGHT_SIDE;
            else
                return KindCollision::LEFT_SIDE;
        }

    }else
    {
        const KindCollision how_collision=obj.howCollision(*this);

        switch(how_collision)
        {
            case KindCollision::BOTTOM:return KindCollision::TOP;
            case KindCollision::TOP:return KindCollision::BOTTOM;
            case KindCollision::LEFT_SIDE:return KindCollision::RIGHT_SIDE;
            case KindCollision::RIGHT_SIDE:return KindCollision::LEFT_SIDE;
        }
    }
}

///-----
CGameObject::KindCollision CGameObject::howCollision(const CGameObject& obj)const
{
    /// ZWRACAM JAKA KOLIZJE MAM Z PRZEKAZANYM OBIEKTEM

    const sf::FloatRect &obj_bounds=obj.getBounds();

    if(this->getBounds().intersects(obj_bounds))
    {
        /// JEST TO PRZYPADEK PESYMISTYCZNY
        /// BO POPRZEDNIA POZYCJA NAKLADA SIE Z OBECNA POZ OBJ
        if(this->getPreviousBounds().intersects(obj_bounds))
            return specialCheckingHowCollision(obj);
        else
        if(this->m_previous_position.y<obj_bounds.top)
            return KindCollision::BOTTOM;
        else
        if((this->m_previous_position.y-this->getSize().y)>obj_bounds.top+obj_bounds.height)
            return KindCollision::TOP;
        else
        {
            if(m_previous_position.x<obj_bounds.left)
                return KindCollision::RIGHT_SIDE;
            else
                return KindCollision::LEFT_SIDE;
        }

    }else return KindCollision::NONE;
}

///---
void CGameObject::createPoints()const
{
    sf::Text *text=CGUI::createText(CGUI::toString(m_my_value_points),{0,0},sf::Color::Red);
    text->setPosition(m_current_position.x,m_current_position.y-this->getBounds().height/2.0f);

    CGUI::addGuiObject(new CFlowText(text,m_current_position.y-getBounds().height/2.0f-CScen::s_tile_size*3));

    CGameScen::addPoints(m_my_value_points);
}

///------
void CGameObject::createBeatsObjSpecialEfect(float m_hop_force)
{
    sf::Sprite * hit_obj=new sf::Sprite(m_animator->getSprite());

    hit_obj->setRotation(180);
    hit_obj->setPosition(hit_obj->getPosition().x,hit_obj->getPosition().y-hit_obj->getGlobalBounds().height);

    CGuiObject* obj=new CSpecialEffects(new CSprite(hit_obj),CSpecialEffects::KindUpdate::JUMP,CMarioGame::s_size_window.y+CScen::s_tile_size,{0,m_hop_force});
    CGUI::addGuiObject(obj);

    createPoints();
    CMarioGame::s_sound_manager.play("stomp");

    removeObject(this);
}

///------------
void CGameObject::isUnderMap()
{
    if(m_current_position.y>CMarioGame::s_size_window.y+CScen::s_tile_size*3)
    {
        this->actOnMe(KindAction::UNDER_MAP);
        removeObject(this);
    }
}

///------
void CGameObject::movePosition(sf::Vector2f value)
{
    m_current_position.x+=value.x;
    m_current_position.y+=value.y;
    m_animator->setPosition(m_current_position);
}

///------
sf::FloatRect  CGameObject::getPreviousBounds()const
{
    return {m_previous_position.x-getBounds().width/2.0f,
    m_previous_position.y-getBounds().height,getBounds().width,getBounds().height};
}

///------------------
void CGameObject::draw(const unique_ptr<sf::RenderWindow>& window)
{
    m_animator->draw(window);
}


