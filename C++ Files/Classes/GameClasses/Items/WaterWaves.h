#pragma once
#include "../GameObject.h"

using namespace std;

class CWaterWaves: public CGameObject
{
    static CAnimations s_static_animator;

public:

    CWaterWaves(sf::Vector2f);
    ~CWaterWaves(){}

    void update()override{};
    void draw(const unique_ptr<sf::RenderWindow>&)override;

    void actOnObject(CGameObject* obj,KindCollision how_collision)override{}
    void actOnMe(KindAction kind_action)override{}

    static void setStaticAnimation();
    static void updateStaticAnimation(){s_static_animator.update();}
};




