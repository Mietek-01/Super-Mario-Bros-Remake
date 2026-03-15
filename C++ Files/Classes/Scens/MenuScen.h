#pragma once

#include "Scen.h"

#include <memory>

class MenuScene : public Scene {
    static const sf::Vector2f sPosMyFirstLabel;
    static const sf::Vector2f sPosMenus;

    inline void CreateMenu();

public:

    MenuScene();
    ~MenuScene();

    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override { pWindow->draw(mBackground); }
    void Update() override {}

};
