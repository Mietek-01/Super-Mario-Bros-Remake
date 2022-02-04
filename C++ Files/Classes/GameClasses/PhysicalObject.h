#pragma once
#include "GameObject.h"
#include "Blocks/Block.h"

using namespace std;

class CPhysicaltObject:public CGameObject
{
protected:

    enum class KindMovement
    {
        LEFT_RUN,
        RIGHT_RUN,
        STANDING
    }m_kind_movement;

    const float m_falling_force=250.0f;
    float m_force=0.0f;
    float m_value_acceleration=0.1f;
    float m_jump_force=-900.0f;

    bool m_right_dir_reversal;
    bool m_jump=false;
    bool m_in_bottom_collision=true;

    /// METHODS
    virtual void updateForCollisionWithBlock(KindCollision,CBlock*);
    virtual void jump();
    virtual void falling();
    virtual void makeJump();

    void move();

public:

     const static float s_gravitation;
     const static float s_correction_to_bottom_collision;

     CPhysicaltObject(CAnimator *,Parentage,const sf::Vector2f&,KindMovement=KindMovement::LEFT_RUN,int=100);
     virtual ~CPhysicaltObject(){}

     virtual void update()override;
     virtual void hop(float);

     void physicGameWithBlock(map<KindCollision,CBlock*>&,bool);

     bool isJumping()const {return m_jump;}
     bool isInBottomCollision()const {return m_in_bottom_collision;}

};

