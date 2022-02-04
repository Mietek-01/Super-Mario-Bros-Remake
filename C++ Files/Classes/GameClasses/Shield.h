#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;

class CShield
{
protected:

    class CShieldPointer
    {

    public:

        enum class KindsOrigin
        {
            LEFT,
            RIGHT,
            BOTTOM
        };

        void draw(const unique_ptr<sf::RenderWindow>&,sf::Vector2f);
        void update();

        bool isTheEndShield()const{return m_the_end_shield;}

    private:

        static const short s_margin;

        const KindsOrigin m_kind_origin;

        unique_ptr<sf::Sprite> m_sprite;

        unique_ptr<sf::RectangleShape> m_first_rctg;
        unique_ptr<sf::RectangleShape> m_second_rctg;
        unique_ptr<sf::RectangleShape> m_damage_rctg;

        const float m_reduceing_shield_speed = 1.0f;
        const float m_basic_shield_value;
        const float m_shield_pointer_lenght;
        const sf::Color m_color_damage_rctg=sf::Color::White;

        bool m_the_end_shield=false;
        float m_extra_speed = 1.0f;

        ///METHODS
        friend class CShield;
        CShieldPointer(sf::RectangleShape*,sf::Color,KindsOrigin,int,sf::Sprite* =nullptr);

        inline void updatePosition(sf::Vector2f);
        inline void createDamageRectangle(float);

        void reduceShieldPointer(float);
    };

    unique_ptr<CShieldPointer> m_shield_pointer;

    void createShieldPointer(sf::RectangleShape*,sf::Color,CShieldPointer::KindsOrigin,sf::Sprite* =nullptr);
    void reduceShield();

private:

    int m_inflicted_damage=0;
    int m_shield_value;
    const int m_basic_shield_value;

public:

    CShield(int);
    void setDamage(int damage){m_inflicted_damage=damage;}
    int getShieldValue()const {return m_shield_value;}

};

