#include "Menu.h"

using namespace std;

Label::Label(sf::Text* pText, function<void(int)> pAction)
    : mBasicColor(pText->getFillColor())
    , mName(pText)
    , mAction(pAction)
{
}

///------
Label::Label(sf::Text* pText)
    : mBasicColor(pText->getFillColor())
    , mName(pText)
{
}

///------
sf::Vector2f Label::GetPosition() const
{
    return sf::Vector2f(mName->getGlobalBounds().left, mName->getGlobalBounds().top);
}

///------
const unique_ptr<sf::RectangleShape>& Label::GetRectangleShape(int pWhich) const
{
    assert(pWhich >= 0 && pWhich < mRectangleShapes.size());

    return mRectangleShapes[pWhich];
}

///------
void Label::Draw(const unique_ptr<sf::RenderWindow>& pWindow)
{
    pWindow->draw(*mName);

    if (mText)
        pWindow->draw(*mText);

    for (auto& obj : mRectangleShapes)
        pWindow->draw(*obj);
}
