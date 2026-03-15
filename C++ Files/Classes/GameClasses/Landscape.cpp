#include "Landscape.h"
#include "../GUIClasses/GUI.h"
#include "GameObject.h"

using namespace std;

Landscape::Landscape(sf::Vector2f pPos, sf::IntRect pBoundsTexture)
    : mSprite(Gui::CreateSprite("Tiles", pBoundsTexture, pPos, GameObject::mScaleToTile, true)) {
}

Landscape::Landscape(sf::Vector2f pPos, sf::IntRect pBoundsTexture, float pScale)
    : mSprite(Gui::CreateSprite("Tiles", pBoundsTexture, pPos, pScale, true)) {
}

void Landscape::Draw(const unique_ptr<sf::RenderWindow>& pWindow) {
    pWindow->draw(*mSprite);
}
