#pragma once
#include "GUIObject.h"
#include <functional>
#include <memory>

class FlowText : public GuiObject
{
    const float mSpeedChangingTransparency;

    float mValueTransparency = 0.0f;
    sf::Color mColor;

    std::unique_ptr<sf::Text> mText;
    std::function<bool(std::unique_ptr<sf::Text>&)> mSpecialUpdate;

    enum class KindChangingTransparency
    {
        None,
        Lower,
        Taller
    } mKindChangingTransparency;

public:
    FlowText(sf::Text* pText, float pWhenRemove, float pSpeedChanging = -1.8f);
    FlowText(sf::Text* pText, std::function<bool(std::unique_ptr<sf::Text>&)> pSpecialUpdate, float pSpeedChanging = -1.8f);
    ~FlowText() {}

    void Draw(std::unique_ptr<sf::RenderWindow>& pWindow) override { pWindow->draw(*mText); }
    void Update() override;
};
