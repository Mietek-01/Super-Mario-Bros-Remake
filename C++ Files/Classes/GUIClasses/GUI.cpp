#include "GUI.h"
#include "../Scens/Scen.h"
#include "../Scens/GameScen.h"
#include "SpecialEffects.h"
#include "../MarioGame.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include "../GameClasses/Items/Coin.h"

const size_t CGUI::s_basic_size_text=25;

bool CGUI::s_is_any_menu_active=false;
bool CGUI::s_maintenance_label=false;
bool CGUI::s_visible_main_labels=true;

std::vector<unique_ptr<sf::Text>> CGUI::m_main_labels;
std::vector<unique_ptr<CMenu>> CGUI::m_menus;
std::vector<CGuiObject*>  CGUI::m_gui_objects;

///---------
CGUI::CGUI()
{
    sf::Text* main_label_text;

    for(int i=0;i<4;i++)
    {
        main_label_text=createText(" ",{0,0},sf::Color::White,"score_font",false,31);
        m_main_labels.push_back(unique_ptr<sf::Text>(main_label_text));
    }

    resetMainLabels();
}

///------
void CGUI::update()
{
    for(auto it=m_gui_objects.begin();it!=m_gui_objects.end();)
    {
        (*it)->update();

        if((*it)->isRemove())
        {
            /// BARDZO WAZNA JEST TAKA KOLEJNOSC
            /// BO W DESTRUKTORACH GUI OBJECTU MOGE DODAWAC
            /// KOLEJNY GUI OBJECT A TO WTEDY POWODUJE BLAD

            CGuiObject *remove_obj=(*it);
            it=m_gui_objects.erase(it);
            delete remove_obj;

        }else
            it++;
    }
}

///------
void CGUI::draw(unique_ptr<sf::RenderWindow>&window)
{
    for(auto obj:m_gui_objects)obj->draw(window);

    if(s_visible_main_labels)
        for(auto &label:m_main_labels)window->draw(*label);

    for(auto &menu:m_menus)
        menu->draw(window);
}

///--------
void CGUI::resetGuiObjects()
{
    CSpecialEffects::deactivateDeathAnimations(true);

    for(auto obj:m_gui_objects)
        delete obj;

    m_gui_objects.clear();

    CSpecialEffects::deactivateDeathAnimations(false);
}

///------------------DO OBSLUGI GOWNYCH ETYKIET-------------////
void CGUI::moveMainLabels(float road)
{
    for(auto &obj:m_main_labels)
        obj->move(road,0);
}

///------------
void CGUI::setTime(int time)
{
    string fill_zeros="";

    if(time<10)
        fill_zeros="00";
    else
    if(time<100)
        fill_zeros="0";

    m_main_labels[3]->setString("TIME: "+fill_zeros+toString(time));
}

///------------
void CGUI::setWorldName(const string &name)
{
    m_main_labels[2]->setString(name);
}

///------------
void CGUI::setPoints(int points)
{
    string fill_zeros;

    if(points<10)
        fill_zeros="00000";
    else if(points<100)
        fill_zeros="0000";
    else if(points<1000)
        fill_zeros="000";
    else if(points<10000)
        fill_zeros="00";
    else if(points<100000)
        fill_zeros="0";
    else if(points<1000000)
        fill_zeros="";
    else
    {
        fill_zeros="";
        points=999999;
    }

    m_main_labels[0]->setString("MARIO: "+fill_zeros+toString(points));
}

///------------
void CGUI::setCoinCounter(int number)
{
    string name;

    if(number<10)
        name="x00"+toString(number);
    else if(number<100)
        name="x0"+toString(number);
    else if(number<1000)
        name="x"+toString(number);
    else
        name="x999";

    m_main_labels[1]->setString(name);
}

///------------
void CGUI::resetMainLabels()
{
    CGameScen::resetPoints();
    CCoin::resetCountGatheredCoins();

    setPoints(0);
    setCoinCounter(0);
    setWorldName("WORLD 1-1");
    setTime(0);

    setPositionMainLabels(CMarioGame::s_size_window.x/2.0f);
}

///---------
void CGUI::setPositionMainLabels(float center)
{
    center-=CMarioGame::s_size_window.x/2.0f;

    m_main_labels[0]->setPosition(center+50,20);
    m_main_labels[1]->setPosition(center+480,20);
    m_main_labels[2]->setPosition(center+735,20);
    m_main_labels[3]->setPosition(center+1075,20);
}

///---------------DO OBSLUGI MENU--------------///
void CGUI::handleMenus(int which_key)
{
    if(!s_maintenance_label)
    {
        if(which_key==sf::Keyboard::Return)
           getCurrentMenu()->handleLabel(which_key);
        else
            if(CGUI::getCurrentMenu()->m_vertical_control)
            {
                if(which_key==sf::Keyboard::Up)
                    getCurrentMenu()->switchPointer(true);
                else if(which_key==sf::Keyboard::Down)
                   getCurrentMenu()->switchPointer(false);
            }
            else
            {
                if(which_key==sf::Keyboard::Right)
                    getCurrentMenu()->switchPointer(false);
                else if(which_key==sf::Keyboard::Left)
                    getCurrentMenu()->switchPointer(true);
            }
    }
    else getCurrentMenu()->handleLabel(which_key);
}

