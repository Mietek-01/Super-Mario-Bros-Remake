#pragma once
#include "../PhysicalObject.h"
#include "ArmedTurtle.h"
#include "Bowser.h"

using namespace std;

class CGoombas:public CPhysicaltObject
{
    const int m_damage_value=100;/// POTRZEBNE GDY W POZIOMIE Z BOWSEREM ZADAJE OBRAZENIA MARIU
    const bool m_created_by_bowser;

    void updateForCollisionWithBlock(KindCollision ,CBlock*)override;

public:

    CGoombas(sf::Vector2f,KindMovement=CPhysicaltObject::KindMovement::LEFT_RUN,bool =false);
    ~CGoombas(){}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

};

///----------
class CTurtle:public CPhysicaltObject
{
    const bool m_created_by_flying_turtle;
    bool m_close_to_fall=false;
    sf::FloatRect m_future_position;

    inline void dontFall();

public:

    CTurtle(sf::Vector2f,bool=false,KindMovement=KindMovement::LEFT_RUN);
    ~CTurtle(){}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

    void update()override;
};

///--------
class CRedTurtle:public CPhysicaltObject
{
    const int m_damage_value=150;
    const bool m_created_by_bowser;

    void updateForCollisionWithBlock(KindCollision ,CBlock*)override;

public:

    CRedTurtle(sf::Vector2f,KindMovement=CPhysicaltObject::KindMovement::LEFT_RUN,bool=false);
    ~CRedTurtle(){}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

};

///------
class CArmoredTurtle:public CPhysicaltObject
{

public:

    CArmoredTurtle(sf::Vector2f);
    ~CArmoredTurtle(){}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

};

///---------
class CFlyingTurtle:public CPhysicaltObject
{
    static const float s_my_gravitation;
    void makeJump()override;

    void updateForCollisionWithBlock(KindCollision ,CBlock*)override;

public:

    CFlyingTurtle(sf::Vector2f);
    ~CFlyingTurtle(){}

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

};

///-----
class CCreatorRedTurltes:public CGameObject
{
    class CRedTurtleShall:public CPhysicaltObject
    {
        const float m_basic_y_pos;
        void updateForCollisionWithBlock(KindCollision,CBlock*)override;

    public:
        CRedTurtleShall(sf::Vector2f,bool);
        ~CRedTurtleShall(){}

        void actOnObject(CGameObject*,KindCollision)override;
        void actOnMe(KindAction)override{};

        bool isVisible()const override {return true;}
    };

    bool m_right_dir=true;
    bool m_enable=false;
    bool m_curbing=false;
    bool m_set_changing_dir_pos=true;
    bool m_leave_map=false;
    bool m_loading_shoot=false;

    const float m_leave_position;
    const sf::Vector2f m_acitivation_position;

    const float m_value_acceleration=0.07f;
    const float m_max_speed=4.5f;

    const float m_iterativity_loading_shoot=1.5f;
    const float m_loading_shoot_time=0.3f;

    const int m_range=350;
    const int m_when_curbing=150;

    float m_change_dir_position;
    float m_speed=0.0f;
    float m_double_speed=1.0f;
    float m_when_load_shoot;
    float m_when_shoot;

    inline void setChangeDirPosition();
    inline void changeDirection();
    inline void move();
    inline void shoot();

    inline void setWhenLoadShoot();
    inline void setWhenShoot();

public:

    CCreatorRedTurltes(sf::Vector2f);
    ~CCreatorRedTurltes(){}

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&) override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;

    bool isVisible()const override {return true;}
};
