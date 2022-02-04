#pragma once
#include "Block.h"

class CFieryBlock :public CStaticBlock
{
    class CFieryLine:public CGameObject
    {
        const float m_speed=1.8f;

    public:

        enum class Direction
        {
            LEFT=232,
            RIGHT=202

        }const m_dir;

        CFieryLine(sf::Vector2f,Direction);
        ~CFieryLine(){}

        void update()override;

        void actOnMe(KindAction)override{};
        void actOnObject(CGameObject*,KindCollision)override;

        bool isVisible()const override {return true;}

    };

public:

    CFieryBlock(sf::Vector2f,const bool);
    ~CFieryBlock(){}

};


