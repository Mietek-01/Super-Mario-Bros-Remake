#include "Menu.h"

CLabel::CLabel(sf::Text* text,function<void(int)> action)
:m_basic_color(text->getFillColor())
,m_my_name(text)
,m_action(action)
{
}

///------
CLabel::CLabel(sf::Text* text)
:m_basic_color(text->getFillColor())
,m_my_name(text)
{
}

///------
sf::Vector2f CLabel::getPosition()const
{
    return sf::Vector2f(m_my_name->getGlobalBounds().left,m_my_name->getGlobalBounds().top);
}

///------
const unique_ptr<sf::RectangleShape>& CLabel::getRectangleShape(int which)const
{
    assert(which>=0&&which<m_rectangle_shape.size());

    return m_rectangle_shape[which];
}

///------
void CLabel::draw(const unique_ptr<sf::RenderWindow>&window)
{
    window->draw(*m_my_name);

    if(m_text)
        window->draw(*m_text);

    for(auto &obj:m_rectangle_shape)
        window->draw(*obj);
}
