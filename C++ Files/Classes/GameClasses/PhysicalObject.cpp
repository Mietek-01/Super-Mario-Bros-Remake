#include "PhysicalObject.h"
#include <math.h>

using namespace std;

const float PhysicalObject::sGravitation = 2600.0f;
const float PhysicalObject::sCorrectionToBottomCollision = 0.2f;

PhysicalObject::PhysicalObject(Animator* pAnimator, Parentage pParentage, const sf::Vector2f& pPos, KindMovement pHowMovement, int pPoints)
    : GameObject(pAnimator, pParentage, pPos, pPoints)
    , mKindMovement(pHowMovement)
{
    mCurrentPosition.y += 0.1f;

    if (pHowMovement == KindMovement::LeftRun || pHowMovement == KindMovement::Standing) {
        mIsRightDirReversal = false;
    } else {
        mIsRightDirReversal = true;
    }
}

///-----------
void PhysicalObject::Update()
{
    mPreviousPosition = mCurrentPosition;

    // Ensures correct operation of collision detection system
    if (mIsInBottomCollision) {
        mPreviousPosition.y -= sCorrectionToBottomCollision;
    }

    Move();

    if (mIsJump) {
        MakeJump();
    }

    mAnimator->update(mCurrentPosition);
}

///-----
void PhysicalObject::UpdateForCollisionWithBlock(KindCollision pHowCollision, Block* pBlock)
{
    // Defines game physics: reactions to blocks
    switch (pHowCollision) {
        case KindCollision::Top: {
            mForce = 0.0f;
            break;
        }

        case KindCollision::Bottom: {
            if (pBlock->IsHit() || pBlock->IsDead()) {
                ActOnMe(KindAction::Hit);
            } else {
                mIsJump = false;
                mIsInBottomCollision = true;
            }

            break;
        }

        // Defined for enemies
        case KindCollision::RightSide: {
            mIsRightDirReversal = false;
            mKindMovement = KindMovement::LeftRun;

            if (mAnimations) {
                mAnimations->play(Animations::LeftMove, mCurrentPosition);
            }

            break;
        }

        case KindCollision::LeftSide: {
            mIsRightDirReversal = true;
            mKindMovement = KindMovement::RightRun;

            if (mAnimations) {
                mAnimations->play(Animations::RightMove, mCurrentPosition);
            }

            break;
        }

        case KindCollision::None: {
            return;
        }
    }

    pBlock->ActOnObject(this, pHowCollision);
}

///---------------------------
void PhysicalObject::PhysicGameWithBlock(map<KindCollision, Block*>& pCollisions, bool pNoneCollision)
{
    if (!mIsJump && pNoneCollision) {
        Falling();
        return;
    }

    ///---------
    if (pCollisions[KindCollision::RightSide]) {
        UpdateForCollisionWithBlock(KindCollision::RightSide, pCollisions[KindCollision::RightSide]);
    }

    ///--------
    if (pCollisions[KindCollision::LeftSide]) {
        UpdateForCollisionWithBlock(KindCollision::LeftSide, pCollisions[KindCollision::LeftSide]);
    }

    ///----------
    if (pCollisions[KindCollision::Top]) {
        if (pCollisions[KindCollision::LeftSide] || pCollisions[KindCollision::RightSide]) {
            // Check if still in this collision after correcting for side collisions
            if (KindCollision::Top == this->HowCollision(*pCollisions[KindCollision::Top])) {
                UpdateForCollisionWithBlock(KindCollision::Top, pCollisions[KindCollision::Top]);
            }
        } else {
            UpdateForCollisionWithBlock(KindCollision::Top, pCollisions[KindCollision::Top]);
        }
    }

    ///-----------------
    if (pCollisions[KindCollision::Bottom]) {
        if (pCollisions[KindCollision::LeftSide] || pCollisions[KindCollision::RightSide]) {
            // Check if still in this collision after correcting for side collisions
            if (KindCollision::Bottom == this->HowCollision(*pCollisions[KindCollision::Bottom])) {
                UpdateForCollisionWithBlock(KindCollision::Bottom, pCollisions[KindCollision::Bottom]);
            }
        } else {
            UpdateForCollisionWithBlock(KindCollision::Bottom, pCollisions[KindCollision::Bottom]);
        }
    }
}

///----------------------
void PhysicalObject::Move()
{
    switch (mKindMovement) {
        case KindMovement::RightRun: mCurrentPosition.x += mValueAcceleration; break;
        case KindMovement::LeftRun: mCurrentPosition.x -= mValueAcceleration; break;
    }
}

///--------------------------
void PhysicalObject::MakeJump()
{
    mForce += sGravitation * Scene::GetFrameTime();
    mCurrentPosition.y += mForce * Scene::GetFrameTime();

    CheckUnderMap();
}

///-------------------------------
void PhysicalObject::Falling()
{
    mIsInBottomCollision = false;
    mIsJump = true;
    mForce = mFallingForce;
}

///-----------------------------
void PhysicalObject::Jump()
{
    mForce = mJumpForce;
    mIsJump = true;
    mIsInBottomCollision = false;
}

///--------------------
void PhysicalObject::Hop(float pForce)
{
    mForce = pForce;
    mIsJump = true;
    mIsInBottomCollision = false;
}
