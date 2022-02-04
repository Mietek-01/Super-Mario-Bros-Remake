#pragma once
#include "../GameObject.h"

using namespace std;

class CCoin: public CGameObject
{
    static CAnimations s_static_animator;
    static int s_count_gathered_coins;

public:

    CCoin(sf::Vector2f);
    ~CCoin(){}

    void draw(const unique_ptr<sf::RenderWindow>&) override;
    void update()override{}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

    static void setStaticAnimation();
    static void updateStaticAnimation(){s_static_animator.update();}

    static CAnimations* creatFallingApartCoinAnimation();
    static CAnimations* creatFlashingCoinAnimation();
    static void resetCountGatheredCoins(){s_count_gathered_coins=0;}
};


