#pragma once
#include "../GameObject.h"

class LavaBullet : public GameObject {
    enum MyAnimations {
        GoDown
    };

    static const float sMyGravitation;

    const sf::Vector2f mBasicPosition;

    const float mJumpForce = -1200;

    float mForce;
    float mWhenJump;

    bool mGoUp = true;

    inline void Move();
    inline void SetWhenJump();

public:

    LavaBullet(sf::Vector2f pPos);
    ~LavaBullet() {}

    void Update() override;

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;
    void ActOnMe(KindAction pAction) override {}
};
