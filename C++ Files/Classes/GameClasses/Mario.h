#pragma once
#include "PhysicalObject.h"
#include "../EventHandler.h"
#include "Shield.h"

#include <map>
#include <memory>

class Mario : public PhysicalObject, public Shield {

public:

    enum class LevelMario {
        SmallMario,
        BigMario
    };

    Mario(sf::Vector2f pPos);
    ~Mario();

    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;
    void Update() override;

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override {}
    void ActOnMe(KindAction pAction) override;

    void ChangingLevel();
    void Hop(float pForce) override;

    static void CreateDeathAnimation(const sf::Vector2f& pPos);

    /// Getters
    static Mario::LevelMario GetLevelMario() { return sLevelMario; }
    static short GetLivesMario() { return sLivesMario; }
    static void ResetLivesMario() { sLivesMario = sBasicNumberLivesMario; }
    static void ResetLevelMario() { sLevelMario = LevelMario::SmallMario; }

    float GetHowLongChangingLevel() const { return mChangingLevelTime; }
    bool IsCrouching() const { return mIsCrouching; }
    bool IsInRightReversal() const { return mIsRightDirReversal; }

    /// Setters
    void SetShield();

private:

    class Bullet : public PhysicalObject {
        float mRange;

        const float mLengthRange = 600;
        const int mDamageValue = 100;

        void UpdateForCollisionWithBlock(KindCollision pCollision, Block* pBlock) override;

    public:

        Bullet(sf::Vector2f pPos, bool pIsRight, bool pIsFast = false);
        ~Bullet() {}

        void Update() override;

        void ActOnMe(KindAction pAction) override;
        void ActOnObject(GameObject* pObject, KindCollision pCollision) override;

    };

    enum MyKindsAnimations {
        LeftLevelUp,
        RightLevelUp,
        LeftLevelDown,
        RightLevelDown,
        LeftShoot,
        RightShoot,
        LeftCrouch,
        RightCrouch
    };

    static short sLivesMario;
    static const short sBasicNumberLivesMario;

    static LevelMario sLevelMario;

    bool mFlashingMario = false;
    bool mVisibleWhileFlashing = false;
    bool mIsShooting = false;
    bool mIsCrouching = false;
    bool mCurbing = false;
    bool mStop = false;

    const float mFlashingIterativity = 0.1f;
    const float mChangingLevelTime = 1.5f;
    const float mFlashingTime = 2.5f;
    const float mShootingIterativity = 0.4f;

    const float mMaxSpeed = 5.0f;
    const float mMovementAnimationSpeed = 0.9f;

    float mShootingIterativityTimer;
    float mFlashingIterativityTimer;
    float mChangingLevelTimer;
    float mFlashingTimer;

    float mHorizontalVelocity = 0.0f;

    std::map<EventHandler::DecisionsPlayer, bool>& mEnabledDecisions;
    std::map<EventHandler::DecisionsPlayer, bool>& mDeactivateDecisions;

    ///----METHODS
    inline void CreateBigMarioAnimations();
    inline void CreateSmallMarioAnimations();
    inline bool Flashing();
    inline void CollisionWithBoundsMap();
    inline void HandleMarioControl();
    inline void Shoot();
    inline void Crouch();
    inline void AnimationUpdate();
    inline void Remove();
    inline void SetFlashing(float pHowLong);
    inline void CorrectPositionWhenChangingOnBigMario(const sf::FloatRect& pPrevBounds);

    inline void Run();
    inline void Curbing();

    void Falling() override;
    void Jump() override;
    void UpdateForCollisionWithBlock(KindCollision pCollision, Block* pBlock) override;
};
