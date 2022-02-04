#pragma once
#include "../GameObject.h"

using namespace std;

class CBlock:public CGameObject
{

public:

    CBlock(const sf::IntRect&,const sf::Vector2f&,const string);
    virtual ~CBlock(){}

    virtual void actOnObject(CGameObject*,KindCollision)override;

    bool isHit()const {return m_hit;}

protected:

    bool m_hit=false;

};

///-------------///
class CDynamicBlock :public CBlock
{
    static const float s_block_gravitation;

    const float m_basic_y_positon;
    const float m_jump_force=-300.0f;

public:
    CDynamicBlock(sf::IntRect,sf::Vector2f,string ="Tiles");
    virtual ~CDynamicBlock(){}

    virtual void update()override{if(m_hit)makeJump();}
    virtual void actOnMe(KindAction)override;

protected:

    float m_force;

    void makeJump();
    void creatRemoveBlock();
};

///-----------------------------///
class CStaticBlock:public CBlock
{
public:
    CStaticBlock(sf::IntRect bounds,sf::Vector2f pos,string name="Tiles")
        :CBlock(bounds,pos,name){}

    virtual ~CStaticBlock(){}

    virtual void update()override{}
    void actOnMe(KindAction)override{}
};
