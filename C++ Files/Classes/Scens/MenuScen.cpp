#include "MenuScen.h"
#include "../GUIClasses/GUI.h"
#include "LoadingGameScen.h"
#include "../GameClasses/Mario.h"
#include "../MarioGame.h"

#include <functional>
#include <string>
#include <vector>

const sf::Vector2f MenuScene::sPosMyFirstLabel(350, 400);
const sf::Vector2f MenuScene::sPosMenus(970, 350);

///-----
MenuScene::~MenuScene()
{
    Gui::RemoveAllMenus();
}

///------
MenuScene::MenuScene() : Scene("MenuBackground")
{
    CreateMenu();
}

///------
inline void MenuScene::CreateMenu()
{
    sf::Text* name;
    std::vector<Label*> labels;

    ///-----------------------------START GAME LABEL-------------------------///

    std::function<void(int)> startGameAction = [](int pAscii)
    {
        sf::Text* name;
        const sf::IntRect boundsMenu(MenuScene::sPosMenus.x, MenuScene::sPosMenus.y, 500, 500);

        const sf::Vector2f posLabel = {boundsMenu.left - boundsMenu.width / 2.0f + Menu::sValueMargin,
                boundsMenu.top - boundsMenu.height / 2.0f + Menu::sValueMargin + 10};

        std::vector<Label*> labels;

        /// WORLDS

        size_t indexLevel = 0;

        for (auto levelName : MarioGame::Instance().GetLevelsNames())
        {
            sf::Color color(250, 250, 255, 100);

            if (indexLevel > MarioGame::Instance().GetCompletedLevelsNumber())
                color = sf::Color(250, 250, 255, 35);

            name = Gui::CreateText(levelName, {posLabel.x, posLabel.y + Menu::sValueMargin * (indexLevel + 1)}, color, "menu_font", false);

            labels.push_back(new Label(name, [levelName, indexLevel](int pAscii)
            {
                if (indexLevel > MarioGame::Instance().GetCompletedLevelsNumber())
                    return;

                Mario::ResetLivesMario();
                Gui::ResetMainLabels();

                MarioGame::Instance().SetIndexLevel(indexLevel);
                MarioGame::Instance().ChangeScene(new LoadingGameScene(levelName));
            }));

            indexLevel++;
        }

        /// MAIN MENU

        name = Gui::CreateText("MAIN MENU", {posLabel.x, boundsMenu.top + boundsMenu.height / 2.0f - Menu::sValueMargin}, sf::Color(250, 250, 255, 100), "menu_font", false);
        labels.push_back(Menu::CreateReturnLabel(name));

        ///----------///

        sf::RectangleShape* background = Gui::CreateRectangleShape(boundsMenu, sf::Color::Black, true, true);
        Gui::AddMenu(new Menu(true, labels, background, "SELECT WORLD"));
    };

    name = Gui::CreateText("START GAME", sPosMyFirstLabel, sf::Color::Black, "menu_font");
    labels.push_back(new Label(name, startGameAction));

    ///-----------------------OPTIONS LABEL------------------------------////

    labels.push_back(Menu::CreateOptionLabel({sPosMyFirstLabel.x, sPosMyFirstLabel.y + Menu::sValueMargin}, MenuScene::sPosMenus, sf::Color::Black));

    ///------------------------ABOUT LABEL---------------------------------///

    std::function<void(int)> aboutAction = [](int pAscii)
    {
        const sf::IntRect boundsMenu(MenuScene::sPosMenus.x, MenuScene::sPosMenus.y, 530, 500);

        const sf::Vector2f posLabel = {boundsMenu.left - boundsMenu.width / 2.0f + Menu::sValueMargin,
                boundsMenu.top - boundsMenu.height / 2.0f + Menu::sValueMargin};

        const std::string myDescribe =
        "HI, I AM MICHAL NIEDEK AND THIS IS\
        \nMY REMAKE OF SUPER MARIO BROS\
        \nGAME WITH A SMALL EXTENSION IN\
        \nIN FIGHT WITH A BOSS.\
        \n\nHAVE A NICE GAME!\
        \n\n                                         GOOD LUCK!!!";

        std::vector<Label*> labels;

        ///---------///

        sf::Text* name = Gui::CreateText("MAIN MENU", {posLabel.x, boundsMenu.top + boundsMenu.height / 2.0f - Menu::sValueMargin}, sf::Color(250, 250, 255, 100), "menu_font", false);
        labels.push_back(Menu::CreateReturnLabel(name));

        sf::RectangleShape* background = Gui::CreateRectangleShape(boundsMenu, sf::Color::Black, true, true);
        sf::Text* aboutMeText = Gui::CreateText(myDescribe, sf::Vector2f(posLabel.x - Menu::sValueMargin * 0.5, posLabel.y - Menu::sValueMargin * 0.5), sf::Color::White, "arial", false, 30);

        Gui::AddMenu(new Menu(true, labels, background, aboutMeText));
    };

    name = Gui::CreateText("ABOUT", {sPosMyFirstLabel.x, sPosMyFirstLabel.y + Menu::sValueMargin * 2}, sf::Color::Black, "menu_font");
    labels.push_back(new Label(name, aboutAction));

    ///------------------------QUIT GAME LABEL---------------------------------///

    std::function<void(int)> quitGameAction = [](int pAscii)
    {
        Gui::AddMenu(Gui::CreateAreYouSureMenu([](int pAscii) { MarioGame::Instance().GetWindow()->close(); }));
    };

    name = Gui::CreateText("QUIT GAME", {sPosMyFirstLabel.x, sPosMyFirstLabel.y + Menu::sValueMargin * 3}, sf::Color::Black, "menu_font");
    labels.push_back(new Label(name, quitGameAction));

    /// ADD MENU
    Gui::AddMenu(new Menu(true, labels, nullptr, nullptr, std::string("Menumshr")));
}
