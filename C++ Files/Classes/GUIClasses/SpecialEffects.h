#pragma once
#include <functional>
#include <memory>
#include "GUIObject.h"
#include "Animator.h"

class SpecialEffects : public GuiObject
{
    static bool sIsDeactivationDeathAnimation;
    bool mIsRotate = false;

    sf::Vector2f mCurrentPos;
    sf::Vector2f mForce;

    std::unique_ptr<GuiObject> mDeathAnimation;
    std::unique_ptr<Animator> mAnimator;

    std::function<bool(std::unique_ptr<Animator>&)> mSpecialUpdate;

public:

    enum KindUpdate
    {
        Jump,
        Standing,
        ConstMove,
        OneLoopAnimation

    } const mKindUpdate;

    SpecialEffects(Animator* pAnimator, KindUpdate pKindUpdate, float pWhenRemove, sf::Vector2f pForce = sf::Vector2f());
    SpecialEffects(Animator* pAnimator, std::function<bool(std::unique_ptr<Animator>&)> pSpecialUpdate);

    ~SpecialEffects();

    void Draw(std::unique_ptr<sf::RenderWindow>& pWindow) override { mAnimator->Draw(pWindow); }
    void Update() override;

    void ActiveRotate() { mIsRotate = true; }
    void SetDeathAnimation(SpecialEffects* pObj) { mDeathAnimation.reset(pObj); }
    static void DeactivateDeathAnimations(bool pValue) { sIsDeactivationDeathAnimation = pValue; }
};
