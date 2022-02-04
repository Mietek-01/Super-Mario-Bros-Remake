#pragma once
#include "../GameObject.h"

using namespace std;

class CBullet: public CGameObject
{
protected:

    const sf::Vector2f m_basic_position;

    const bool m_right_dir;

    const float m_speed;
    const float m_my_scal;
    const float m_range=1500;
    const int m_damage_value=100;

    virtual void corectPosition()=0;

public:

    CBullet(sf::Vector2f,bool,float,float);
    virtual ~CBullet(){}

    virtual void update()override;

    void actOnMe(KindAction)override;
    void actOnObject(CGameObject*,KindCollision)override;

    bool isVisible()const override {return true;}
};

///--------
class CBasicBullet: public CBullet
{
    void corectPosition()override{}

public:

    CBasicBullet(sf::Vector2f,bool);
    virtual ~CBasicBullet(){}

};


///---------
class CSinBullet: public CBullet
{
    const float m_sin_function_speed=3.14159265/80.0f;
    float m_my_speed=0.0f;

    void corectPosition()override;

public:

    CSinBullet(sf::Vector2f,bool);
    virtual ~CSinBullet(){}
};

///---------
class CCircleBullet: public CBullet
{
    const float m_radius=18.0f;/// ZASIEK KOLA
    const float m_spin_speed=0.05f;/// PREDKOSC ROBIENIA KOLEK
    const float m_rotate_speed=4;/// OBROT POCISKU

    float m_center_circle_x_pos;
    float m_angle=0;
    float m_rotate=0;

    void corectPosition()override;

public:

    CCircleBullet(sf::Vector2f,bool);
    virtual ~CCircleBullet(){}

    void update()override;

};



