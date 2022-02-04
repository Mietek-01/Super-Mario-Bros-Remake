#include "MenuScen.h"
#include "../GUIClasses/GUI.h"
#include "LoadingGameScen.h"
#include "../GameClasses/Mario.h"
#include "../MarioGame.h"

const sf::Vector2f CMenuScen::s_pos_my_first_label(350,400);
const sf::Vector2f CMenuScen::s_pos_menus(970,350);

///-----
CMenuScen::~CMenuScen()
{
    CGUI::removeAllMenus();
}

///------
CMenuScen::CMenuScen():CScen("MenuBackground")
{
    createMenu();
}

///------
inline void CMenuScen::createMenu()
{
    sf::Text * name;
    vector<CLabel*> labels;

    ///-----------------------------START GAME LABEL-------------------------///

    std::function<void(int)> start_game_action=[](int ascii)
    {
        sf::Text * name;
        const sf::IntRect bounds_menu(CMenuScen::s_pos_menus.x,CMenuScen::s_pos_menus.y,500,500);

        const sf::Vector2f pos_label={bounds_menu.left-bounds_menu.width/2.0f+CMenu::s_value_margin,
                bounds_menu.top-bounds_menu.height/2.0f+CMenu::s_value_margin+10};

        vector<CLabel*> labels;

        /// WORLDS

        size_t index_lvl=0;

        for(auto lvl_name:CMarioGame::instance().getLevelsNames())
        {
            sf::Color color(250,250,255,100);

            if(index_lvl>CMarioGame::instance().getCompletedLvlsNumber())
                color=sf::Color(250,250,255,35);

            name=CGUI::createText(lvl_name,{pos_label.x,pos_label.y+CMenu::s_value_margin*(index_lvl+1)},color,"menu_font",false);

            labels.push_back(new CLabel(name,[lvl_name,index_lvl](int ascii)
            {
                if(index_lvl>CMarioGame::instance().getCompletedLvlsNumber())
                    return;

                CMario::resetLivesMario();
                CGUI::resetMainLabels();

                CMarioGame::instance().setIndexLevel(index_lvl);
                CMarioGame::instance().changeScen(new CLoadingGameScen(lvl_name));
            }));

            index_lvl++;
        }

        /// MAIN MENU

        name=CGUI::createText("MAIN MENU",{pos_label.x,bounds_menu.top+bounds_menu.height/2.0f-CMenu::s_value_margin},sf::Color(250,250,255,100),"menu_font",false);
        labels.push_back(CMenu::createReturnLabel(name));

        ///----------///

        sf::RectangleShape* background=CGUI::createRectangleShape(bounds_menu,sf::Color::Black,true,true);
        CGUI::addMenu(new CMenu(true,labels,background,"SELECT WORLD"));
    };

    name=CGUI::createText("START GAME",s_pos_my_first_label,sf::Color::Black,"menu_font");
    labels.push_back(new CLabel(name,start_game_action));

    ///-----------------------OPTIONS LABEL------------------------------////

    labels.push_back(CMenu::createOptionLabel({s_pos_my_first_label.x,s_pos_my_first_label.y+CMenu::s_value_margin},CMenuScen::s_pos_menus,sf::Color::Black));

    ///------------------------ABOUT LABEL---------------------------------///

    std::function<void(int)> about_action=[](int ascii)
    {
        const sf::IntRect bounds_menu(CMenuScen::s_pos_menus.x,CMenuScen::s_pos_menus.y,530,500);

        const sf::Vector2f pos_label={bounds_menu.left-bounds_menu.width/2.0f+CMenu::s_value_margin,
                bounds_menu.top-bounds_menu.height/2.0f+CMenu::s_value_margin};
        
        const string my_describe=
        "HI, I AM MICHAL NIEDEK AND THIS IS\
        \nMY REMAKE OF SUPER MARIO BROS\
        \nGAME WITH A SMALL EXTENSION IN\
        \nIN FIGHT WITH A BOSS.\
        \n\nHAVE A NICE GAME!\
        \n\n                                         GOOD LUCK!!!";

        vector<CLabel*> labels;

        ///---------///

        sf::Text *name=CGUI::createText("MAIN MENU",{pos_label.x,bounds_menu.top+bounds_menu.height/2.0f-CMenu::s_value_margin},sf::Color(250,250,255,100),"menu_font",false);
        labels.push_back(CMenu::createReturnLabel(name));

        sf::RectangleShape *background=CGUI::createRectangleShape(bounds_menu,sf::Color::Black,true,true);
        sf::Text * about_me_text=CGUI::createText(my_describe,sf::Vector2f(pos_label.x-CMenu::s_value_margin*0.5,pos_label.y-CMenu::s_value_margin*0.5),sf::Color::White,"arial",false,30);

        CGUI::addMenu(new CMenu(true,labels,background,about_me_text));
    };

    name=CGUI::createText("ABOUT",{s_pos_my_first_label.x,s_pos_my_first_label.y+CMenu::s_value_margin*2},sf::Color::Black,"menu_font");
    labels.push_back(new CLabel(name,about_action));

    ///------------------------QUIT GAME LABEL---------------------------------///

    std::function<void(int)> quite_game_action=[](int ascii)
    {
        CGUI::addMenu(CGUI::creatAreYouSureMenu([](int ascii){CMarioGame::instance().getWindow()->close();}));
    };

    name=CGUI::createText("QUITE GAME",{s_pos_my_first_label.x,s_pos_my_first_label.y+CMenu::s_value_margin*3},sf::Color::Black,"menu_font");
    labels.push_back(new CLabel(name,quite_game_action));

    /// DODAJE MENU
    CGUI::addMenu(new CMenu(true,labels,nullptr,nullptr,string("Menumshr")));
}
