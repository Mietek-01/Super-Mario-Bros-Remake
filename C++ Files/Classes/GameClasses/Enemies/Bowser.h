#pragma once
#include "../PhysicalObject.h"
#include "../Blocks/Block.h"
#include "../Shield.h"

#include <memory>
#include <vector>

class Bowser : public PhysicalObject, public Shield {
    class Barse : public StaticBlock {
        const bool mGoUp;
        const float mMoveSpeed = 4.0f;

        bool mActive;

    public:

        Barse(sf::Vector2f, bool, bool);
        ~Barse() {}

        void Update() override;
        void Activate() { mActive = true; }
    };

    ///------
    class Princess : public GameObject {
        class BotMario : public PhysicalObject {
            bool mIsBlocking = false;
            bool mChangeAnimation = false;

            void UpdateForCollisionWithBlock(KindCollision, Block*) override;

        public:

            BotMario(sf::Vector2f);
            ~BotMario() {}

            void ActOnMe(KindAction) override {}
            void ActOnObject(GameObject*, KindCollision) override {}

            void Update() override;

            void Falling() override;

            void Stop();

        }* mBotMario = nullptr;

        const std::string mMessageToMario = "THANKS MARIO !!!";
        const float mWhenShowMessage = 1.0f;
        const float mWhenStateWinGame = 3.0f;
        const float mStopMarioPosition = 50;

        float mTimer;
        bool mBotMarioStopped = false;

        std::unique_ptr<sf::Text> mText;

    public:

        Princess(sf::Vector2f);
        ~Princess() {}

        void Update() override;
        void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

        void ActOnMe(KindAction) override {}
        void ActOnObject(GameObject*, KindCollision) override {}
    };

    ///-----
    class CreatorEnemies {
        float mWhenCreateEnemy = 0;
        int mLastRandTile = -1;

        const int mNumberTiles;
        const int mFirstTilePosition;
        const float mCreatingSpeed = 0.5f;

        inline void SetWhenNextEnemy();

    public:

        CreatorEnemies(int, int);
        PhysicalObject* GetCreatedEnemy();
    };

///------------------------------------///

    enum MyKindsAnimations {
        R_CHANGING_TO_REVERSAL,
        L_CHANGING_TO_REVERSAL,

        R_BASIC_FIRE_BALL_ATTACK,
        L_BASIC_FIRE_BALL_ATTACK,

        R_SPECIAL_CIRCLE_FIRE_BALL_ATTACK,
        L_SPECIAL_CIRCLE_FIRE_BALL_ATTACK,

        R_SPECIAL_SIN_FIRE_BALL_ATTACK,
        L_SPECIAL_SIN_FIRE_BALL_ATTACK,
    };

    ///---
    enum class KindsAttacks {
        RANDOM = -1,
        LEAP_ON_MARIO,
        SPECIAL_CIRCLE_FIREBALL_ATTACK,
        SPECIAL_SIN_FIREBALL_ATTACK,
        BASIC_FIREBALL_ATTACK,
        CHARGE,
        ENEMIES_RAID
    };

    ///----
    enum class KindBullet {
        BASIC,
        SIN_MOVEMENT,
        CIRCLE_MOVEMENT
    };

    ///ATTRIBUTES

    static bool sInstance;

    void (Bowser::*mAttack)() = nullptr;

    Barse* mBarseToPrincess = nullptr;
    std::unique_ptr<CreatorEnemies> mCreatorEnemies;

    bool mChangingReversal = false;
    bool mEnable = false;
    bool mShoot = false;
    bool mAttackInterrupted = false;
    bool mBlockChangingReversal = false;

    const sf::Vector2f mPrincessPosition;

    const float mMyScale = GameObject::kScaleToTile * 1.3f;
    const float mWhereBarse;
    const float mShootingSpeed = 5.0f;
    const float mBasicSpeed = 1.0f;
    const float mRaidLength = 350;
    const float mLeftBarsePos;
    const float mRightBarsePos;
    const float mEnemiesRaidLengthAttack = 4.0f;
    const float mFireballSinLengthAttack = 6.0f;

    const int mDamageValue = 250;

    float mTimer = 0.0f;
    float mWhenAttack;

    /// Attributes for jump handling
    sf::Vector2f mMyPosBeforeJump;
    float mSecondZeroPlace;
    float mACoefficient;
    float mXCoordinate;

    ///---Methods
    inline void CreateAnimations();
    inline void ChangeReversal();
    inline void SetTrapForMario();

    inline void SetAttack(KindsAttacks = KindsAttacks::RANDOM);
    inline void SetWhenAttack();
    inline void Shoot(const std::vector<int>, int, KindBullet);

    void MakeJump() override;

    ///---Attack methods
    void BasicFireBallAttack();
    void SpecialCircleFireBallAttack();
    void SpecialSinFireBallAttack();
    void LeapOnMario();
    void Charge();
    void EnemiesRaid();

public:

    Bowser(sf::Vector2f);
    ~Bowser();

    void Update() override;
    void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

    void ActOnObject(GameObject*, KindCollision) override;
    void ActOnMe(KindAction) override;
};
