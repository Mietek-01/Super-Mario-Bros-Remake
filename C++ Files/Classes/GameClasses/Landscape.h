#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

/// Static visual element placed in the world (e.g. bushes, clouds, pipes).
class Landscape {
    std::unique_ptr<sf::Sprite> mSprite;

public:
    Landscape(sf::Vector2f pPos, sf::IntRect pBoundsTexture);
    Landscape(sf::Vector2f pPos, sf::IntRect pBoundsTexture, float pScale);

    [[nodiscard]] sf::FloatRect GetBounds() const { return mSprite->getGlobalBounds(); }
    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow);

};
