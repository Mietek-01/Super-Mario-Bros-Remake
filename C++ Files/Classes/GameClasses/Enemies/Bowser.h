#pragma once
#include "../PhysicalObject.h"
#include "../Blocks/Block.h"
#include "../Shield.h"

using namespace std;

class CBowser:public CPhysicaltObject,public CShield
{
    class CBarse:public CStaticBlock
    {
        const bool m_go_up;
        const float m_move_speed=4.0f;

        bool m_active;

    public:

        CBarse(sf::Vector2f,bool,bool);
        ~CBarse(){}

        void update()override;
        void active(){m_active=true;}
    };

    ///------
    class CPrincess:public CGameObject
    {
        class CBotMario:public CPhysicaltObject
        {
            bool m_iam_blocking=false;
            bool m_change_animation=false;

            void updateForCollisionWithBlock(KindCollision, CBlock*)override;

        public:

            CBotMario(sf::Vector2f);
            ~CBotMario(){}

            void actOnMe(KindAction)override{}
            void actOnObject(CGameObject*,KindCollision)override{}

            void update()override;

            void falling()override;

            void stop();

        }* m_bot_mario=nullptr;

        const string m_message_to_mario="THANKS MARIO !!!";
        const float m_when_show_message=1.0f;
        const float m_when_state_win_game = 3.0f;
        const float m_stop_mario_position=50;
        
        float m_timer;
        bool m_bot_mario_stoped=false;

        unique_ptr<sf::Text>m_text;

    public:

    CPrincess(sf::Vector2f);
    ~CPrincess(){}

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&)override;

    void actOnMe(KindAction)override{}
    void actOnObject(CGameObject*,KindCollision)override{}

    };

    ///-----
    class CCreatorEnemies
    {
        float m_when_create_enemie=0;
        int m_last_rand_tile=-1;

        const int m_number_tiles;
        const int m_first_tile_position;
        const float m_creating_speed=0.5f;

        inline void setWhenNextEnemy();

    public:

        CCreatorEnemies(int,int);
        CPhysicaltObject *getCreatedEnemy();

    };

///------------------------------------///

    enum MyKindsAnimations
    {
        R_CHANGING_TO_REVERSAL,
        L_CHANGING_TO_REVERSAL,

        R_BASIC_FIRE_BALL_ATACK,
        L_BASIC_FIRE_BALL_ATACK,

        R_SPECIAL_CIRCLE_FIRE_BALL_ATACK,
        L_SPECIAL_CIRCLE_FIRE_BALL_ATACK,

        R_SPECIAL_SIN_FIRE_BALL_ATACK,
        L_SPECIAL_SIN_FIRE_BALL_ATACK,
    };

    ///---
    enum class KindsAtacks
    {
        RANDOM=-1,
        LEAP_ON_MARIO,
        SPECIAL_CIRCLE_FIREBALL_ATACK,
        SPECIAL_SIN_FIREBALL_ATACK,
        BASIC_FIREBALL_ATACK,
        CHARGE,
        ENEMIES_RAID
    };

    ///----
    enum class KindBullet
    {
        BASIC,
        SIN_MOVEMENT,
        CIRCLE_MOVEMENT
    };

    ///ATRYBUTY

    static bool s_instance;

    void (CBowser::*m_atack)()=nullptr;

    CBarse * m_barse_to_princes=nullptr;
    unique_ptr<CCreatorEnemies> m_creator_enemies;

    bool m_changing_reversal=false;
    bool m_enable=false;
    bool m_shoot=false;
    bool m_attack_interrupted=false;
    bool m_block_changing_reversal=false;

    const sf::Vector2f m_princess_position;
    
    const float m_my_scal=CGameObject::m_scale_to_tile*1.3f;
    const float m_where_barse;
    const float m_shooting_speed=5.0f;
    const float m_basic_speed=1.0f;
    const float m_raid_lenght=350;
    const float m_left_barse_pos;
    const float m_right_barse_pos;
    const float m_enemies_raid_lenght_attack=4.0f;
    const float m_fireball_sin_lenght_attack=6.0f;

    const int m_damage_value=250;

    float m_timer=0.0f;
    float m_when_atack;

    /// ATRYBUTY DO OBSLUGI SKOKU
    sf::Vector2f m_my_pos_before_jump;
    float m_second_zero_place;
    float m_A_coefficient;
    float m_X_coordinate;

    ///---METODY
    inline void createAnimations();
    inline void changeReversal();
    inline void setTrapForMario();

    inline void setAtack(KindsAtacks= KindsAtacks::RANDOM);
    inline void setWhenAtack();
    inline void shoot(const vector<int>,int,KindBullet);

    void makeJump()override;

    ///---METODY ATAKU
    void basicFireBallAtack();
    void specialCircleFireBallAtack();
    void specialSinFireBallAtack();
    void leapOnMario();
    void charge();
    void enemiesRaid();

public:

    CBowser(sf::Vector2f);
    ~CBowser();

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&)override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;
};

