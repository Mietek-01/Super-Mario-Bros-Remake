#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <map>
#include <memory>
#include <vector>

#include "../GUIClasses/Animator.h"
#include "../MarioGame.h"
#include "../Scens/Scen.h"
#include "../GUIClasses/SpecialEffects.h"
#include "../GUIClasses/GUI.h"

using namespace std;

class CGameObject
{

public:

    enum class KindCollision
    {
        BOTTOM,
        LEFT_SIDE,
        RIGHT_SIDE,
        TOP,
        NONE,
    };

    enum class Parentage
    {
       BLOCK,
       ENEMY,
       ITEM,
       MARIO
    };

    enum class KindAction
    {
        UNDER_MAP,
        HIT,
        CRUMPLED,
        HOP,
        LVL_UP,
        DESTROYED
    };

    const static float m_scale_to_tile;

    CGameObject(CAnimator*,Parentage,const sf::Vector2f&,int=100);
    virtual ~CGameObject(){}

    /// METHODS
    virtual void draw(const unique_ptr<sf::RenderWindow>&);
    virtual void update()=0;

    virtual void actOnObject(CGameObject*,KindCollision)=0;
    virtual void actOnMe(KindAction)=0;

    virtual bool isVisible()const{return m_visible;}

    KindCollision howCollision(const CGameObject&)const;
    void movePosition(sf::Vector2f);
    void corectObjectPositionOnMe(CGameObject&,KindCollision)const;

    /// GETTERS
    int getValuePoints()const {return m_my_value_points;}
    Parentage getParentage()const {return m_parentage;}
    bool iamDead()const {return m_dead;}

    sf::Vector2f getCurrentPosition()const{return m_current_position;}
    sf::Vector2f getPreviousPosition()const{return m_previous_position;}

    sf::FloatRect getBounds()const {return m_animator->getGlobalBounds();}
    sf::FloatRect getPreviousBounds()const;
    sf::Vector2f getSize()const {return sf::Vector2f(m_animator->getGlobalBounds().width,m_animator->getGlobalBounds().height);}

    /// SETTERS
    void setPosition(sf::Vector2f pos){m_current_position=pos;m_animator->setPosition(pos);}
    void setPreviousPosition(sf::Vector2f pos){m_previous_position=pos;}
    void setVisible(bool visbl){m_visible=visbl;}

    /// STATIC METHOD
    static vector<CGameObject* >& getRemovedObjects(){return s_removed_objects;}
    static vector<CGameObject* >& getNewObjects(){return s_new_objects;}

protected:

    bool m_visible=false;
    bool m_dead=false;

    const int m_my_value_points;
    const Parentage m_parentage;

    sf::Vector2f m_previous_position,m_current_position;
    unique_ptr<CAnimator> m_animator;
    CAnimations * m_animations;

    void isUnderMap();

    void createBeatsObjSpecialEfect(float=-600.0f);
    void createPoints()const;

    static void addNewObject(CGameObject* obj);
    static void removeObject(CGameObject* obj);

private:

    static vector<CGameObject* > s_removed_objects;
    static vector<CGameObject* > s_new_objects;

    KindCollision specialCheckingHowCollision(const CGameObject&)const ;

};
