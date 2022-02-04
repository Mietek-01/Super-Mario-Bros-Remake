#pragma once
#include "GUIObject.h"
#include <functional>

using namespace std;

class CFlowText:public CGuiObject
{
    const float m_speed_changing_transparency;

    float m_value_transparency=0.0f;
    sf::Color m_color;

    unique_ptr<sf::Text> m_text;
    std::function<bool(unique_ptr<sf::Text>&)> m_special_update;

    enum class KindChangingTransparency
    {
        NONE,
        LOWER,
        TALLER
    }m_kind_changing_transparency;

public:
     CFlowText(sf::Text*,float,float =-1.8f);
     CFlowText(sf::Text*,std::function<bool(unique_ptr<sf::Text>&)>,float =-1.8f);
    ~CFlowText(){}

    void draw(unique_ptr<sf::RenderWindow>&window)override{window->draw(*m_text);}
    void update()override;
};

