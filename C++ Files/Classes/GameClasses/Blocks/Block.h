#pragma once
#include "../GameObject.h"

#include <string>

class Block : public GameObject {
public:
    Block(const sf::IntRect&, const sf::Vector2f&, const std::string);
    virtual ~Block() {}

    virtual void ActOnObject(GameObject*, KindCollision) override;

    [[nodiscard]] bool IsHit() const { return mIsHit; }

protected:
    bool mIsHit = false;
};

///-------------///
class DynamicBlock : public Block {
    static const float sBlockGravitation;

    const float mBasicYPosition;
    const float mJumpForce = -300.0f;

public:
    DynamicBlock(sf::IntRect, sf::Vector2f, std::string = "Tiles");
    virtual ~DynamicBlock() {}

    virtual void Update() override { if (mIsHit) MakeJump(); }
    virtual void ActOnMe(KindAction) override;

protected:
    float mForce;

    void MakeJump();
    void CreateRemoveBlock();
};

///-----------------------------///
class StaticBlock : public Block {
public:
    StaticBlock(sf::IntRect pBounds, sf::Vector2f pPos, std::string pName = "Tiles")
        : Block(pBounds, pPos, pName) {}

    virtual ~StaticBlock() {}

    virtual void Update() override {}
    void ActOnMe(KindAction) override {}
};
