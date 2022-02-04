#pragma once
#include "../GameObject.h"

using namespace std;

class CLavaWaves: public CGameObject
{
    static CAnimations s_static_animator;

public:

    CLavaWaves(sf::Vector2f);
    ~CLavaWaves(){}

    void update()override{};
    void draw(const unique_ptr<sf::RenderWindow>&)override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override{}

    static void setStaticAnimation();
    static void updateStaticAnimation(){s_static_animator.update();}
};




