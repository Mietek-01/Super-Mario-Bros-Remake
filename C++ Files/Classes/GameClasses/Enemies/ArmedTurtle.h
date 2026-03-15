#pragma once
#include "../PhysicalObject.h"

#include <memory>
#include <vector>

class ArmedTurtle : public PhysicalObject {
    class Hammer : public GameObject {
        int mValueRotate = -8;

        bool mEnabled = false;
        bool mIsRightDirReversal;

        float mHorizontalAcceleration = -3.0f;
        float mForce = -700.0f;

    public:

        Hammer(sf::Vector2f, bool);
        ~Hammer() {}

        void ActOnObject(GameObject*, KindCollision) override;
        void ActOnMe(KindAction) override {}

        void Update() override;

        void SetEnabled(bool);
        void ChangeReversal(sf::Vector2f);

    }* mMyHammer = nullptr;

    enum MyKindsAnimations {
        R_MOVE_WITH_HAMMER,
        L_MOVE_WITH_HAMMER,
        R_MOVE_THROWING_HAMMER,
        L_MOVE_THROWING_HAMMER,
    };

    static const float sLengthRoad;
    static const float sCorrectionPosHammer;
    static const float sForceHop;

    const int mAfterWhatTimeJump;
    const int mAfterWhatTimeThrowHammer;

    float mWhenJump;
    float mWhenThrowHammer;
    float mWhereChangeDirection;

    int mForWhichBlockBottomCollision;

    bool mCanJump = false;
    bool mDownJump = false;

    /// Methods
    inline void ChangeLookingDirection();
    inline void ChangeMovementDirection();
    inline bool RandKindJump();
    inline void CreateHammer();
    inline void SetWhenJump();

    void Jump() override;
    void UpdateForCollisionWithBlock(KindCollision, Block*) override;

public:

    ArmedTurtle(sf::Vector2f);
    ~ArmedTurtle() {}

    void Update() override;
    void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;
};
