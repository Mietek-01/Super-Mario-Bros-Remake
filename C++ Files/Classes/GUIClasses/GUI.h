#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Menu.h"
#include "GUIObject.h"
#include <functional>

class Gui final {
    static bool sIsAnyMenuActive;
    static bool sIsMaintenanceLabel;
    static bool sIsVisibleMainLabels;

    static std::vector<std::unique_ptr<sf::Text>> mMainLabels;
    static std::vector<std::unique_ptr<Menu>> mMenus;

    /// NOT USING UNIQUE_PTR BECAUSE I NEED CONTROL OVER DELETION
    static std::vector<GuiObject*> mGuiObjects;

    [[nodiscard]] static const std::unique_ptr<Menu>& GetCurrentMenu();

public:

    static const size_t sBasicSizeText;

    Gui();

    void Draw(std::unique_ptr<sf::RenderWindow>&);
    void Update();

    /// METHODS FOR HANDLING GUIOBJECTS
    static void ResetGuiObjects();
    static void AddGuiObject(GuiObject* pObj) { mGuiObjects.push_back(pObj); }

    ///METHODS FOR HANDLING MAIN LABELS
    static void ResetMainLabels();
    static void MoveMainLabels(float);
    static void SetPositionMainLabels(float);

    static void SetTime(int);
    static void SetWorldName(const std::string&);
    static void SetPoints(int);
    static void SetCoinCounter(int);
    static void SetVisibleMainLabels(bool pValue) { sIsVisibleMainLabels = pValue; }

    ///METHODS FOR HANDLING MENU
    static void HandleMenus(int);
    static void RemoveCurrentMenu();
    static void AddMenu(Menu*);
    [[nodiscard]] static bool IsMenuActive() { return sIsAnyMenuActive; }
    [[nodiscard]] static bool IsLabelMaintenance() { return sIsMaintenanceLabel; }
    static void SetLabelMaintenance(bool pValue) { sIsMaintenanceLabel = pValue; }
    static void RemoveAllMenus() { mMenus.clear(); sIsAnyMenuActive = false; }

    /// HELPER METHODS
    static sf::Text* CreateText(std::string, sf::Vector2f, sf::Color, std::string = "arial", bool = true, size_t = sBasicSizeText);
    static sf::RectangleShape* CreateRectangleShape(sf::IntRect, sf::Color, bool, bool = false, sf::Color = sf::Color::White);
    static sf::Sprite* CreateSprite(std::string, sf::IntRect, sf::Vector2f, float, bool);
    static sf::Sprite* CreateSprite(std::string, sf::Vector2f, float, bool);
    static Menu* CreateAreYouSureMenu(std::function<void(int)>);
    static std::string ToString(int);
    static int Rand(const int&, const int&);
};
