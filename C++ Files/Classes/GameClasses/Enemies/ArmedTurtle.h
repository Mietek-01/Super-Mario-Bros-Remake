#pragma once
#include "../PhysicalObject.h"

using namespace std;

class CArmedTurtle:public CPhysicaltObject
{
    class CHammer:public CGameObject
    {
        int m_value_rotate=-8;

        bool m_enabled=false;
        bool m_right_dir_reversal;

        float m_horizontal_acceleration=-3.0f;
        float m_force=-700.0f;

    public:

        CHammer(sf::Vector2f,bool);
        ~CHammer(){}

        void actOnObject(CGameObject*,KindCollision)override;
        void actOnMe(KindAction)override{}

        void update()override;

        void setEnabled(bool);
        void changeReversal(sf::Vector2f);

    }* m_my_hammer=nullptr;

    enum MyKindsAnimations
    {
        R_MOVE_WITH_HAMMER,
        L_MOVE_WITH_HAMMER,
        R_MOVE_THROWING_HAMMER,
        L_MOVE_THROWING_HAMMER,

    };

    static const float s_length_road;
    static const float s_correction_pos_hammer;
    static const float s_force_hop;

    const int m_after_what_time_jump;
    const int m_after_what_time_throw_hammer;

    float m_when_jump;
    float m_when_throw_hammer;
    float m_where_change_direction;

    int m_for_which_block_bottom_collision;/// POZWALA NA PRZESKAKIWANIE PRZEZ BLOCKI

    bool m_can_jump=false;
    bool m_down_jump=false;

    /// METHODS
    inline void changeLookingDirection();
    inline void changeMovementDirection();
    inline bool randKindJump();
    inline void createHammer();
    inline void setWhenJump();

    void jump()override;
    void updateForCollisionWithBlock(KindCollision, CBlock*)override;

public:

    CArmedTurtle(sf::Vector2f);
    ~CArmedTurtle(){}

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&)override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;
};

