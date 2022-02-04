#pragma once
#include "../GameObject.h"

using namespace std;

class CLavaBullet: public CGameObject
{
    enum MyAnimations
    {
        GO_DOWN
    };

    static const float s_my_gravitation;

    const sf::Vector2f m_basic_position;

    const float m_jump_force=-1200;

    float m_force;
    float m_when_jump;

    bool m_go_up=true;

    inline void move();
    inline void setWhenJump();

public:

    CLavaBullet(sf::Vector2f);
    ~CLavaBullet(){}

    void update()override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction kind_action)override{}

};




