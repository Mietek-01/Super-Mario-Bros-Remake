#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <map>
#include <memory>
#include <vector>

#include "../GUIClasses/Animator.h"
#include "../MarioGame.h"
#include "../Scens/Scen.h"
#include "../GUIClasses/SpecialEffects.h"
#include "../GUIClasses/GUI.h"

class GameObject {

public:

    enum class KindCollision {
        Bottom,
        LeftSide,
        RightSide,
        Top,
        None,
    };

    enum class Parentage {
        Block,
        Enemy,
        Item,
        Mario
    };

    enum class KindAction {
        UnderMap,
        Hit,
        Crumpled,
        Hop,
        LevelUp,
        Destroyed
    };

    static const float kScaleToTile;

    GameObject(Animator* pAnimator, Parentage pParentage, const sf::Vector2f& pPosition, int pPoints = 100);
    virtual ~GameObject() {}

    /// Methods
    virtual void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow);
    virtual void Update() = 0;

    virtual void ActOnObject(GameObject* pObject, KindCollision pCollision) = 0;
    virtual void ActOnMe(KindAction pAction) = 0;

    virtual bool IsVisible() const { return mIsVisible; }

    KindCollision HowCollision(const GameObject& pObject) const;
    void MovePosition(sf::Vector2f pValue);
    void CorrectObjectPositionOnMe(GameObject& pObject, KindCollision pCollision) const;

    /// Getters
    [[nodiscard]] int GetValuePoints() const { return mValuePoints; }
    [[nodiscard]] Parentage GetParentage() const { return mParentage; }
    [[nodiscard]] bool IsDead() const { return mIsDead; }

    [[nodiscard]] sf::Vector2f GetCurrentPosition() const { return mCurrentPosition; }
    [[nodiscard]] sf::Vector2f GetPreviousPosition() const { return mPreviousPosition; }

    [[nodiscard]] sf::FloatRect GetBounds() const { return mAnimator->getGlobalBounds(); }
    [[nodiscard]] sf::FloatRect GetPreviousBounds() const;
    [[nodiscard]] sf::Vector2f GetSize() const { return sf::Vector2f(mAnimator->getGlobalBounds().width, mAnimator->getGlobalBounds().height); }

    /// Setters
    void SetPosition(sf::Vector2f pPosition) { mCurrentPosition = pPosition; mAnimator->setPosition(pPosition); }
    void SetPreviousPosition(sf::Vector2f pPosition) { mPreviousPosition = pPosition; }
    void SetVisible(bool pVisible) { mIsVisible = pVisible; }

    /// Static Methods
    [[nodiscard]] static std::vector<GameObject*>& GetRemovedObjects() { return sRemovedObjects; }
    [[nodiscard]] static std::vector<GameObject*>& GetNewObjects() { return sNewObjects; }

protected:

    bool mIsVisible = false;
    bool mIsDead = false;

    const int mValuePoints;
    const Parentage mParentage;

    sf::Vector2f mPreviousPosition, mCurrentPosition;
    std::unique_ptr<Animator> mAnimator;
    Animations* mAnimations;

    void CheckUnderMap();

    void CreateBeatObjectEffect(float pHopForce = -600.0f);
    void CreatePoints() const;

    static void AddNewObject(GameObject* pObject);
    static void RemoveObject(GameObject* pObject);

private:

    static std::vector<GameObject*> sRemovedObjects;
    static std::vector<GameObject*> sNewObjects;

    KindCollision SpecialCheckingHowCollision(const GameObject& pObject) const;

};
