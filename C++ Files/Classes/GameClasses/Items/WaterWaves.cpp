#include "WaterWaves.h"

#include <vector>

Animations WaterWaves::sStaticAnimator;

WaterWaves::WaterWaves(sf::Vector2f pPos)
    : GameObject(new SpriteAnimator(Gui::CreateSprite("Tiles", {0, 0, 32, 32}, pPos, kScaleToTile, true)), Parentage::Item, pPos)
{
}

///-----
void WaterWaves::SetStaticAnimation()
{
    std::vector<sf::IntRect> framesAnimation;

    framesAnimation.push_back(sf::IntRect(0, 32, 32, 32));
    framesAnimation.push_back(sf::IntRect(32, 32, 32, 32));
    framesAnimation.push_back(sf::IntRect(64, 32, 32, 32));
    framesAnimation.push_back(sf::IntRect(96, 32, 32, 32));

    sStaticAnimator.Create(Animations::Standard, MarioGame::sTextureManager["AnimTiles"], framesAnimation, 2.0f, kScaleToTile, true);
}

///------
void WaterWaves::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    sStaticAnimator.SetPosition(mCurrentPosition);
    sStaticAnimator.Draw(pWindow);
}
