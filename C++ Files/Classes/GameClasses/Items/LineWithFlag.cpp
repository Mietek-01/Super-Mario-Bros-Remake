#include "LineWithFlag.h"
#include "../../Scens/GameScen.h"
#include "../Blocks/Block.h"
#include "../Mario.h"

#include <vector>

LineWithFlag::LineWithFlag(sf::Vector2f pPos)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("TheEndLvlLine", {17, 0, 18, (int)((MarioGame::sSizeWindow.y - Scene::sTileSize * 5) / kScaleToTile)}, pPos, kScaleToTile, true)), Parentage::Item, pPos)
    , mCastle(Gui::CreateSprite("Castle", {pPos.x + Scene::sTileSize * 13, pPos.y}, 1.0f, true))
    , mCastlePosition(pPos.x + Scene::sTileSize * 13)
{
    std::vector<sf::IntRect> frameAnimation;

    frameAnimation.push_back({0, 180, 32, 32});
    frameAnimation.push_back({32, 180, 32, 32});
    frameAnimation.push_back({64, 180, 32, 32});
    frameAnimation.push_back({96, 180, 32, 32});

    mLineFlag.Create(MyKindsAnimations::FlagAnimation, MarioGame::sTextureManager["Items"], frameAnimation, 1.5f, kScaleToTile, false);

    mLineFlagPosition = mCurrentPosition;
    mLineFlagPosition.y -= GetBounds().height;
    mLineFlagPosition.y += 22;
    mLineFlagPosition.x += 3;

    mLineFlag.SetPosition(mLineFlagPosition);
}

///--------
void LineWithFlag::Update()
{
    if (mMario)
    {
        switch (mRatchetingFlagState)
        {
        case MarioAnimationsStates::LeftRatcheting:
        {
            mMarioPosition.y += mMarioSpeedRatcheting;

            if (mMarioPosition.y >= mCurrentPosition.y)
            {
                mMario->Play(MyKindsAnimations::RRatchetingByMario, mCurrentPosition);
                mMarioPosition.x = mCurrentPosition.x + mMario->GetGlobalBounds().width / 2.0f;
                mMarioPosition.y = mCurrentPosition.y;

                mRatchetingFlagState = MarioAnimationsStates::RightRatcheting;
            }

            break;
        }

        case MarioAnimationsStates::RightRatcheting:
        {
            break;
        }

        case MarioAnimationsStates::GoToCastle:
        {
            mMarioPosition.x += mMarioSpeed;
            MarioGame::Instance().GetScene<GameScene>().ChangePositionView(mMarioSpeed);

            if (mMarioPosition.x >= mCastlePosition)
            {
                mMario.reset();
                MarioGame::Instance().GetScene<GameScene>().MarioInCastle(true);

                mCastleFlag.reset(new Animations);

                std::vector<sf::IntRect> castleFrameAnimation;

                castleFrameAnimation.push_back(sf::IntRect(0, 147, 32, 32));
                castleFrameAnimation.push_back(sf::IntRect(32, 147, 32, 32));
                castleFrameAnimation.push_back(sf::IntRect(64, 147, 32, 32));
                castleFrameAnimation.push_back(sf::IntRect(96, 147, 32, 32));

                mCastleFlag->Create(MyKindsAnimations::FlagAnimation, MarioGame::sTextureManager["Items"], castleFrameAnimation, 1.5f, kScaleToTile, false);

                mCastleFlagPosition = mCastle->getPosition();

                mCastleFlagPosition.y -= mCastle->getGlobalBounds().height;
                mCastleFlagPosition.y += mCastleFlag->GetGlobalBounds().height / 2.0f;

                mCastleFlagPosition.x += mCastleFlag->GetGlobalBounds().width / 2.4f;

                mCastleFlag->SetPosition(mCastleFlagPosition);
            }

            break;
        }
        }

        if (mIsFlagRatcheting)
        {
            mLineFlagPosition.y += mFlagSpeedRatcheting;

            if (mLineFlagPosition.y >= mCurrentPosition.y - mLineFlag.GetGlobalBounds().height)
            {
                mIsFlagRatcheting = false;
                mRatchetingFlagState = MarioAnimationsStates::GoToCastle;

                mMario->Play(MyKindsAnimations::MovementMario, mCurrentPosition);
            }
        }
    }

    mLineFlag.Update(mLineFlagPosition);

    if (mCastleFlag)
    {
        if (mCastleFlagPosition.y >= mCastle->getPosition().y - mCastle->getGlobalBounds().height - mCastleFlag->GetGlobalBounds().height)
            mCastleFlagPosition.y -= mCastleFlagSpeed;

        mCastleFlag->Update(mCastleFlagPosition);
    }

    if (mMario)
        mMario->Update(mMarioPosition);
}

