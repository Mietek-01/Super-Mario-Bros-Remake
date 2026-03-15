#pragma once
#include "Block.h"
#include "../PhysicalObject.h"

#include <memory>

class MysteryBox : public DynamicBlock
{

    class Mushroom : public PhysicalObject
    {
        void UpdateForCollisionWithBlock(KindCollision, Block*) override;

    public:

        Mushroom(sf::Vector2f);
        ~Mushroom() {}

        void ActOnObject(GameObject*, KindCollision) override;
        void ActOnMe(KindAction) override;
    };

    class SpecialFlower : public GameObject
    {
    public:

        SpecialFlower(sf::Vector2f);
        ~SpecialFlower() {}

        void ActOnObject(GameObject*, KindCollision) override;
        void ActOnMe(KindAction) override;

        void Update() override {};
    };

public:

    enum class MyItem
    {
        Coin,
        Mushroom,
        Flower
    };

    MysteryBox(sf::Vector2f, MyItem);
    ~MysteryBox() {}

    void Update() override;
    void Draw(const std::unique_ptr<sf::RenderWindow>&) override;
    void ActOnMe(KindAction) override;

    static void SetStaticAnimation();
    static void UpdateStaticAnimation() { sPrivateAnimator.Update(); }

private:

    const MyItem mRandItem;

    bool mToRand = true;
    bool mCreatingItem = false;
    bool mDrawQuestionMark = true;

    const float mLeavingSpeed = -0.2f;

    GameObject* mMyObj = nullptr;

    static Animations sPrivateAnimator;
};
