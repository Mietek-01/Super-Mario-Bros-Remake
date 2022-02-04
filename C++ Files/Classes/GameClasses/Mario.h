#pragma once
#include "PhysicalObject.h"
#include "../EventHandler.h"
#include "Shield.h"

using namespace std;

class CMario: public CPhysicaltObject,public CShield
{

public:

    enum class LevelMario
    {
        SMALL_MARIO,
        BIG_MARIO
    };

    CMario(sf::Vector2f);
    ~CMario();

    void draw(const unique_ptr<sf::RenderWindow>&) override;
    void update() override;

    void actOnObject(CGameObject*,KindCollision)override{}
    void actOnMe(KindAction)override;

    void changingLvl();
    void hop(float)override;

    static void createDeathAnimation(const sf::Vector2f&);

    /// GETTERY
    static CMario::LevelMario getLevelMario(){return s_level_mario;}
    static short getLivesMario() { return s_lives_mario; }
    static void resetLivesMario() { s_lives_mario = s_basic_number_lives_mario; }
    static void resetLvlMario() { s_level_mario = LevelMario::SMALL_MARIO; }
    
    float getHowLongChangingLvl()const {return m_changing_lvl_time;}
    bool iAmCrouching()const {return m_iam_crouching;}
    bool isInRightReversal()const {return m_right_dir_reversal;}

    /// SETTERY
    void setShield();

private:

    class CBullet:public CPhysicaltObject
    {
        float m_range;

        const float m_lenght_range=600;
        const int m_damage_value=100;

        void updateForCollisionWithBlock(KindCollision,CBlock*)override;

    public:

        CBullet(sf::Vector2f,bool,bool=false);
        ~CBullet(){}

        void update()override;

        void actOnMe(KindAction)override;
        void actOnObject(CGameObject*,KindCollision )override;

    };

    enum MyKindsAnimations
    {
        L_LVL_UP,
        R_LVL_UP,
        L_LVL_DOWN,
        R_LVL_DOWN,
        L_SHOOT,
        R_SHOOT,
        L_CROUCH,
        R_CROUCH
    };

    static short s_lives_mario;
    static const short s_basic_number_lives_mario;

    static LevelMario s_level_mario;

    bool m_flashing_mario=false;
    bool m_visible_while_flashing=false;
    bool m_iam_shooting=false;
    bool m_iam_crouching=false;
    bool m_curbing=false;
    bool m_stop=false;

    const float m_flashing_iterativity=0.1f;
    const float m_changing_lvl_time=1.5f;
    const float m_flashing_time=2.5f;
    const float m_shooting_iterativity=0.4f;

    const float m_max_speed=5.0f;
    const float m_movement_animation_speed=0.9f;

    float m_shooting_iterativity_timer;
    float m_flashing_iterativity_timer;
    float m_changeing_lvl_timer;
    float m_flashing_timer;
    
    float m_horizontal_velocity=0.0f;

    std::map<CEventHandler::DecisionsPlayer,bool> &m_enabled_decisions;
    std::map<CEventHandler::DecisionsPlayer,bool> &m_deactivate_decisions;

    ///----METHODS
    inline void createBigMarioAnimations();
    inline void createSmalMarioAnimations();
    inline bool flashing();
    inline void collisionWithBoundsMap();
    inline void handleMarioControll();
    inline void shoot();
    inline void crouch();
    inline void animationUpdtate();
    inline void remove();
    inline void setFlashing(float);
    inline void corectPositionWhenChangingOnBigMario(const sf::FloatRect&);

    inline void run();
    inline void curbing();

    void falling() override;
    void jump()override;
    void updateForCollisionWithBlock(KindCollision,CBlock*)override;
};

