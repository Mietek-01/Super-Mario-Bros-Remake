#pragma once
#include "../PhysicalObject.h"

using namespace std;

class CTrampoline: public CGameObject
{
    const float m_speed=1.2f;
    const int m_jump_force=-1500;

    bool m_enable=false;

    enum MyAnimations
    {
        TRAMPOLINE_ACTIVATION
    };

    inline void activate(CPhysicaltObject*);

public:

    CTrampoline(sf::Vector2f);
    ~CTrampoline(){}

    void update()override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override{}
};




