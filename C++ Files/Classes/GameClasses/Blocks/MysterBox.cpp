#include "MysterBox.h"
#include "../../GUIClasses/FlowText.h"
#include "../../Scens/GameScen.h"
#include "../Items/Coin.h"

Animations MysteryBox::sPrivateAnimator;

MysteryBox::MysteryBox(sf::Vector2f pPos, MyItem pMyItem)
    : DynamicBlock({0, 32, 32, 32}, pPos)
    , mRandItem(pMyItem)
{
}

///---------
void MysteryBox::Update()
{
    if (mIsHit)
    {
        this->MakeJump();

        if (mForce > 0)
            mDrawQuestionMark = false;
    }

    if (mCreatingItem)
    {
        if (mRandItem == MyItem::Coin)
        {
            mToRand = mCreatingItem = false;

            Animations* coinAnimation = Coin::CreateFlashingCoinAnimation();

            coinAnimation->SetPosition({mCurrentPosition.x, mCurrentPosition.y - GetSize().y});

            SpecialEffects* obj = new SpecialEffects(coinAnimation, SpecialEffects::KindUpdate::Jump, mCurrentPosition.y - GetSize().y, {0, -600});

            ///-----------------------

            Animations* fallingApartCoinAnimation = Coin::CreateFallingApartCoinAnimation();

            fallingApartCoinAnimation->SetPosition({mCurrentPosition.x, mCurrentPosition.y - 10.0f - GetSize().y});

            obj->SetDeathAnimation(new SpecialEffects(fallingApartCoinAnimation, SpecialEffects::KindUpdate::OneLoopAnimation, 0));

            Gui::AddGuiObject(obj);

            ///------------------------------

            sf::Text* text = Gui::CreateText(Gui::ToString(Coin({0, 0}).GetValuePoints()), {mCurrentPosition.x, mCurrentPosition.y - GetSize().y - static_cast<float>(Scene::sTileSize)}, sf::Color::Red);
            Gui::AddGuiObject(new FlowText(text, mCurrentPosition.y - GetSize().y - Scene::sTileSize * 3));

            MarioGame::Instance().GetScene<GameScene>().AddPoints(Coin({0, 0}).GetValuePoints());

            MarioGame::sSoundManager.Play("coin");

        }
        else
        {
            if (!mIsHit)
            {
                if (!mMyObj)
                {
                    switch (mRandItem)
                    {
                    case MyItem::Mushroom:
                        mMyObj = new MysteryBox::Mushroom(mCurrentPosition);
                        break;
                    case MyItem::Flower:
                        mMyObj = new MysteryBox::SpecialFlower(mCurrentPosition);
                        break;
                    }

                    MarioGame::sSoundManager.Play("powerup_appears");
                }
                else
                {
                    if (mMyObj->GetCurrentPosition().y > this->mCurrentPosition.y - GetSize().y)
                        mMyObj->MovePosition({0, mLeavingSpeed});
                    else
                    {
                        mMyObj->SetPosition({mMyObj->GetCurrentPosition().x, this->mCurrentPosition.y - GetSize().y + 0.1f});
                        mMyObj->SetPreviousPosition({mMyObj->GetCurrentPosition().x, this->mCurrentPosition.y - GetSize().y - 0.2f});

                        AddNewObject(mMyObj);

                        mToRand = mCreatingItem = false;
                        mMyObj = nullptr;
                    }
                }
            }
        }
    }
}

///----------
void MysteryBox::ActOnMe(KindAction pWhichAction)
{
    /// PREVENTS HITTING WHEN THE MUSHROOM IS COMING OUT
    if (mMyObj)
        return;

    if (mToRand)
        mCreatingItem = true;

    mIsHit = true;
}

///----------
void MysteryBox::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    if (mDrawQuestionMark)
    {
        sPrivateAnimator.SetPosition(mCurrentPosition);
        sPrivateAnimator.Draw(pWindow);

    }
    else
    {
        if (mMyObj)
            mMyObj->Draw(pWindow);

        mAnimator->SetPosition(mCurrentPosition);
        mAnimator->Draw(pWindow);
    }
}

///----------
void MysteryBox::SetStaticAnimation()
{
    std::vector<sf::IntRect> framesAnimation;

    for (int i = 0; i < 4; i++)
        framesAnimation.push_back({32 * i, 0, 32, 32});

    sPrivateAnimator.Create(Animations::Standard, MarioGame::sTextureManager["AnimTiles"], framesAnimation, 3.0f, kScaleToTile, true);
}
