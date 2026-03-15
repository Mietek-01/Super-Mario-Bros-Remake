#pragma once
#include "../GameObject.h"

#include <memory>

class LavaWaves : public GameObject {
    static Animations sStaticAnimator;

public:

    LavaWaves(sf::Vector2f pPos);
    ~LavaWaves() {}

    void Update() override {}
    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;

    void ActOnObject(GameObject* pObject, KindCollision pCollision) override;
    void ActOnMe(KindAction pAction) override {}

    static void SetStaticAnimation();
    static void UpdateStaticAnimation() { sStaticAnimator.Update(); }
};
