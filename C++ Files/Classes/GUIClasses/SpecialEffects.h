#pragma once
#include <functional>
#include "GUIObject.h"
#include "Animator.h"

using namespace std;

class CSpecialEffects:public CGuiObject
{
    static bool s_deactivation_death_animation;
    bool m_rotate=false;

    sf::Vector2f m_current_pos;
    sf::Vector2f m_force;

    unique_ptr<CGuiObject> m_death_animation;
    unique_ptr<CAnimator> m_animator;

    std::function<bool(unique_ptr<CAnimator>&)> m_special_update;

public:

    enum KindUpdate
    {
        JUMP,
        STANDIG,
        CONST_MOVE,
        ONE_LOOP_ANIMATION

    }const m_kind_update;

    CSpecialEffects(CAnimator*,KindUpdate,float,sf::Vector2f=sf::Vector2f());
    CSpecialEffects(CAnimator*,std::function<bool(unique_ptr<CAnimator>&)>);

    ~CSpecialEffects();

    void draw(unique_ptr<sf::RenderWindow>& window)override {m_animator->draw(window);}
    void update() override;

    void activeRotate(){m_rotate=true;}
    void setDeathAnimation(CSpecialEffects* obj){m_death_animation.reset(obj);}
    static void deactivateDeathAnimations(bool value){s_deactivation_death_animation=value;}
};
