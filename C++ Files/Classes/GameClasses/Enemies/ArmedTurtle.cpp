#include "ArmedTurtle.h"
#include "../../Scens/GameScen.h"

#include <ctime>
#include <vector>

const float ArmedTurtle::sLengthRoad = 150.0f;
const float ArmedTurtle::sCorrectionPosHammer = 10.0f;
const float ArmedTurtle::sForceHop = -500.0f;

ArmedTurtle::ArmedTurtle(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Enemy, {pPos.x, pPos.y - 1}, PhysicalObject::KindMovement::LeftRun, 500)
    , mAfterWhatTimeJump(rand() % 2 + 2)
    , mAfterWhatTimeThrowHammer(rand() % 2 + 1)
{
    mValueAcceleration = 0.8f;
    mJumpForce = -1200.0f;
    mWhereChangeDirection = mCurrentPosition.x - sLengthRoad;

    std::vector<sf::IntRect> frameAnimation;

    frameAnimation.push_back(sf::IntRect(288, 159, 32, 50));
    frameAnimation.push_back(sf::IntRect(320, 159, 32, 50));

    mAnimations->Create(MyKindsAnimations::R_MOVE_WITH_HAMMER, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 1.5f, kScaleToTile);

    ///------
    frameAnimation.clear();
    frameAnimation.push_back(sf::IntRect(384, 159, 32, 50));
    frameAnimation.push_back(sf::IntRect(352, 159, 32, 50));
    frameAnimation.push_back(sf::IntRect(384, 159, 32, 50));

    mAnimations->Create(MyKindsAnimations::R_MOVE_THROWING_HAMMER, MarioGame::sTextureManager["Enemies_right"], frameAnimation, 2.3f, kScaleToTile);
    ///-----
    frameAnimation.clear();
    frameAnimation.push_back(sf::IntRect(160, 159, 32, 50));
    frameAnimation.push_back(sf::IntRect(192, 159, 32, 50));

    mAnimations->Create(MyKindsAnimations::L_MOVE_WITH_HAMMER, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.5f, kScaleToTile);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back(sf::IntRect(96, 159, 32, 50));
    frameAnimation.push_back(sf::IntRect(128, 159, 32, 50));
    frameAnimation.push_back(sf::IntRect(96, 159, 32, 50));

    mAnimations->Create(MyKindsAnimations::L_MOVE_THROWING_HAMMER, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 2.3f, kScaleToTile);

    mAnimations->Play(MyKindsAnimations::L_MOVE_THROWING_HAMMER, mCurrentPosition);
    mAnimations->SetIndexFrame(2, mCurrentPosition);
}

///----------
inline void ArmedTurtle::CreateHammer()
{
    mMyHammer = new Hammer({mCurrentPosition.x, mCurrentPosition.y - GetSize().y / 2.0f + sCorrectionPosHammer}, mIsRightDirReversal);
    AddNewObject(mMyHammer);

    mWhenThrowHammer = Scene::GetDurationScene() + mAfterWhatTimeThrowHammer;

    if (mIsRightDirReversal)
        mAnimations->Play(MyKindsAnimations::R_MOVE_WITH_HAMMER, mCurrentPosition);
    else
        mAnimations->Play(MyKindsAnimations::L_MOVE_WITH_HAMMER, mCurrentPosition);
}

///----------
inline bool ArmedTurtle::RandKindJump()
{
    bool upJump = rand() % 2;

    if (upJump && mCurrentPosition.y < Scene::sTileSize * 4 + 1)
        upJump = false;

    if (upJump)
        return true;
    else
    {
        const auto gameSceneBlocks = MarioGame::Instance().GetScene<GameScene>().GetBlocks();
        std::vector<int> potentialBlockWithBottomCollision;

        const sf::Vector2f savedCurrentPosition = mCurrentPosition;
        const KindMovement savedKindMovement = mKindMovement;
        const float savedChangeDirectionPos = mWhereChangeDirection;

        int prevYPosBlockWithBottomCollision = 0;

        Hop(sForceHop);

        while (mCurrentPosition.y < MarioGame::sSizeWindow.y)
        {
            ChangeMovementDirection();
            PhysicalObject::Update();

            for (const auto& block : *gameSceneBlocks)
            {
                if (!block->IsVisible()) continue;

                const KindCollision howCollision = HowCollision(*block);

                if (howCollision == KindCollision::Bottom)
                {
                    if (mPreviousPosition.y > savedCurrentPosition.y && !block->IsHit() &&
                        block->GetBounds().top - this->GetBounds().height > prevYPosBlockWithBottomCollision)
                    {
                        potentialBlockWithBottomCollision.push_back(block->GetCurrentPosition().y);
                    }

                    prevYPosBlockWithBottomCollision = block->GetCurrentPosition().y;

                    break;
                }
            }
        }

        this->SetPosition(savedCurrentPosition);
        mPreviousPosition = savedCurrentPosition;
        mPreviousPosition.y -= PhysicalObject::sCorrectionToBottomCollision;

        mKindMovement = savedKindMovement;
        mWhereChangeDirection = savedChangeDirectionPos;

        mIsJump = false;

        if (potentialBlockWithBottomCollision.size() != 0)
        {
            mForWhichBlockBottomCollision = potentialBlockWithBottomCollision
                [rand() % potentialBlockWithBottomCollision.size()];

            mDownJump = true;

            return false;
        } else
            return true;
    }
}

