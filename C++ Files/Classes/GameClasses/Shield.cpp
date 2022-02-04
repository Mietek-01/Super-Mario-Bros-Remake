#include "Shield.h"
#include "../GUIClasses/GUI.h"
#include <assert.h>
#include <iostream>

CShield::CShield(int shield_value)
:m_basic_shield_value(shield_value)
,m_shield_value(shield_value)
{
    assert(shield_value>200);
}

///-----
void CShield::createShieldPointer(sf::RectangleShape* rctng,sf::Color color,CShieldPointer::KindsOrigin kind_origin,sf::Sprite* sprite )
{
    m_shield_pointer.reset(new CShieldPointer(rctng,color,kind_origin,m_basic_shield_value,sprite));
}

///-----
void CShield::reduceShield()
{
    if(m_inflicted_damage==0)return;

    m_shield_value-=m_inflicted_damage;

    if(m_shield_pointer)
        m_shield_pointer->reduceShieldPointer(m_inflicted_damage);

    m_inflicted_damage=0;
}

///-------------------CShieldPointer-----------------////
const short CShield::CShieldPointer::s_margin=7;

CShield::CShieldPointer::CShieldPointer(sf::RectangleShape *first_rctg,sf::Color second_color,KindsOrigin kind_origin,int shield_value,sf::Sprite* sprite)
:m_second_rctg(new sf::RectangleShape({first_rctg->getSize().x-2*s_margin,first_rctg->getSize().y-2*s_margin}))
,m_basic_shield_value(shield_value)
,m_kind_origin(kind_origin)
,m_shield_pointer_lenght(first_rctg->getSize().x-2*s_margin)
,m_first_rctg(first_rctg)
{
    switch(kind_origin)
    {
    case KindsOrigin::LEFT:
        break;/// BO LEWY JEST ZAWSZE DOMYSLNY
    case KindsOrigin::RIGHT:
        m_first_rctg->setOrigin(m_first_rctg->getGlobalBounds().width,0);
        break;
    case KindsOrigin::BOTTOM:
        m_first_rctg->setOrigin(m_first_rctg->getGlobalBounds().width/2.0f,m_first_rctg->getGlobalBounds().height);
        break;
    }

    if(sprite)
        m_sprite.reset(sprite);

    m_second_rctg->setFillColor(second_color);
}

///---------
void CShield::CShieldPointer::update()
{
    if(m_damage_rctg)
    {
        if(m_damage_rctg->getSize().x-m_reduceing_shield_speed*m_extra_speed>0)
            m_damage_rctg->setSize({m_damage_rctg->getSize().x-m_reduceing_shield_speed*m_extra_speed,m_damage_rctg->getSize().y});
        else
        {
            m_extra_speed=1.0f;
            m_damage_rctg.reset();

            if(!m_second_rctg)
                m_the_end_shield=true;
        }
    }
}

///-------
inline void CShield::CShieldPointer::createDamageRectangle(float lenght_damage)
{
    if(!m_damage_rctg)
    {
        m_damage_rctg.reset(CGUI::createRectangleShape(sf::IntRect(m_second_rctg->getPosition().x+m_second_rctg->getSize().x-lenght_damage,m_second_rctg->getPosition().y,lenght_damage,m_second_rctg->getSize().y)
        ,m_color_damage_rctg,false));
    }else
    {
        m_damage_rctg->move({-lenght_damage,0});
        m_damage_rctg->setSize({m_damage_rctg->getSize().x+lenght_damage,m_damage_rctg->getSize().y});
    }
}

///--------
void CShield::CShieldPointer::reduceShieldPointer(float damage)
{
    if(!m_second_rctg)return;

    if(damage>=m_basic_shield_value)
    {
        this->createDamageRectangle(m_second_rctg->getSize().x);
        m_second_rctg.reset();
        m_extra_speed=3.0f;
    }
    else
    {
        const float procent_damage=damage/m_basic_shield_value;
        const float lenght_damage=m_shield_pointer_lenght*procent_damage;

        sf::Vector2f new_size=m_second_rctg->getSize();
        new_size.x-=lenght_damage;

        if((int)new_size.x<=0)
        {
            this->createDamageRectangle(m_second_rctg->getSize().x);
            m_second_rctg.reset();
            m_extra_speed=2.0f;
        }
        else
        {
            if(m_damage_rctg)
                m_extra_speed=1.5f;

            this->createDamageRectangle(lenght_damage);
            m_second_rctg->setSize(new_size);
        }
    }
}

///---------
inline void CShield::CShieldPointer::updatePosition(sf::Vector2f pos)
{
    m_first_rctg->setPosition(pos);

    if (m_sprite)m_sprite->setPosition(m_first_rctg->getGlobalBounds().left - m_sprite->getGlobalBounds().width, m_first_rctg->getGlobalBounds().top + m_first_rctg->getGlobalBounds().height);
    if (m_second_rctg)m_second_rctg->setPosition({ m_first_rctg->getGlobalBounds().left + s_margin*1.5f,m_first_rctg->getGlobalBounds().top + s_margin*1.5f });
    if (m_damage_rctg)
        if (m_second_rctg)
            m_damage_rctg->setPosition({ m_second_rctg->getPosition().x + m_second_rctg->getSize().x,m_second_rctg->getPosition().y });
        else
            m_damage_rctg->setPosition(m_first_rctg->getGlobalBounds().left + s_margin, m_first_rctg->getGlobalBounds().top + s_margin);
}

///------
void CShield::CShieldPointer::draw(const unique_ptr<sf::RenderWindow>&window,sf::Vector2f pos)
{
    this->updatePosition(pos);

    window->draw(*m_first_rctg);

    if(m_second_rctg)
        window->draw(*m_second_rctg);

    if(m_damage_rctg)
        window->draw(*m_damage_rctg);

    if(m_sprite)
        window->draw(*m_sprite);
}


