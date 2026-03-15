#include "Coin.h"
#include "../../Scens/GameScen.h"
#include "../../GUIClasses/FlowText.h"

#include <memory>
#include <vector>

Animations Coin::sStaticAnimator;
int Coin::sCountGatheredCoins = 0;

Coin::Coin(sf::Vector2f pPos)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("Items", {0, 0, 14, 30}, pPos, kScaleToTile, true)), Parentage::Item, pPos)
{
}

///-----
void Coin::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
        ActOnMe(KindAction::Hit);
}

///----
void Coin::ActOnMe(KindAction pAction)
{
    if (pAction == KindAction::Hit)
    {
        Animations* fallingApartAnimation = CreateFallingApartCoinAnimation();
        mCurrentPosition.y -= Scene::sTileSize / 2.0f;
        fallingApartAnimation->SetPosition(mCurrentPosition);

        GuiObject* obj = new SpecialEffects(fallingApartAnimation, SpecialEffects::KindUpdate::OneLoopAnimation, 0);

        Gui::AddGuiObject(obj);

        GameScene::AddPoints(mValuePoints);

        MarioGame::sSoundManager.play("coin");

        sCountGatheredCoins++;
        Gui::SetCoinCounter(sCountGatheredCoins);

        RemoveObject(this);
    }
}

///------
void Coin::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    sStaticAnimator.SetPosition(mCurrentPosition);
    sStaticAnimator.Draw(pWindow);
}

///-----
void Coin::SetStaticAnimation()
{
    Animations* animation = CreateFlashingCoinAnimation();
    sStaticAnimator = *animation;
    sStaticAnimator.Play(Animations::Standard, {0, 0});

    delete animation;
}

///------
Animations* Coin::CreateFallingApartCoinAnimation()
{
    Animations* animation = new Animations();
    std::vector<sf::IntRect> framesAnimation;

    framesAnimation.push_back(sf::IntRect(12, 123, 15, 15));
    framesAnimation.push_back(sf::IntRect(44, 115, 34, 33));
    framesAnimation.push_back(sf::IntRect(81, 115, 40, 32));
    framesAnimation.push_back(sf::IntRect(120, 115, 42, 32));
    framesAnimation.push_back(sf::IntRect(161, 121, 38, 24));

    animation->Create(Animations::Standard, MarioGame::sTextureManager["Items"], framesAnimation, 2.0f, kScaleToTile, true);
    return animation;
}

///-----
Animations* Coin::CreateFlashingCoinAnimation()
{
    Animations* animation = new Animations();
    std::vector<sf::IntRect> framesAnimation;

    framesAnimation.push_back(sf::IntRect(9, 85, 14, 30));
    framesAnimation.push_back(sf::IntRect(39, 85, 20, 30));
    framesAnimation.push_back(sf::IntRect(74, 85, 14, 30));
    framesAnimation.push_back(sf::IntRect(110, 85, 10, 30));

    animation->Create(Animations::Standard, MarioGame::sTextureManager["Items"], framesAnimation, 3.0f, kScaleToTile, true);
    return animation;
}
