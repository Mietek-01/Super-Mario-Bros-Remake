#pragma once
#include "../GameObject.h"

#include <vector>

class Bullet : public GameObject {
protected:

    const sf::Vector2f mBasicPosition;

    const bool mRightDir;

    const float mSpeed;
    const float mMyScale;
    const float mRange = 1500;
    const int mDamageValue = 100;

    virtual void CorrectPosition() = 0;

public:

    Bullet(sf::Vector2f pPos, bool pRightDir, float pSpeed, float pScale);
    virtual ~Bullet() {}

    virtual void Update() override;

    void ActOnMe(KindAction pAction) override;
    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;

    bool IsVisible() const override { return true; }
};

///--------
class BasicBullet : public Bullet {
    void CorrectPosition() override {}

public:

    BasicBullet(sf::Vector2f pPos, bool pRightDir);
    virtual ~BasicBullet() {}
};

///---------
class SinBullet : public Bullet {
    const float mSinFunctionSpeed = 3.14159265f / 80.0f;
    float mMySpeed = 0.0f;

    void CorrectPosition() override;

public:

    SinBullet(sf::Vector2f pPos, bool pRightDir);
    virtual ~SinBullet() {}
};

///---------
class CircleBullet : public Bullet {
    const float mRadius = 18.0f;
    const float mSpinSpeed = 0.05f;
    const float mRotateSpeed = 4;

    float mCenterCircleXPos;
    float mAngle = 0;
    float mRotate = 0;

    void CorrectPosition() override;

public:

    CircleBullet(sf::Vector2f pPos, bool pRightDir);
    virtual ~CircleBullet() {}

    void Update() override;
};
