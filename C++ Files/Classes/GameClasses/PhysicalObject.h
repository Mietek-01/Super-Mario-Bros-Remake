#pragma once
#include "GameObject.h"
#include "Blocks/Block.h"

#include <map>

class Block;

class PhysicalObject : public GameObject {
protected:
    enum class KindMovement {
        LeftRun,
        RightRun,
        Standing
    } mKindMovement;

    const float mFallingForce = 250.0f;
    float mForce = 0.0f;
    float mValueAcceleration = 0.1f;
    float mJumpForce = -900.0f;

    bool mIsRightDirReversal;
    bool mIsJump = false;
    bool mIsInBottomCollision = true;

    // Methods
    virtual void UpdateForCollisionWithBlock(KindCollision, Block*);
    virtual void Jump();
    virtual void Falling();
    virtual void MakeJump();

    void Move();

public:
    const static float sGravitation;
    const static float sCorrectionToBottomCollision;

    PhysicalObject(Animator*, Parentage, const sf::Vector2f&, KindMovement = KindMovement::LeftRun, int = 100);
    virtual ~PhysicalObject() {}

    virtual void Update() override;
    virtual void Hop(float);

    void PhysicGameWithBlock(std::map<KindCollision, Block*>&, bool);

    [[nodiscard]] bool IsJumping() const { return mIsJump; }
    [[nodiscard]] bool IsInBottomCollision() const { return mIsInBottomCollision; }
};
