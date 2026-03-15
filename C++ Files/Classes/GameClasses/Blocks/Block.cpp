#include "Block.h"

using namespace std;

///--------------------Block---------------///
Block::Block(const sf::IntRect& pPositionTexture, const sf::Vector2f& pPos, const string pName)
    : GameObject(new SpriteAnimator(Gui::CreateSprite(pName, pPositionTexture, pPos, kScaleToTile, true)), Parentage::Block, pPos)
{
}

///------
void Block::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    this->CorrectObjectPositionOnMe(*pObj, pHowCollision);
}

///--------------------DynamicBlock---------------///
const float DynamicBlock::sBlockGravitation = 2000.0f;

DynamicBlock::DynamicBlock(sf::IntRect pBounds, sf::Vector2f pPos, string pName)
    : Block(pBounds, pPos, pName)
    , mBasicYPosition(pPos.y)
    , mForce(mJumpForce)
{
}

///-------
void DynamicBlock::MakeJump()
{
    mPreviousPosition.y = mCurrentPosition.y;

    mForce += sBlockGravitation * Scene::GetFrameTime();
    mCurrentPosition.y += mForce * Scene::GetFrameTime();

    if (mCurrentPosition.y >= mBasicYPosition) {
        mCurrentPosition.y = mBasicYPosition;
        mForce = mJumpForce;
        mIsHit = false;
    }

    mAnimator->setPosition(mCurrentPosition);
}

///------
void DynamicBlock::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    switch (pWhichAction) {
        case KindAction::Destroyed: {
            CreateRemoveBlock();
            MarioGame::sSoundManager.play("breakblock");
            break;
        }

        case KindAction::Hit: {
            mForce = mJumpForce;
            mIsHit = true;
            break;
        }
    }
}

///------
void DynamicBlock::CreateRemoveBlock()
{
    static const sf::Vector2f forces[] = {{1, -300}, {-1, -300}, {1, -450}, {-1, -450}};

    for (int i = 0; i < 4; i++) {
        sf::Sprite* sprite = new sf::Sprite(mAnimator->getSprite());

        sprite->setScale(0.5, 0.5);
        sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
        sprite->setPosition(mCurrentPosition.x, mCurrentPosition.y - GetSize().y / 2.0f);

        SpecialEffects* obj = new SpecialEffects(new SpriteAnimator(sprite), SpecialEffects::KindUpdate::JUMP, MarioGame::sSizeWindow.y + Scene::sTileSize, forces[i]);
        obj->activeRotate();

        Gui::AddGuiObject(obj);
    }

    RemoveObject(this);
}
