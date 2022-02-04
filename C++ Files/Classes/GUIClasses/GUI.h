#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Menu.h"
#include "GUIObject.h"
#include <functional>

using namespace std;

class CGUI final
{
    static bool s_is_any_menu_active;
    static bool s_maintenance_label;
    static bool s_visible_main_labels;

    static std::vector<unique_ptr<sf::Text>> m_main_labels;
    static std::vector<unique_ptr<CMenu>> m_menus;

    /// NIE NA UNIQUE_PTR BO MUSZE MIEC KONTROLE NA USUWANIEM
    static std::vector<CGuiObject*>  m_gui_objects;

    static const unique_ptr<CMenu>& getCurrentMenu();

public:

    static const size_t s_basic_size_text;

    CGUI();

    void draw(unique_ptr<sf::RenderWindow>&);
    void update();

    /// METODY DO OBSLUGI GUIOBJECTS
    static void resetGuiObjects();
    static void addGuiObject(CGuiObject* obj){m_gui_objects.push_back(obj);}

    ///METODY DO OBSLUGI GLOWNYCH ETYKIET
    static void resetMainLabels();
    static void moveMainLabels(float);
    static void setPositionMainLabels(float);

    static void setTime(int);
    static void setWorldName(const string &);
    static void setPoints(int);
    static void setCoinCounter(int);
    static void setVisibleMainLables(bool value){s_visible_main_labels=value;}

    ///METODY DO OBSLUGI MENU
    static void handleMenus(int);
    static void removeCurrentMenu();
    static void addMenu(CMenu*);
    static bool isMenuActive(){return s_is_any_menu_active;}
    static bool isLabelMaintenance(){return s_maintenance_label;}
    static void setLabelMaintenance(bool value){s_maintenance_label=value;}
    static void removeAllMenus(){m_menus.clear();s_is_any_menu_active=false;}

    /// METODY POMOCNICZE
    static sf::Text* createText(string,sf::Vector2f,sf::Color,string="arial",bool=true,size_t=s_basic_size_text);
    static sf::RectangleShape* createRectangleShape(sf::IntRect,sf::Color,bool,bool=false,sf::Color=sf::Color::White);
    static sf::Sprite * createSprite(string,sf::IntRect,sf::Vector2f,float,bool);
    static sf::Sprite * createSprite(string,sf::Vector2f,float,bool);
    static CMenu * creatAreYouSureMenu(function<void(int)>);
    static string toString(int);
    static int rand(const int&,const int&);
};

