#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <cassert>

#include <functional>

class Label {
    const sf::Color mBasicColor;

    const std::unique_ptr<sf::Text> mName;
    std::function<void(int)> mAction;

    std::vector<std::unique_ptr<sf::RectangleShape>> mRectangleShapes;
    std::unique_ptr<sf::Text> mText;

public:

    Label(sf::Text*, std::function<void(int)>);
    Label(sf::Text*);

    void Draw(const std::unique_ptr<sf::RenderWindow>&);
    void HandleAction(int pAscii) const { assert(mAction); mAction(pAscii); }

    void AddAction(std::function<void(int)> pAction) { mAction = pAction; }
    void AddText(sf::Text* pText) { mText.reset(pText); }
    void AddRectangleShape(sf::RectangleShape* pRectangle) { mRectangleShapes.push_back(std::unique_ptr<sf::RectangleShape>(pRectangle)); }

    void SetColor(sf::Color pColor) { mName->setFillColor(pColor); }
    void SetOnBasicColor() { mName->setFillColor(mBasicColor); }

    [[nodiscard]] sf::Vector2f GetPosition() const;
    [[nodiscard]] sf::Color GetBasicColor() const { return mBasicColor; }
    [[nodiscard]] const std::unique_ptr<sf::RectangleShape>& GetRectangleShape(int) const;
    [[nodiscard]] const std::unique_ptr<sf::Text>& GetText() const { return mText; }

};

///--------------------------------------///
class Menu {
    class PointerToLabel {
        using REF_TO_LABELS = const std::vector<std::unique_ptr<Label>>&;

        REF_TO_LABELS mPointerToLabels;
        int mCurrentLabel = 0;
        static const sf::Vector2f sSpriteOffsetPos;

        std::unique_ptr<sf::Sprite> mSprite;

        void Update();

    public:

        PointerToLabel(REF_TO_LABELS, const std::string&);

        PointerToLabel(const PointerToLabel&) = delete;
        PointerToLabel& operator=(const PointerToLabel&) = delete;

        void Draw(const std::unique_ptr<sf::RenderWindow>&);
        void SwitchPointer(bool);

        [[nodiscard]] const std::unique_ptr<Label>& GetCurrentLabel();

    };

    ///////////////

    std::unique_ptr<PointerToLabel> mPointerToLabel;

    std::unique_ptr<sf::RectangleShape> mBackground;
    std::unique_ptr<sf::Text> mTitle;

    std::vector<std::unique_ptr<Label>> mLabels;

    void SetTitle(std::string, bool);

public:

    const bool mIsVerticalControl;
    static const float sValueMargin;

    explicit Menu(bool, std::vector<Label*>&, sf::RectangleShape*, sf::Text*, std::string = "none");
    explicit Menu(bool, std::vector<Label*>&, sf::RectangleShape*, std::string, bool = false, std::string = "none");

    Menu(const Label&) = delete;
    Menu& operator=(const Label&) = delete;

    void Draw(const std::unique_ptr<sf::RenderWindow>&);

    void HandleLabel(int pAscii) { mPointerToLabel->GetCurrentLabel()->HandleAction(pAscii); }
    void SwitchPointer(bool pUpOrLeft) { mPointerToLabel->SwitchPointer(pUpOrLeft); }

    static Label* CreateReturnLabel(sf::Text*);
    static Label* CreateOptionLabel(const sf::Vector2f, const sf::Vector2f, const sf::Color);
};
