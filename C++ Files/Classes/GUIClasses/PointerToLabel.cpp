#include "Menu.h"
#include "../MarioGame.h"

using namespace std;

const sf::Vector2f Menu::PointerToLabel::sSpriteOffsetPos(-50, -7);

Menu::PointerToLabel::PointerToLabel(REF_TO_LABELS pPtr, const string& pTextureName)
    : mPointerToLabels(pPtr)
{
    if (pTextureName != "none")
        mSprite.reset(new sf::Sprite(MarioGame::sTextureManager[pTextureName]));

    Update();
}

///------
void Menu::PointerToLabel::SwitchPointer(bool pUpOrLeft)
{
    if (!mSprite)
        GetCurrentLabel()->SetOnBasicColor();

    if (pUpOrLeft) {
        if (mCurrentLabel > 0)
            mCurrentLabel--;
    } else
        if (mCurrentLabel < mPointerToLabels.size() - 1)
            mCurrentLabel++;

    Update();
}

///------
const unique_ptr<Label>& Menu::PointerToLabel::GetCurrentLabel()
{
    assert(mPointerToLabels.size() != 0);
    return mPointerToLabels[mCurrentLabel];
}

///------
void Menu::PointerToLabel::Update()
{
    if (mSprite) {
        mSprite->setPosition(GetCurrentLabel()->GetPosition());
        mSprite->move(sSpriteOffsetPos);

    } else
        GetCurrentLabel()->SetColor(sf::Color::White);
}

///-----
void Menu::PointerToLabel::Draw(const unique_ptr<sf::RenderWindow>& pWindow)
{
    if (mSprite)
        pWindow->draw(*mSprite);
}
