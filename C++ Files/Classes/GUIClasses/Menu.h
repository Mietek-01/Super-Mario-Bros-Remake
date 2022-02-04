#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <assert.h>

#include <functional>

using namespace std;

class CLabel
{
    const sf::Color m_basic_color;

    const unique_ptr<sf::Text> m_my_name;
    function<void(int)> m_action;

    vector<std::unique_ptr<sf::RectangleShape>> m_rectangle_shape;
    unique_ptr<sf::Text> m_text;

public:

    CLabel(sf::Text*,function<void(int)>);
    CLabel(sf::Text*);

    void draw(const unique_ptr<sf::RenderWindow>&);
    void handleAction(int ascii)const{assert(m_action);m_action(ascii);}

    void addAction(function<void(int)>action){m_action=action;}
    void addText(sf::Text* text){m_text.reset(text);}
    void addRectangleShap(sf::RectangleShape* rctng){m_rectangle_shape.push_back(unique_ptr<sf::RectangleShape>(rctng));}

    void setColor(sf::Color color){m_my_name->setFillColor(color);}
    void setOnBasicColor(){m_my_name->setFillColor(m_basic_color);}

    sf::Vector2f getPosition()const;
    sf::Color getBasicColor()const{return m_basic_color;}
    const unique_ptr<sf::RectangleShape>& getRectangleShape(int)const;
    const unique_ptr<sf::Text>& getText()const{return m_text;}

};

///--------------------------------------///
class CMenu
{
    class CPointerToLabel
    {
        using REF_TO_LABELS=const vector<unique_ptr<CLabel>>&;

        REF_TO_LABELS m_pointer_to_labels;
        int m_current_label=0;
        static const sf::Vector2f s_sprite_offset_poz;

        unique_ptr<sf::Sprite> m_sprite;

        void update();

    public:

        CPointerToLabel(REF_TO_LABELS,const string&);

        CPointerToLabel(const CPointerToLabel&)=delete;
        CPointerToLabel& operator=(const CPointerToLabel&)=delete;

        void draw(const unique_ptr<sf::RenderWindow>&);
        void switchPointer(bool);

        const unique_ptr<CLabel>& getCurrentLabel();

    };

    ///////////////

    unique_ptr<CPointerToLabel> m_pointer_to_label;

    unique_ptr<sf::RectangleShape> m_background;
    unique_ptr<sf::Text> m_title;

    vector<std::unique_ptr<CLabel>> m_my_labels;

    void setTitle(string,bool);

public:

    const bool m_vertical_control;
    static const float s_value_margin;

    CMenu(bool,vector<CLabel*>&,sf::RectangleShape*,sf::Text *,string ="none"); explicit
    CMenu(bool,vector<CLabel*>&,sf::RectangleShape*,string,bool=false,string ="none"); explicit

    CMenu(const CLabel&)=delete;
    CMenu& operator=(const CLabel&)=delete;

    void draw(const unique_ptr<sf::RenderWindow>&);

    void handleLabel(int ascii){m_pointer_to_label->getCurrentLabel()->handleAction(ascii);}
    void switchPointer(bool up_or_left){m_pointer_to_label->switchPointer(up_or_left);}

    static CLabel* createReturnLabel(sf::Text*);
    static CLabel* createOptionLabel(const sf::Vector2f,const sf::Vector2f,const sf::Color);
};

