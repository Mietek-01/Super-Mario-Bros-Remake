#pragma once
#include "../Blocks/Block.h"

using namespace std;

class CGun: public CStaticBlock
{
    class CBullet:public CGameObject
    {
        const float m_speed=2.0f;
        const bool m_right_dir;

        int m_range=1500;

        bool m_the_end_range=false;

    public:

        CBullet(sf::Vector2f,bool);
        ~CBullet(){}

        void update()override;

        void actOnMe(KindAction)override;
        void actOnObject(CGameObject*,KindCollision)override;

        bool isVisible()const override {return true;}

    };

    float m_when_shoot;

    inline void setWhenShoot();

public:

    enum class KindGun
    {
        SMALL=32,
        BIG=64
    };

    CGun(sf::Vector2f,KindGun);
    ~CGun(){}

    void update()override;
};




