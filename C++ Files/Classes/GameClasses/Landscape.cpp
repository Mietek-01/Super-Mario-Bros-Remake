#include "Landscape.h"
#include "../GUIClasses/GUI.h"
#include "GameObject.h"

CLandScape::CLandScape(sf::Vector2f pos,sf::IntRect bounds_texture)
:m_sprite(CGUI::createSprite("Tiles",bounds_texture,pos,CGameObject::m_scale_to_tile,true))
{
}

CLandScape::CLandScape(sf::Vector2f pos,sf::IntRect bounds_texture,float scale)
:m_sprite(CGUI::createSprite("Tiles",bounds_texture,pos,scale,true))
{
}

void CLandScape::draw(const unique_ptr<sf::RenderWindow>&window)
{
    window->draw(*m_sprite);
}
