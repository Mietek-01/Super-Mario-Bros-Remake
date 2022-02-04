#pragma once

#include "Scen.h"

using namespace std;

class CMenuScen: public CScen
{
    static const sf::Vector2f s_pos_my_first_label;
    static const sf::Vector2f s_pos_menus;

    inline void createMenu();

public:

    CMenuScen();
    ~CMenuScen();

    void draw(const unique_ptr<sf::RenderWindow>& window) override{window->draw(m_background);}
    void update() override {}

};


