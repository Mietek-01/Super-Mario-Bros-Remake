#include "Enemies/Bowser.h"
#include "../Scens/GameScen.h"

#include <memory>
#include <vector>

///---------------- Princess ------------------///
Bowser::Princess::Princess(sf::Vector2f pPos)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("Items", {222, 106, 32, 54}, pPos, kScaleToTile, true)), Parentage::Item, pPos)
    , mBotMario(new BotMario(MarioGame::Instance().GetScene<GameScene>().GetMarioPosition()))
{
    AddNewObject(mBotMario);
}

///------
void Bowser::Princess::Update()
{
    if (!mBotMarioStopped)
    {
        if (mBotMario->GetCurrentPosition().x >= mCurrentPosition.x - GetSize().x / 2.0f - mStopMarioPosition)
        {
            mBotMario->Stop();
            mBotMarioStopped = true;
            mTimer = Scene::GetDurationScene() + mWhenShowMessage;
        }
    } else
    {
        if (!mText)
        {
            if (mTimer < Scene::GetDurationScene())
            {
                mTimer = Scene::GetDurationScene() + mWhenStateWinGame;
                mText.reset(Gui::CreateText(mMessageToMario, MarioGame::Instance().GetViewPosition(), sf::Color::White
                    , "arial", true, 70));
            }
        } else
        {
            if (mTimer < Scene::GetDurationScene())
                MarioGame::Instance().GetScene<GameScene>().SetGameState(GameScene::GameStates::WinGame);
        }
    }
}

///--------
void Bowser::Princess::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
   mAnimator->Draw(pWindow);

   if (mText)
    pWindow->draw(*mText);
}

///-----------------------------BotMario---------------------///
Bowser::Princess::BotMario::BotMario(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Item, pPos, KindMovement::RightRun)
{
    mValueAcceleration = 3.0f;
    /// CREATE ANIMATIONS
    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({32, 36, 32, 60});
    frameAnimation.push_back({65, 36, 30, 60});
    frameAnimation.push_back({96, 38, 32, 58});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.0f, kScaleToTile);

    ///------
    mAnimations->Create(Animations::RightStanding, MarioGame::sTextureManager["Mario_right"], {0, 38, 32, 58}, kScaleToTile);
    mAnimations->Create(Animations::RightJump, MarioGame::sTextureManager["Mario_right"], {160, 35, 32, 60}, kScaleToTile);

    mAnimations->Play(Animations::RightMove, mCurrentPosition);
}

///-----
void Bowser::Princess::BotMario::Update()
{
    if (!mIsBlocking && mChangeAnimation)
    {
        if (mIsJump)
            mAnimations->Play(Animations::RightJump, mCurrentPosition);
        else
            mAnimations->Play(Animations::RightMove, mCurrentPosition);

        mChangeAnimation = false;
    }

    if (!mIsBlocking && mKindMovement != KindMovement::Standing)
        MarioGame::Instance().GetScene<GameScene>().ChangePositionView(mValueAcceleration);

    PhysicalObject::Update();

    mIsBlocking = false;
}

///----
void Bowser::Princess::BotMario::UpdateForCollisionWithBlock(KindCollision pCollision, Block* pBlock)
{
    switch (pCollision)
    {
        case KindCollision::Top:
        {
            return;
        }

        case KindCollision::Bottom:
        {
            mIsJump = false;
            mIsInBottomCollision = true;

            mAnimations->Play(Animations::RightMove, mCurrentPosition);
            break;
        }

        case KindCollision::RightSide:
        {
            if (!mIsBlocking)
            {
                mChangeAnimation = true;
                if (mIsJump)
                    mAnimations->Play(Animations::RightJump, mCurrentPosition);
                else
                    mAnimations->Play(Animations::RightStanding, mCurrentPosition);
            }

            mIsBlocking = true;
            break;
        }

        case KindCollision::LeftSide:
        {
            return;
        }

        case KindCollision::None:
        {
            return;
        }

    }

    pBlock->ActOnObject(this, pCollision);
}

///---
void Bowser::Princess::BotMario::Stop()
{
    mKindMovement = KindMovement::Standing;
    mAnimations->Play(Animations::RightStanding, mCurrentPosition);
}

///------
void Bowser::Princess::BotMario::Falling()
{
    PhysicalObject::Falling();
    mAnimations->Play(Animations::RightJump, mCurrentPosition);
}
