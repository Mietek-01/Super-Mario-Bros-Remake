#pragma once
#include "../GameObject.h"

#include <memory>

class Coin : public GameObject {
    static Animations sStaticAnimator;
    static int sCountGatheredCoins;

public:

    Coin(sf::Vector2f pPos);
    ~Coin() {}

    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;
    void Update() override {}

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;
    void ActOnMe(KindAction pAction) override;

    static void SetStaticAnimation();
    static void UpdateStaticAnimation() { sStaticAnimator.Update(); }

    static Animations* CreateFallingApartCoinAnimation();
    static Animations* CreateFlashingCoinAnimation();
    static void ResetCountGatheredCoins() { sCountGatheredCoins = 0; }
};
