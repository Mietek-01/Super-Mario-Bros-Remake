#pragma once
#include "../PhysicalObject.h"

class CShell:public CPhysicaltObject
{
    bool m_active=false;
public:
    CShell(sf::Vector2f);
    ~CShell(){}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

    void update()override;

};



