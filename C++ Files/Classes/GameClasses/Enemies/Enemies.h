#pragma once
#include "../PhysicalObject.h"
#include "ArmedTurtle.h"
#include "Bowser.h"

#include <memory>
#include <vector>

class Goombas : public PhysicalObject {
    const int mDamageValue = 100;
    const bool mCreatedByBowser;

    void UpdateForCollisionWithBlock(KindCollision, Block*) override;

public:

    Goombas(sf::Vector2f, KindMovement = PhysicalObject::KindMovement::LeftRun, bool = false);
    ~Goombas() {}

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;
};

///----------
class Turtle : public PhysicalObject {
    const bool mCreatedByFlyingTurtle;
    bool mCloseToFall = false;
    sf::FloatRect mFuturePosition;

    inline void DontFall();

public:

    Turtle(sf::Vector2f, bool = false, KindMovement = KindMovement::LeftRun);
    ~Turtle() {}

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;

    void Update() override;
};

///--------
class RedTurtle : public PhysicalObject {
    const int mDamageValue = 150;
    const bool mCreatedByBowser;

    void UpdateForCollisionWithBlock(KindCollision, Block*) override;

public:

    RedTurtle(sf::Vector2f, KindMovement = PhysicalObject::KindMovement::LeftRun, bool = false);
    ~RedTurtle() {}

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;
};

///------
class ArmoredTurtle : public PhysicalObject {

public:

    ArmoredTurtle(sf::Vector2f);
    ~ArmoredTurtle() {}

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;
};

///---------
class FlyingTurtle : public PhysicalObject {
    static const float sMyGravitation;
    void MakeJump() override;

    void UpdateForCollisionWithBlock(KindCollision, Block*) override;

public:

    FlyingTurtle(sf::Vector2f);
    ~FlyingTurtle() {}

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;
};

///-----
class CreatorRedTurtles : public GameObject {
    class RedTurtleShell : public PhysicalObject {
        const float mBasicYPos;
        void UpdateForCollisionWithBlock(KindCollision, Block*) override;

    public:
        RedTurtleShell(sf::Vector2f, bool);
        ~RedTurtleShell() {}

        void ActOnObject(GameObject*, KindCollision) override;
        void ActOnMe(KindAction) override {}

        bool IsVisible() const override { return true; }
    };

    bool mRightDir = true;
    bool mEnable = false;
    bool mCurbing = false;
    bool mSetChangingDirPos = true;
    bool mLeaveMap = false;
    bool mLoadingShoot = false;

    const float mLeavePosition;
    const sf::Vector2f mActivationPosition;

    const float mValueAcceleration = 0.07f;
    const float mMaxSpeed = 4.5f;

    const float mIterativityLoadingShoot = 1.5f;
    const float mLoadingShootTime = 0.3f;

    const int mRange = 350;
    const int mWhenCurbing = 150;

    float mChangeDirPosition;
    float mSpeed = 0.0f;
    float mDoubleSpeed = 1.0f;
    float mWhenLoadShoot;
    float mWhenShoot;

    inline void SetChangeDirPosition();
    inline void ChangeDirection();
    inline void Move();
    inline void Shoot();

    inline void SetWhenLoadShoot();
    inline void SetWhenShoot();

public:

    CreatorRedTurtles(sf::Vector2f);
    ~CreatorRedTurtles() {}

    void Update() override;
    void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;

    bool IsVisible() const override { return true; }
};
