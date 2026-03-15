#pragma once
#include "Block.h"
#include "../PhysicalObject.h"

#include <vector>

class MoveableBlock : public StaticBlock
{
    const sf::Vector2f mBasicPosition;

    const float mSpeed = 1;
    const int mRange = Scene::sTileSize * 4;

    std::vector<PhysicalObject*> mObjectsOnMe;

    static std::vector<MoveableBlock*> sAllMoveableBlocks;

public:

    enum class Direction
    {
        Left,
        Right,
        Up,
        Down
    } mDir;

    MoveableBlock(sf::Vector2f, Direction);
    ~MoveableBlock();

    void Update() override;
    void ActOnObject(GameObject*, KindCollision) override;

    static void RemoveDeadObjOnMoveableBlock(const PhysicalObject*);
};