///-----
const unique_ptr<CMenu>& CGUI::getCurrentMenu()
{
    if(m_menus.size()==0)
        return nullptr;

    return m_menus.back();
}

///------------
void CGUI::removeCurrentMenu()
{
    if(m_menus.size()==0)
        return;

    m_menus.pop_back();

    if(m_menus.size()==0)
        s_is_any_menu_active=false;
}

///------------
void CGUI::addMenu(CMenu* menu)
{
    m_menus.push_back(unique_ptr<CMenu>(menu));
    s_is_any_menu_active=true;
}

///----------------------METODY POMOCNICZE-----------------///
CMenu * CGUI::creatAreYouSureMenu(std::function<void(int)> action_for_yes)
{
    const sf::Vector2f center_pos(CMarioGame::instance().getWindow()->getView().getCenter());
    const sf::IntRect bounds_menu(center_pos.x,center_pos.y,400,150);

    vector<CLabel*> labels;
    const float pos_y_names=center_pos.y+30.0f;

    /// ---- TWORZE ETYKIETKI TAK, NIE

    sf::Text *yes_text=CGUI::createText("YES",sf::Vector2f(bounds_menu.left-60,pos_y_names),sf::Color(250,250,255,100),"menu_font");
    sf::Text *no_text=CGUI::createText("NO", sf::Vector2f(bounds_menu.left+60,pos_y_names),sf::Color(250,250,255,100),"menu_font");

    labels.push_back(new CLabel(yes_text,action_for_yes));
    labels.push_back(CMenu::createReturnLabel(no_text));

    sf::RectangleShape * rctngl=CGUI::createRectangleShape(bounds_menu,sf::Color::Black,true,true);

    return new CMenu(false,labels,rctngl,"ARE YOU SURE?");
}

///-------
sf::Text* CGUI::createText(string name,sf::Vector2f poz,sf::Color color,string font_name,bool middle_origin,size_t size_text)
{
    sf::Text *text=new sf::Text(name,CMarioGame::s_font_manager[font_name],size_text);
    text->setFillColor(color);

    if(middle_origin)
        text->setOrigin(text->getGlobalBounds().width/2.0f,text->getGlobalBounds().height/2.0f);

    text->setPosition(poz);

    return text;
}

///------------
sf::RectangleShape* CGUI::createRectangleShape(sf::IntRect bounds,sf::Color color,bool midle_origin,bool out_line,sf::Color color_out_linie)
{
    sf::RectangleShape *rctng=new sf::RectangleShape(sf::Vector2f(bounds.width,bounds.height));

    if(midle_origin)
        rctng->setOrigin(bounds.width/2.0f,bounds.height/2.0f);

    if(out_line)
    {
        rctng->setOutlineThickness(3);
        rctng->setOutlineColor(color_out_linie);
    }

    rctng->setFillColor(color);
    rctng->setPosition(sf::Vector2f(bounds.left,bounds.top));

    return rctng;
}

///---------
sf::Sprite * CGUI::createSprite(string name,sf::IntRect bounds_texture,sf::Vector2f pos,float scal,bool bottom_origin)
{
    sf::Sprite *sprite=new sf::Sprite(CMarioGame::s_texture_manager[name]);
    sprite->setTextureRect(bounds_texture);

    if(bottom_origin)
        sprite->setOrigin(sprite->getGlobalBounds().width/2.0f,sprite->getGlobalBounds().height);

    sprite->setScale(scal,scal);
    sprite->setPosition(pos.x,pos.y);

    return sprite;
}

///---
sf::Sprite * CGUI::createSprite(string name,sf::Vector2f pos,float scal,bool bottom_origin)
{
    sf::Sprite *sprite=new sf::Sprite(CMarioGame::s_texture_manager[name]);

    if(bottom_origin)
        sprite->setOrigin(sprite->getGlobalBounds().width/2.0f,sprite->getGlobalBounds().height);

    sprite->setScale(scal,scal);
    sprite->setPosition(pos.x,pos.y);

    return sprite;
}

///--------
string CGUI::toString(int valuee)
{
    std::stringstream sstream;
    sstream << valuee;
    return sstream.str();
}

///--------
int CGUI::rand(const int &a,const int &b)
{
    int rand_number=std::rand()%a+b;
    int how_many=std::rand()%10+5;

    for(int i=1;i<=how_many;i++)
        if(i==std::rand()%i+1)
            rand_number=std::rand()%a+b;

    return rand_number;
}
