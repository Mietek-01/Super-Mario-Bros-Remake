#pragma once
#include "../Blocks/Block.h"

#include <vector>

class Gun : public StaticBlock {
    class GunBullet : public GameObject {
        const float mSpeed = 2.0f;
        const bool mRightDir;

        int mRange = 1500;

        bool mTheEndRange = false;

    public:

        GunBullet(sf::Vector2f pPos, bool pRightDir);
        ~GunBullet() {}

        void Update() override;

        void ActOnMe(KindAction pAction) override;
        void ActOnObject(GameObject* pObject, KindCollision pCollision) override;

        bool IsVisible() const override { return true; }
    };

    float mWhenShoot;

    inline void SetWhenShoot();

public:

    enum class KindGun {
        Small = 32,
        Big = 64
    };

    Gun(sf::Vector2f pPos, KindGun pKindGun);
    ~Gun() {}

    void Update() override;
};
