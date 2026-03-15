#pragma once
#include "Block.h"

class FieryBlock : public StaticBlock
{
    class FieryLine : public GameObject
    {
        const float mSpeed = 1.8f;

    public:

        enum class Direction
        {
            Left = 232,
            Right = 202

        } const mDir;

        FieryLine(sf::Vector2f, Direction);
        ~FieryLine() {}

        void Update() override;

        void ActOnMe(KindAction) override {};
        void ActOnObject(GameObject*, KindCollision) override;

        bool IsVisible() const override { return true; }

    };

public:

    FieryBlock(sf::Vector2f, const bool);
    ~FieryBlock() {}

};
