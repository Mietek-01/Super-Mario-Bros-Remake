#include "Menu.h"
#include "../EventHandler.h"
#include "../Scens/Scen.h"
#include "GUI.h"
#include "../MarioGame.h"

using namespace std;

const float Menu::sValueMargin = 50.0f;

/// ALLOWS ADDING A DESCRIPTION TO THE MENU IN THE FORM OF A TITLE
Menu::Menu(bool pVerticalControl, vector<Label*>& pLabels, sf::RectangleShape* pBackground, sf::Text* pTitle, string pTextureName)
    : mIsVerticalControl(pVerticalControl)
{
    ///------
    if (pBackground)
        mBackground.reset(pBackground);

    ///-----
    for (auto label : pLabels)
        mLabels.push_back(unique_ptr<Label>(label));

    ///-----
    mPointerToLabel.reset(new PointerToLabel(mLabels, pTextureName));

    ///-----
    if (pTitle)
        mTitle.reset(pTitle);
}

///------
Menu::Menu(bool pVerticalControl, vector<Label*>& pLabels, sf::RectangleShape* pBackground, string pNameTitle, bool pDoubleSize, string pTextureName)
    : mIsVerticalControl(pVerticalControl)
{
    ///------
    if (pBackground)
        mBackground.reset(pBackground);

    ///-----
    for (auto label : pLabels)
        mLabels.push_back(unique_ptr<Label>(label));

    ///-----
    mPointerToLabel.reset(new PointerToLabel(mLabels, pTextureName));

    ///-----
    SetTitle(pNameTitle, pDoubleSize);
}