///----------
void ArmedTurtle::Jump()
{
    mCanJump = false;

    if (RandKindJump())
        PhysicalObject::Jump();
    else
        Hop(sForceHop);
}

///---------
void ArmedTurtle::Update()
{
    ChangeMovementDirection();

    if (mWhenJump < Scene::GetDurationScene() && mCanJump)
        this->Jump();

    PhysicalObject::Update();

    ///----------HAMMER MECHANISM-------///

    if (!mMyHammer && mAnimations->IsLastFrame())
        CreateHammer();

    if (mMyHammer && mWhenThrowHammer < Scene::GetDurationScene())
    {
        mMyHammer->SetEnabled(mIsJump);
        mMyHammer = nullptr;

        if (mIsRightDirReversal)
            mAnimations->Play(MyKindsAnimations::R_MOVE_THROWING_HAMMER, mCurrentPosition);
        else
            mAnimations->Play(MyKindsAnimations::L_MOVE_THROWING_HAMMER, mCurrentPosition);
    }

    if (mMyHammer)
        mMyHammer->SetPosition({mCurrentPosition.x, mCurrentPosition.y - GetSize().y / 2.0f - sCorrectionPosHammer});

    ChangeLookingDirection();
}

///-------
inline void ArmedTurtle::ChangeMovementDirection()
{
    if ((mKindMovement == KindMovement::LeftRun && mCurrentPosition.x < mWhereChangeDirection) ||
        (mKindMovement == KindMovement::RightRun && mCurrentPosition.x > mWhereChangeDirection))
    {
        if (mKindMovement == KindMovement::LeftRun)
        {
            mWhereChangeDirection = mCurrentPosition.x + sLengthRoad;
            mKindMovement = KindMovement::RightRun;
        } else
        {
            mWhereChangeDirection = mCurrentPosition.x - sLengthRoad;
            mKindMovement = KindMovement::LeftRun;
        }
    }
}

///-----
inline void ArmedTurtle::ChangeLookingDirection()
{
    if (!mIsRightDirReversal && MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x > mCurrentPosition.x)
    {
        if (mMyHammer)
        {
            mAnimations->Play(MyKindsAnimations::R_MOVE_WITH_HAMMER, mCurrentPosition);
            mMyHammer->ChangeReversal(mCurrentPosition);
        } else
        {
            const int savedIndexFrame = mAnimations->GetIndexFrame();
            mAnimations->Play(MyKindsAnimations::R_MOVE_THROWING_HAMMER, mCurrentPosition);
            mAnimations->SetIndexFrame(savedIndexFrame, mCurrentPosition);
        }

        mIsRightDirReversal = true;
    } else
        if (mIsRightDirReversal && MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x < mCurrentPosition.x)
        {
            if (mMyHammer)
            {
                mAnimations->Play(MyKindsAnimations::L_MOVE_WITH_HAMMER, mCurrentPosition);
                mMyHammer->ChangeReversal(mCurrentPosition);
            } else
            {
                const int savedIndexFrame = mAnimations->GetIndexFrame();
                mAnimations->Play(MyKindsAnimations::L_MOVE_THROWING_HAMMER, mCurrentPosition);
                mAnimations->SetIndexFrame(savedIndexFrame, mCurrentPosition);
            }

            mIsRightDirReversal = false;
        }
}

///----------
void ArmedTurtle::UpdateForCollisionWithBlock(KindCollision pHowCollision, Block* pBlock)
{
    if (pHowCollision == KindCollision::Bottom)
    {
        if (pBlock->IsHit() || pBlock->IsDead())
            ActOnMe(KindAction::Hit);
        else
        {
            if (mDownJump)
            {
                if (static_cast<int>(pBlock->GetCurrentPosition().y) == mForWhichBlockBottomCollision)
                    mDownJump = false;
                else
                    return;
            }

            mIsInBottomCollision = true;
            mIsJump = false;

            SetWhenJump();
        }

        pBlock->ActOnObject(this, pHowCollision);
    }
}

///-----
void ArmedTurtle::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
        pObj->ActOnMe(KindAction::Hit);
}

///-----
void ArmedTurtle::ActOnMe(KindAction pWhichAction)
{
    if (mIsDead) return;

    switch (pWhichAction)
    {
    case KindAction::Hit:
        {
            if (mMyHammer)
                RemoveObject(mMyHammer);

            CreateBeatObjectEffect(-100.0f);

            break;
        }
    }
}

///------
inline void ArmedTurtle::SetWhenJump()
{
    mCanJump = true;
    mWhenJump = Scene::GetDurationScene() + mAfterWhatTimeJump;
}

///---------
void ArmedTurtle::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    mAnimations->Draw(pWindow);

    if (mMyHammer)
        mMyHammer->Draw(pWindow);
}
