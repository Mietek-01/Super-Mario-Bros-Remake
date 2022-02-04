#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;

class CLandScape
{
    unique_ptr<sf::Sprite> m_sprite;

public:
    CLandScape(sf::Vector2f,sf::IntRect);
    CLandScape(sf::Vector2f,sf::IntRect,float);

    sf::FloatRect getBounds()const {return m_sprite->getGlobalBounds();}
    void draw(const unique_ptr<sf::RenderWindow>&);

};

