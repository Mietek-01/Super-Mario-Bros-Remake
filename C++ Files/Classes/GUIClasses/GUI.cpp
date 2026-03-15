#include "GUI.h"
#include "../Scens/Scen.h"
#include "../Scens/GameScen.h"
#include "SpecialEffects.h"
#include "../MarioGame.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include "../GameClasses/Items/Coin.h"

using namespace std;

const size_t Gui::sBasicSizeText = 25;

bool Gui::sIsAnyMenuActive = false;
bool Gui::sIsMaintenanceLabel = false;
bool Gui::sIsVisibleMainLabels = true;

std::vector<unique_ptr<sf::Text>> Gui::mMainLabels;
std::vector<unique_ptr<Menu>> Gui::mMenus;
std::vector<GuiObject*> Gui::mGuiObjects;

///---------
Gui::Gui()
{
    sf::Text* mainLabelText;

    for (int i = 0; i < 4; i++) {
        mainLabelText = CreateText(" ", {0, 0}, sf::Color::White, "score_font", false, 31);
        mMainLabels.push_back(unique_ptr<sf::Text>(mainLabelText));
    }

    ResetMainLabels();
}

///------
void Gui::Update()
{
    for (auto it = mGuiObjects.begin(); it != mGuiObjects.end();) {
        (*it)->update();

        if ((*it)->isRemove()) {
            /// THIS ORDER IS VERY IMPORTANT
            /// BECAUSE IN GUI OBJECT DESTRUCTORS I MAY ADD
            /// ANOTHER GUI OBJECT WHICH WOULD THEN CAUSE AN ERROR

            GuiObject* removeObj = (*it);
            it = mGuiObjects.erase(it);
            delete removeObj;

        } else
            it++;
    }
}

///------
void Gui::Draw(unique_ptr<sf::RenderWindow>& pWindow)
{
    for (auto obj : mGuiObjects) obj->draw(pWindow);

    if (sIsVisibleMainLabels)
        for (auto& label : mMainLabels) pWindow->draw(*label);

    for (auto& menu : mMenus)
        menu->Draw(pWindow);
}

///--------
void Gui::ResetGuiObjects()
{
    SpecialEffects::deactivateDeathAnimations(true);

    for (auto obj : mGuiObjects)
        delete obj;

    mGuiObjects.clear();

    SpecialEffects::deactivateDeathAnimations(false);
}

///------------------FOR HANDLING MAIN LABELS-------------////
void Gui::MoveMainLabels(float pRoad)
{
    for (auto& obj : mMainLabels)
        obj->move(pRoad, 0);
}

///------------
void Gui::SetTime(int pTime)
{
    string fillZeros = "";

    if (pTime < 10)
        fillZeros = "00";
    else
    if (pTime < 100)
        fillZeros = "0";

    mMainLabels[3]->setString("TIME: " + fillZeros + ToString(pTime));
}

///------------
void Gui::SetWorldName(const string& pName)
{
    mMainLabels[2]->setString(pName);
}

///------------
void Gui::SetPoints(int pPoints)
{
    string fillZeros;

    if (pPoints < 10)
        fillZeros = "00000";
    else if (pPoints < 100)
        fillZeros = "0000";
    else if (pPoints < 1000)
        fillZeros = "000";
    else if (pPoints < 10000)
        fillZeros = "00";
    else if (pPoints < 100000)
        fillZeros = "0";
    else if (pPoints < 1000000)
        fillZeros = "";
    else {
        fillZeros = "";
        pPoints = 999999;
    }

    mMainLabels[0]->setString("MARIO: " + fillZeros + ToString(pPoints));
}

///------------
void Gui::SetCoinCounter(int pNumber)
{
    string name;

    if (pNumber < 10)
        name = "x00" + ToString(pNumber);
    else if (pNumber < 100)
        name = "x0" + ToString(pNumber);
    else if (pNumber < 1000)
        name = "x" + ToString(pNumber);
    else
        name = "x999";

    mMainLabels[1]->setString(name);
}

///------------
void Gui::ResetMainLabels()
{
    GameScene::ResetPoints();
    Coin::ResetCountGatheredCoins();

    SetPoints(0);
    SetCoinCounter(0);
    SetWorldName("WORLD 1-1");
    SetTime(0);

    SetPositionMainLabels(MarioGame::sSizeWindow.x / 2.0f);
}

///---------
void Gui::SetPositionMainLabels(float pCenter)
{
    pCenter -= MarioGame::sSizeWindow.x / 2.0f;

    mMainLabels[0]->setPosition(pCenter + 50, 20);
    mMainLabels[1]->setPosition(pCenter + 480, 20);
    mMainLabels[2]->setPosition(pCenter + 735, 20);
    mMainLabels[3]->setPosition(pCenter + 1075, 20);
}

