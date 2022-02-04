#pragma once
#include "Block.h"
#include "../PhysicalObject.h"

class CMoveableBlock :public CStaticBlock
{
    const sf::Vector2f m_basic_position;

    const float m_speed=1;
    const int m_range=CScen::s_tile_size*4;

    vector<CPhysicaltObject*> m_objects_on_me;

    static vector<CMoveableBlock*> s_all_moveable_blocks;

public:

    enum class Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    }m_dir;

    CMoveableBlock(sf::Vector2f,Direction);
    ~CMoveableBlock();

    void update()override;
    void actOnObject(CGameObject*,KindCollision)override;

    static void removeDeadObjOnMoveableBlock(const CPhysicaltObject*);
};


