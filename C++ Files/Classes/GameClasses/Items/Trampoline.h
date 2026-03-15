#pragma once
#include "../PhysicalObject.h"

class Trampoline : public GameObject {
    const float mSpeed = 1.2f;
    const int mJumpForce = -1500;

    bool mEnabled = false;

    enum MyAnimations {
        TrampolineActivation
    };

    inline void Activate(PhysicalObject* pObjectOnMe);

public:

    Trampoline(sf::Vector2f pPos);
    ~Trampoline() {}

    void Update() override;

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;
    void ActOnMe(KindAction pAction) override {}
};
