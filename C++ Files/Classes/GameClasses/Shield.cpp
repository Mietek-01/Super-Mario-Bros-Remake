#include "Shield.h"
#include "../GUIClasses/GUI.h"
#include <cassert>
#include <iostream>

using namespace std;

Shield::Shield(int pShieldValue)
    : mBasicShieldValue(pShieldValue)
    , mShieldValue(pShieldValue) {
    assert(pShieldValue > 200);
}

///-----
void Shield::CreateShieldPointer(sf::RectangleShape* pRect, sf::Color pColor, ShieldPointer::KindsOrigin pKindOrigin, sf::Sprite* pSprite) {
    mShieldPointer.reset(new ShieldPointer(pRect, pColor, pKindOrigin, mBasicShieldValue, pSprite));
}

///-----
void Shield::ReduceShield() {
    if (mInflictedDamage == 0) return;

    mShieldValue -= mInflictedDamage;

    if (mShieldPointer)
        mShieldPointer->ReduceShieldPointer(mInflictedDamage);

    mInflictedDamage = 0;
}

///-------------------ShieldPointer-----------------////
const short Shield::ShieldPointer::sMargin = 7;

Shield::ShieldPointer::ShieldPointer(sf::RectangleShape* pFirstRect, sf::Color pSecondColor, KindsOrigin pKindOrigin, int pShieldValue, sf::Sprite* pSprite)
    : mSecondRect(new sf::RectangleShape({pFirstRect->getSize().x - 2 * sMargin, pFirstRect->getSize().y - 2 * sMargin}))
    , mBasicShieldValue(pShieldValue)
    , mKindOrigin(pKindOrigin)
    , mShieldPointerLength(pFirstRect->getSize().x - 2 * sMargin)
    , mFirstRect(pFirstRect) {
    switch (pKindOrigin) {
    case KindsOrigin::Left:
        break; /// Left is always the default origin
    case KindsOrigin::Right:
        mFirstRect->setOrigin(mFirstRect->getGlobalBounds().width, 0);
        break;
    case KindsOrigin::Bottom:
        mFirstRect->setOrigin(mFirstRect->getGlobalBounds().width / 2.0f, mFirstRect->getGlobalBounds().height);
        break;
    }

    if (pSprite)
        mSprite.reset(pSprite);

    mSecondRect->setFillColor(pSecondColor);
}

///---------
void Shield::ShieldPointer::Update() {
    if (mDamageRect) {
        if (mDamageRect->getSize().x - mReducingShieldSpeed * mExtraSpeed > 0)
            mDamageRect->setSize({mDamageRect->getSize().x - mReducingShieldSpeed * mExtraSpeed, mDamageRect->getSize().y});
        else {
            mExtraSpeed = 1.0f;
            mDamageRect.reset();

            if (!mSecondRect)
                mIsShieldDepleted = true;
        }
    }
}

///-------
inline void Shield::ShieldPointer::CreateDamageRectangle(float pLengthDamage) {
    if (!mDamageRect) {
        mDamageRect.reset(Gui::CreateRectangleShape(sf::IntRect(mSecondRect->getPosition().x + mSecondRect->getSize().x - pLengthDamage, mSecondRect->getPosition().y, pLengthDamage, mSecondRect->getSize().y)
            , mColorDamageRect, false));
    } else {
        mDamageRect->move({-pLengthDamage, 0});
        mDamageRect->setSize({mDamageRect->getSize().x + pLengthDamage, mDamageRect->getSize().y});
    }
}

///--------
void Shield::ShieldPointer::ReduceShieldPointer(float pDamage) {
    if (!mSecondRect) return;

    if (pDamage >= mBasicShieldValue) {
        this->CreateDamageRectangle(mSecondRect->getSize().x);
        mSecondRect.reset();
        mExtraSpeed = 3.0f;
    } else {
        const float percentDamage = pDamage / mBasicShieldValue;
        const float lengthDamage = mShieldPointerLength * percentDamage;

        sf::Vector2f newSize = mSecondRect->getSize();
        newSize.x -= lengthDamage;

        if ((int)newSize.x <= 0) {
            this->CreateDamageRectangle(mSecondRect->getSize().x);
            mSecondRect.reset();
            mExtraSpeed = 2.0f;
        } else {
            if (mDamageRect)
                mExtraSpeed = 1.5f;

            this->CreateDamageRectangle(lengthDamage);
            mSecondRect->setSize(newSize);
        }
    }
}

///---------
inline void Shield::ShieldPointer::UpdatePosition(sf::Vector2f pPos) {
    mFirstRect->setPosition(pPos);

    if (mSprite) mSprite->setPosition(mFirstRect->getGlobalBounds().left - mSprite->getGlobalBounds().width, mFirstRect->getGlobalBounds().top + mFirstRect->getGlobalBounds().height);
    if (mSecondRect) mSecondRect->setPosition({mFirstRect->getGlobalBounds().left + sMargin * 1.5f, mFirstRect->getGlobalBounds().top + sMargin * 1.5f});
    if (mDamageRect)
        if (mSecondRect)
            mDamageRect->setPosition({mSecondRect->getPosition().x + mSecondRect->getSize().x, mSecondRect->getPosition().y});
        else
            mDamageRect->setPosition(mFirstRect->getGlobalBounds().left + sMargin, mFirstRect->getGlobalBounds().top + sMargin);
}

///------
void Shield::ShieldPointer::Draw(const unique_ptr<sf::RenderWindow>& pWindow, sf::Vector2f pPos) {
    this->UpdatePosition(pPos);

    pWindow->draw(*mFirstRect);

    if (mSecondRect)
        pWindow->draw(*mSecondRect);

    if (mDamageRect)
        pWindow->draw(*mDamageRect);

    if (mSprite)
        pWindow->draw(*mSprite);
}