///-----------
void LineWithFlag::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    Mario* mario = dynamic_cast<Mario*>(pObject);

    if (mario)
    {
        std::vector<sf::IntRect> frameAnimation;
        mMario.reset(new Animations);

        switch (mario->GetLevelMario())
        {
            case Mario::LevelMario::SmallMario:
            {
                frameAnimation.push_back(sf::IntRect(35, 96, 25, 32));
                frameAnimation.push_back(sf::IntRect(68, 96, 25, 32));
                frameAnimation.push_back(sf::IntRect(97, 96, 30, 32));

                mMario->Create(MyKindsAnimations::MovementMario, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.2f, kScaleToTile);

                frameAnimation.clear();
                frameAnimation.push_back(sf::IntRect(226, 96, 27, 32));
                frameAnimation.push_back(sf::IntRect(258, 97, 27, 32));

                mMario->Create(MyKindsAnimations::RRatchetingByMario, MarioGame::sTextureManager["Mario_left"], frameAnimation, 1.5f, kScaleToTile);

                frameAnimation.clear();
                frameAnimation.push_back(sf::IntRect(258, 96, 27, 32));
                frameAnimation.push_back(sf::IntRect(226, 97, 27, 32));

                mMario->Create(MyKindsAnimations::LRatchetingByMario, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.5f, kScaleToTile);

                break;
            }

            case Mario::LevelMario::BigMario:
            {
                frameAnimation.push_back(sf::IntRect(32, 36, 32, 60));
                frameAnimation.push_back(sf::IntRect(65, 36, 30, 60));
                frameAnimation.push_back(sf::IntRect(95, 35, 32, 60));

                mMario->Create(MyKindsAnimations::MovementMario, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.2f, kScaleToTile);

                frameAnimation.clear();
                frameAnimation.push_back(sf::IntRect(192, 39, 30, 57));
                frameAnimation.push_back(sf::IntRect(226, 44, 30, 52));

                mMario->Create(MyKindsAnimations::RRatchetingByMario, MarioGame::sTextureManager["Mario_left"], frameAnimation, 1.5f, kScaleToTile);

                frameAnimation.clear();
                frameAnimation.push_back(sf::IntRect(290, 39, 30, 57));
                frameAnimation.push_back(sf::IntRect(256, 44, 30, 52));

                mMario->Create(MyKindsAnimations::LRatchetingByMario, MarioGame::sTextureManager["Mario_right"], frameAnimation, 1.5f, kScaleToTile);

                break;
            }
        }

        mMario->Play(MyKindsAnimations::LRatchetingByMario, mCurrentPosition);

        mMarioPosition = mario->GetCurrentPosition();
        mMarioPosition.x = mCurrentPosition.x;
        mMarioPosition.x -= mMario->GetGlobalBounds().width / 2.0f;

        mMario->SetPosition(mMarioPosition);

        ActOnMe(KindAction::Hit);
        RemoveObject(pObject);
    }
}

///-------
void LineWithFlag::ActOnMe(KindAction pAction)
{
    if (pAction == KindAction::Hit)
        if (MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::LevelCompleted))
            mIsFlagRatcheting = true;
}

///--------
void LineWithFlag::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    mAnimator->Draw(pWindow);
    mLineFlag.Draw(pWindow);

    if (mCastleFlag)
        mCastleFlag->Draw(pWindow);

    pWindow->draw(*mCastle);

    if (mMario)
        mMario->Draw(pWindow);
}
