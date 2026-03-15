#include "FieryBlock.h"

FieryBlock::FieryBlock(sf::Vector2f pPos, const bool pRightDir)
    : StaticBlock({0, 32, 32, 32}, pPos)
{
    FieryLine::Direction dir;

    if (pRightDir)
        dir = FieryLine::Direction::Right;
    else
        dir = FieryLine::Direction::Left;

    AddNewObject(new FieryLine({pPos.x, pPos.y - GetBounds().height / 2.0f}, dir));
}

///----------FieryLine---------///

FieryBlock::FieryLine::FieryLine(sf::Vector2f pPos, Direction pDir)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("Tiles", {static_cast<int>(pDir), 667, 14, 96}, pPos, kScaleToTile, true)), Parentage::Item, pPos)
    , mDir(pDir)
{
}

///-------
void FieryBlock::FieryLine::Update()
{
    if (mDir == Direction::Right)
        mAnimator->GetSprite().rotate(mSpeed);
    else
        mAnimator->GetSprite().rotate(-mSpeed);

    mAnimator->Update(mCurrentPosition);
}

///------
void FieryBlock::FieryLine::ActOnObject(GameObject* pObj, KindCollision pKindCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        pObj->ActOnMe(KindAction::Hit);
}
