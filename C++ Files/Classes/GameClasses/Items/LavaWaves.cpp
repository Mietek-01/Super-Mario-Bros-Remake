#include "LavaWaves.h"

#include <vector>

Animations LavaWaves::sStaticAnimator;

LavaWaves::LavaWaves(sf::Vector2f pPos)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("Tiles", {0, 0, 32, 32}, {pPos.x, pPos.y + 1}, kScaleToTile, true)), Parentage::Item, pPos)
{
}

///------
void LavaWaves::ActOnObject(GameObject* pObject, KindCollision pCollision)
{
    if (pObject->GetParentage() == Parentage::Mario)
        pObject->ActOnMe(KindAction::Hit);
    else
        RemoveObject(pObject);
}

///-----
void LavaWaves::SetStaticAnimation()
{
    std::vector<sf::IntRect> framesAnimation;

    framesAnimation.push_back(sf::IntRect(0, 64, 32, 32));
    framesAnimation.push_back(sf::IntRect(32, 64, 32, 32));
    framesAnimation.push_back(sf::IntRect(64, 64, 32, 32));
    framesAnimation.push_back(sf::IntRect(96, 64, 32, 32));

    sStaticAnimator.Create(Animations::Standard, MarioGame::sTextureManager["AnimTiles"], framesAnimation, 2.0f, kScaleToTile, true);
}

///------
void LavaWaves::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    sStaticAnimator.SetPosition(mCurrentPosition);
    sStaticAnimator.Draw(pWindow);
}
