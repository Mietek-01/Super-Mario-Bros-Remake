#include "MoveableBlock.h"

#include <algorithm>

std::vector<MoveableBlock*> MoveableBlock::sAllMoveableBlocks;

MoveableBlock::~MoveableBlock()
{
    sAllMoveableBlocks.erase(std::find(sAllMoveableBlocks.begin(), sAllMoveableBlocks.end(), this));
}

///-----
MoveableBlock::MoveableBlock(sf::Vector2f pPos, Direction pDir)
    : StaticBlock({13, 659, 90, 16}, pPos)
    , mDir(pDir)
    , mBasicPosition(pPos)
{
    sAllMoveableBlocks.push_back(this);
}

///------
void MoveableBlock::Update()
{
    mPreviousPosition = mCurrentPosition;

    switch (mDir)
    {
    case Direction::Up:
    {
        mCurrentPosition.y -= mSpeed;

        if (mCurrentPosition.y <= mBasicPosition.y - mRange)
        {
            mCurrentPosition.y = mBasicPosition.y - mRange;
            mDir = Direction::Down;
        }

        break;
    }

    case Direction::Down:
    {
        mCurrentPosition.y += mSpeed;

        if (mCurrentPosition.y >= mBasicPosition.y + mRange)
        {
            mCurrentPosition.y = mBasicPosition.y + mRange;
            mDir = Direction::Up;
        }

        break;
    }

    case Direction::Left:
    {
        mCurrentPosition.x -= mSpeed;

        if (mCurrentPosition.x <= mBasicPosition.x - mRange)
        {
            mCurrentPosition.x = mBasicPosition.x - mRange;
            mDir = Direction::Right;
        }
        break;
    }

    case Direction::Right:
    {
        mCurrentPosition.x += mSpeed;

        if (mCurrentPosition.x >= mBasicPosition.x + mRange)
        {
            mCurrentPosition.x = mBasicPosition.x + mRange;
            mDir = Direction::Left;
        }

        break;
    }

    }

    mAnimator->SetPosition(mCurrentPosition);

    if (mObjectsOnMe.size() != 0)
    {
        for (auto it = mObjectsOnMe.begin(); it != mObjectsOnMe.end();)
        {
            if ((*it)->IsJumping())
                it = mObjectsOnMe.erase(it);
            else
            {
                const sf::Vector2f differenceMovement(mCurrentPosition.x - mPreviousPosition.x,
                                mCurrentPosition.y - mPreviousPosition.y);

                (*it)->MovePosition(differenceMovement);

                it++;
            }

        }

    }
}

///---------
void MoveableBlock::ActOnObject(GameObject* pObj, KindCollision pKindCollision)
{
    PhysicalObject* pPhyObj = dynamic_cast<PhysicalObject*>(pObj);

    if (!pPhyObj)
        return;

    for (const auto pObjOnMe : mObjectsOnMe)
        if (pObjOnMe == pObj)
            return;

    if (pKindCollision == KindCollision::Top && pPhyObj->IsInBottomCollision())
    {
        /// CZYLI GDY PRZYGNIOTE
        if (pObj->GetParentage() == Parentage::Mario)
            pObj->ActOnMe(KindAction::Hit);
        else
            RemoveObject(pObj);
    }
    else
    {
        this->CorrectObjectPositionOnMe(*pObj, pKindCollision);

        if (pKindCollision == KindCollision::Bottom)
            mObjectsOnMe.push_back(pPhyObj);
    }
}

///-------
void MoveableBlock::RemoveDeadObjOnMoveableBlock(const PhysicalObject* pObj)
{
    for (auto pMoveableBlock : sAllMoveableBlocks)
    {
        auto& objsOnMoveableBlock = pMoveableBlock->mObjectsOnMe;
        auto it = std::find(objsOnMoveableBlock.begin(), objsOnMoveableBlock.end(), pObj);

        if (it != objsOnMoveableBlock.end())
            objsOnMoveableBlock.erase(it);
    }
}
