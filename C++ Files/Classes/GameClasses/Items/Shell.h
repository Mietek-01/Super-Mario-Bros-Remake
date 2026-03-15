#pragma once
#include "../PhysicalObject.h"

class Shell : public PhysicalObject {
    bool mActive = false;

public:
    Shell(sf::Vector2f pPos);
    ~Shell() {}

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;
    void ActOnMe(KindAction pAction) override;

    void Update() override;
};
