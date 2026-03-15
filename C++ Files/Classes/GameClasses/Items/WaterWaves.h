#pragma once
#include "../GameObject.h"

#include <memory>

class WaterWaves : public GameObject {
    static Animations sStaticAnimator;

public:

    WaterWaves(sf::Vector2f pPos);
    ~WaterWaves() {}

    void Update() override {}
    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override {}
    void ActOnMe(KindAction pAction) override {}

    static void SetStaticAnimation();
    static void UpdateStaticAnimation() { sStaticAnimator.Update(); }
};
