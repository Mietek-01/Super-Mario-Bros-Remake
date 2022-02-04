#pragma once
#include "Block.h"

class CPipe :public CStaticBlock
{
    class CFlower:public CGameObject
    {
        CPipe * m_my_pipe;

        const float m_pos_gnawing;
        const float m_how_long_hiding;
        float m_when_move_upper;
        float m_time_gnawing;

        bool m_enabled=false;

        static const float s_how_long_gnawing;
        static const float s_speed;

        enum MyKindsAnimations
        {
            GNAWING,
            MOVE_UP,
            MOVE_DOWN,

        };

        enum class State
        {
            MOVE_UP,
            MOVE_DOWN,
            HIDING,
            GNAWING
        }m_state=State::HIDING;

        inline void setWhenMoveUpper();

    public:

        CFlower(CPipe*,sf::Vector2f);
        ~CFlower(){m_my_pipe->m_my_flower=nullptr;}

        void update()override;
        void draw(const unique_ptr<sf::RenderWindow>&) override;

        void actOnMe(KindAction)override;
        void actOnObject(CGameObject*,KindCollision)override;

        void reset();
    };

    friend class CFlower;

    CFlower *m_my_flower=nullptr;

public:

    enum class KindPipe
    {
        BIG=128,
        SMALL=64,
    };

    CPipe(sf::Vector2f,KindPipe);
    virtual ~CPipe(){;}

    void resetFlower();
};


