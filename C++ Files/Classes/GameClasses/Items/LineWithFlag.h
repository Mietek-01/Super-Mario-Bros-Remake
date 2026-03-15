#pragma once
#include "../GameObject.h"

#include <memory>

class LineWithFlag : public GameObject {

    enum MyKindsAnimations {
        FlagAnimation,
        MovementMario,
        RRatchetingByMario,
        LRatchetingByMario,
    };

    enum class MarioAnimationsStates {
        LeftRatcheting,
        RightRatcheting,
        GoToCastle

    } mRatchetingFlagState = MarioAnimationsStates::LeftRatcheting;

    std::unique_ptr<sf::Sprite> mCastle;
    std::unique_ptr<Animations> mCastleFlag;
    std::unique_ptr<Animations> mMario;

    Animations mLineFlag;

    bool mIsFlagRatcheting = false;

    const float mCastlePosition;
    const float mCastleFlagSpeed = 0.3f;
    const float mFlagSpeedRatcheting = 2.2f;
    const float mMarioSpeedRatcheting = 2.5f;
    const float mMarioSpeed = 2.8f;

    sf::Vector2f mLineFlagPosition, mMarioPosition, mCastleFlagPosition;

public:

    LineWithFlag(sf::Vector2f pPos);
    ~LineWithFlag() {}

    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;
    void Update() override;

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;
    void ActOnMe(KindAction pAction) override;
};