///---
Label* Menu::CreateOptionLabel(const sf::Vector2f pPosLabel, const sf::Vector2f pPosOptionMenu, const sf::Color pColor)
{
    std::function<void(int)> actionCreateMenu = [pPosOptionMenu](int pAscii)
    {

    const sf::IntRect boundsMenu(pPosOptionMenu.x, pPosOptionMenu.y, 550, 500);

    const sf::Vector2f firstLabelPos(boundsMenu.left - boundsMenu.width / 2.0f + Menu::sValueMargin,
    boundsMenu.top - boundsMenu.height / 2.0f + Menu::sValueMargin / 2.0f);

    ///---- CREATE LABELS AND LATER ADD ACTIONS TO THEM

    std::vector<Label*> optionsCollection;

    const int numberLabels = 8;
    string names[numberLabels] = {"VOLUME SOUND", "VOLUME MUSIC", "LEFT RUN", "RIGHT RUN", "JUMP", "CROUCH", "SHOOT", "DEFAULT STEERAGE"};

    for (int i = 0; i < numberLabels; i++) {
        sf::Text* name;

        if (i < numberLabels - 1)
            name = Gui::CreateText(names[i], {firstLabelPos.x, firstLabelPos.y + Menu::sValueMargin * i}, sf::Color(250, 250, 255, 100), "menu_font", false);
        else
            name = Gui::CreateText(names[i], {firstLabelPos.x, firstLabelPos.y + Menu::sValueMargin * i + 15}, sf::Color(250, 250, 255, 100), "menu_font", false);

        optionsCollection.push_back(new Label(name));
    }

    ///----- SOUND VOLUME LABEL
    optionsCollection[0]->AddRectangleShape(Gui::CreateRectangleShape(sf::IntRect(firstLabelPos.x + 260, firstLabelPos.y, 10 * 20 + 2 * 5, 25), sf::Color::Black, false, true, sf::Color(250, 250, 255, 100)));
    optionsCollection[0]->AddRectangleShape(Gui::CreateRectangleShape(sf::IntRect(firstLabelPos.x + 265, firstLabelPos.y + 5, (MarioGame::sSoundManager.getVolume() / 10) * 20, 15), sf::Color(250, 250, 255, 100), false));

    /// ACTION FOR SOUND VOLUME LABEL
    optionsCollection[0]->AddAction([optionsCollection](int pAscii) {

    const unique_ptr<sf::RectangleShape>& rctn = optionsCollection[0]->GetRectangleShape(1);

    if (Gui::IsLabelMaintenance() && pAscii == (int)(sf::Keyboard::Return)) {
        optionsCollection[0]->GetRectangleShape(0)->setOutlineColor(optionsCollection[0]->GetBasicColor());
        rctn->setFillColor(optionsCollection[0]->GetBasicColor());

        Gui::SetLabelMaintenance(false);
        return;
    }

    if (!Gui::IsLabelMaintenance()) {
        optionsCollection[0]->GetRectangleShape(0)->setOutlineColor(sf::Color::White);
        rctn->setFillColor(sf::Color::White);

        MarioGame::sSoundManager.play("coin");
        Gui::SetLabelMaintenance(true);
    }

    if (pAscii == (int)(sf::Keyboard::Left) && rctn->getSize().x > 0) {
        rctn->setSize({rctn->getSize().x - 20, rctn->getSize().y});
        MarioGame::sSoundManager.updateVolume(false);
        MarioGame::sSoundManager.play("coin");
    }
    if (pAscii == (int)(sf::Keyboard::Right) && rctn->getSize().x < 200) {
        rctn->setSize({rctn->getSize().x + 20, rctn->getSize().y});
        MarioGame::sSoundManager.updateVolume(true);
        MarioGame::sSoundManager.play("coin");
    }

    });

    ///----- MUSIC VOLUME LABEL
    optionsCollection[1]->AddRectangleShape(Gui::CreateRectangleShape(sf::IntRect(firstLabelPos.x + 260, firstLabelPos.y + Menu::sValueMargin, 10 * 20 + 2 * 5, 25), sf::Color::Black, false, true, sf::Color(250, 250, 255, 100)));
    optionsCollection[1]->AddRectangleShape(Gui::CreateRectangleShape(sf::IntRect(firstLabelPos.x + 265, firstLabelPos.y + 5 + Menu::sValueMargin, (MarioGame::sMusicManager.getVolume() / 10) * 20, 15), sf::Color(250, 250, 255, 100), false));

    /// ACTION FOR MUSIC LABEL
    optionsCollection[1]->AddAction([optionsCollection](int pAscii) {

    const unique_ptr<sf::RectangleShape>& rctn = optionsCollection[1]->GetRectangleShape(1);

    if (Gui::IsLabelMaintenance() && pAscii == (int)(sf::Keyboard::Return)) {
        optionsCollection[1]->GetRectangleShape(0)->setOutlineColor(optionsCollection[1]->GetBasicColor());
        rctn->setFillColor(optionsCollection[1]->GetBasicColor());

        Gui::SetLabelMaintenance(false);
        MarioGame::sMusicManager.stop();
        return;
    }

    if (!Gui::IsLabelMaintenance()) {
        optionsCollection[1]->GetRectangleShape(0)->setOutlineColor(sf::Color::White);
        rctn->setFillColor(sf::Color::White);

        MarioGame::sMusicManager.playToDefineVolume();
        Gui::SetLabelMaintenance(true);
    }

    if (pAscii == (int)(sf::Keyboard::Left) && rctn->getSize().x > 0) {
        rctn->setSize({rctn->getSize().x - 20, rctn->getSize().y});
        MarioGame::sMusicManager.updateMusicVolume(false);
    }
    if (pAscii == (int)(sf::Keyboard::Right) && rctn->getSize().x < 200) {
        rctn->setSize({rctn->getSize().x + 20, rctn->getSize().y});
        MarioGame::sMusicManager.updateMusicVolume(true);
    }

    });

    ///----ACTIONS FOR STEERAGE LABELS
    for (int i = 2; i < numberLabels - 1; i++) {
        EventHandler::PlayerDecision myDecision = static_cast<EventHandler::PlayerDecision>(i - 2);
        string inscription = EventHandler::ConvertKeyToString(EventHandler::GetRegisteredKeys()[myDecision]);

        optionsCollection[i]->AddText(Gui::CreateText(inscription, {firstLabelPos.x + 260, firstLabelPos.y + Menu::sValueMargin * i}, sf::Color(250, 250, 255, 100), "menu_font", false));

        optionsCollection[i]->AddAction([i, optionsCollection, myDecision](int pAscii)
        {
            const unique_ptr<sf::Text>& text = optionsCollection[i]->GetText();

            if (Gui::IsLabelMaintenance()) {
                if (pAscii != (int)(sf::Keyboard::Return)) {
                    string newText = EventHandler::ConvertKeyToString(pAscii);
                    if (newText == "" || EventHandler::IsKeyCodeOccupied(pAscii)) return;

                    EventHandler::ChangeRegisteredKeys(myDecision, pAscii);

                    text->setString(newText);

                } else {
                    text->setFillColor(optionsCollection[i]->GetBasicColor());
                    Gui::SetLabelMaintenance(false);
                }
                return;
            } else {
                text->setFillColor(sf::Color::White);
                Gui::SetLabelMaintenance(true);
            }

        });
    }

    /// ACTION FOR DEFAULT STEERAGE
    optionsCollection[numberLabels - 1]->AddAction([optionsCollection, numberLabels](int pAscii) {

    if (pAscii == (int)(sf::Keyboard::Return)) {
        EventHandler::SetDefaultRegisteredKeys();

        for (int i = 2; i < numberLabels - 1; i++) {
            const string inscription = EventHandler::ConvertKeyToString(EventHandler::GetRegisteredKeys()[static_cast<EventHandler::PlayerDecision>(i - 2)]);

            optionsCollection[i]->GetText()->setString(inscription);
        }
    }

    });

    /// ADD RETURN LABELS
    optionsCollection.push_back(Menu::CreateReturnLabel(Gui::CreateText("MAIN MENU", {firstLabelPos.x, firstLabelPos.y + Menu::sValueMargin * numberLabels + 20}, sf::Color(250, 250, 255, 100), "menu_font", false)));

    /// CREATE THE MENU
    sf::RectangleShape* background = Gui::CreateRectangleShape(boundsMenu, sf::Color::Black, true, true);
    Gui::AddMenu(new Menu(true, optionsCollection, background, nullptr));

    };

    ///--------------RETURN THE CREATED LABEL----------------///
    sf::Text* name = Gui::CreateText("OPTIONS", pPosLabel, pColor, "menu_font");
    return new Label(name, actionCreateMenu);
}

///---
Label* Menu::CreateReturnLabel(sf::Text* pText)
{
    Label* returnLabel = new Label(pText, [](int pAscii)
    {
        Gui::RemoveCurrentMenu();
    });

    return returnLabel;
}

///----
void Menu::SetTitle(string pText, bool pDoubleSize)
{
    if (!mBackground)
        return;

    const sf::FloatRect& backgroundBounds = mBackground->getGlobalBounds();

    sf::Vector2f posTitle = {backgroundBounds.left + backgroundBounds.width / 2.0f, backgroundBounds.top + Menu::sValueMargin};
    size_t sizeText = Gui::sBasicSizeText;

    if (pDoubleSize) {
        sizeText *= 1.6;
        posTitle.x += 5;
    } else
        sizeText *= 1.2;

    mTitle.reset(Gui::CreateText(pText, posTitle, sf::Color::Red, "menu_font", true, sizeText));
}

///----
void Menu::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    if (mBackground)
        pWindow->draw(*mBackground);

    if (mTitle)
        pWindow->draw(*mTitle);

    mPointerToLabel->Draw(pWindow);

    for (auto& obj : mLabels)
        obj->Draw(pWindow);
}
