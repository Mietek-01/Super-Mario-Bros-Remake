#pragma once
#include "Block.h"

#include <memory>

class Pipe : public StaticBlock
{
    class Flower : public GameObject
    {
        Pipe* mMyPipe;

        const float mPosGnawing;
        const float mHowLongHiding;
        float mWhenMoveUpper;
        float mTimeGnawing;

        bool mEnabled = false;

        static const float sHowLongGnawing;
        static const float sSpeed;

        enum MyKindsAnimations
        {
            Gnawing,
            MoveUp,
            MoveDown,

        };

        enum class State
        {
            MoveUp,
            MoveDown,
            Hiding,
            Gnawing
        } mState = State::Hiding;

        inline void SetWhenMoveUpper();

    public:

        Flower(Pipe*, sf::Vector2f);
        ~Flower() { mMyPipe->mMyFlower = nullptr; }

        void Update() override;
        void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

        void ActOnMe(KindAction) override;
        void ActOnObject(GameObject*, KindCollision) override;

        void Reset();
    };

    friend class Flower;

    Flower* mMyFlower = nullptr;

public:

    enum class KindPipe
    {
        Big = 128,
        Small = 64,
    };

    Pipe(sf::Vector2f, KindPipe);
    virtual ~Pipe() { ; }

    void ResetFlower();
};