///---------------FOR HANDLING MENU--------------///
void Gui::HandleMenus(int pWhichKey)
{
    if (!sIsMaintenanceLabel) {
        if (pWhichKey == sf::Keyboard::Return)
            GetCurrentMenu()->HandleLabel(pWhichKey);
        else
            if (Gui::GetCurrentMenu()->mIsVerticalControl) {
                if (pWhichKey == sf::Keyboard::Up)
                    GetCurrentMenu()->SwitchPointer(true);
                else if (pWhichKey == sf::Keyboard::Down)
                    GetCurrentMenu()->SwitchPointer(false);
            } else {
                if (pWhichKey == sf::Keyboard::Right)
                    GetCurrentMenu()->SwitchPointer(false);
                else if (pWhichKey == sf::Keyboard::Left)
                    GetCurrentMenu()->SwitchPointer(true);
            }
    } else
        GetCurrentMenu()->HandleLabel(pWhichKey);
}

///-----
const unique_ptr<Menu>& Gui::GetCurrentMenu()
{
    if (mMenus.size() == 0)
        return nullptr;

    return mMenus.back();
}

///------------
void Gui::RemoveCurrentMenu()
{
    if (mMenus.size() == 0)
        return;

    mMenus.pop_back();

    if (mMenus.size() == 0)
        sIsAnyMenuActive = false;
}

///------------
void Gui::AddMenu(Menu* pMenu)
{
    mMenus.push_back(unique_ptr<Menu>(pMenu));
    sIsAnyMenuActive = true;
}

///----------------------HELPER METHODS-----------------///
Menu* Gui::CreateAreYouSureMenu(std::function<void(int)> pActionForYes)
{
    const sf::Vector2f centerPos(MarioGame::Instance().getWindow()->getView().getCenter());
    const sf::IntRect boundsMenu(centerPos.x, centerPos.y, 400, 150);

    vector<Label*> labels;
    const float posYNames = centerPos.y + 30.0f;

    /// ---- CREATE YES, NO LABELS

    sf::Text* yesText = Gui::CreateText("YES", sf::Vector2f(boundsMenu.left - 60, posYNames), sf::Color(250, 250, 255, 100), "menu_font");
    sf::Text* noText = Gui::CreateText("NO", sf::Vector2f(boundsMenu.left + 60, posYNames), sf::Color(250, 250, 255, 100), "menu_font");

    labels.push_back(new Label(yesText, pActionForYes));
    labels.push_back(Menu::CreateReturnLabel(noText));

    sf::RectangleShape* rctngl = Gui::CreateRectangleShape(boundsMenu, sf::Color::Black, true, true);

    return new Menu(false, labels, rctngl, "ARE YOU SURE?");
}

///-------
sf::Text* Gui::CreateText(string pName, sf::Vector2f pPos, sf::Color pColor, string pFontName, bool pMiddleOrigin, size_t pSizeText)
{
    sf::Text* text = new sf::Text(pName, MarioGame::sFontManager[pFontName], pSizeText);
    text->setFillColor(pColor);

    if (pMiddleOrigin)
        text->setOrigin(text->getGlobalBounds().width / 2.0f, text->getGlobalBounds().height / 2.0f);

    text->setPosition(pPos);

    return text;
}

///------------
sf::RectangleShape* Gui::CreateRectangleShape(sf::IntRect pBounds, sf::Color pColor, bool pMiddleOrigin, bool pOutLine, sf::Color pColorOutLine)
{
    sf::RectangleShape* rctng = new sf::RectangleShape(sf::Vector2f(pBounds.width, pBounds.height));

    if (pMiddleOrigin)
        rctng->setOrigin(pBounds.width / 2.0f, pBounds.height / 2.0f);

    if (pOutLine) {
        rctng->setOutlineThickness(3);
        rctng->setOutlineColor(pColorOutLine);
    }

    rctng->setFillColor(pColor);
    rctng->setPosition(sf::Vector2f(pBounds.left, pBounds.top));

    return rctng;
}

///---------
sf::Sprite* Gui::CreateSprite(string pName, sf::IntRect pBoundsTexture, sf::Vector2f pPos, float pScale, bool pBottomOrigin)
{
    sf::Sprite* sprite = new sf::Sprite(MarioGame::sTextureManager[pName]);
    sprite->setTextureRect(pBoundsTexture);

    if (pBottomOrigin)
        sprite->setOrigin(sprite->getGlobalBounds().width / 2.0f, sprite->getGlobalBounds().height);

    sprite->setScale(pScale, pScale);
    sprite->setPosition(pPos.x, pPos.y);

    return sprite;
}

///---
sf::Sprite* Gui::CreateSprite(string pName, sf::Vector2f pPos, float pScale, bool pBottomOrigin)
{
    sf::Sprite* sprite = new sf::Sprite(MarioGame::sTextureManager[pName]);

    if (pBottomOrigin)
        sprite->setOrigin(sprite->getGlobalBounds().width / 2.0f, sprite->getGlobalBounds().height);

    sprite->setScale(pScale, pScale);
    sprite->setPosition(pPos.x, pPos.y);

    return sprite;
}

///--------
string Gui::ToString(int pValue)
{
    std::stringstream sstream;
    sstream << pValue;
    return sstream.str();
}

///--------
int Gui::Rand(const int& pA, const int& pB)
{
    int randNumber = std::rand() % pA + pB;
    int howMany = std::rand() % 10 + 5;

    for (int i = 1; i <= howMany; i++)
        if (i == std::rand() % i + 1)
            randNumber = std::rand() % pA + pB;

    return randNumber;
}
