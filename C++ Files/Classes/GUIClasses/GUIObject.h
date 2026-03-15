#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

/// Base class for all GUI objects that can be drawn and updated.
class GuiObject {
protected:
    bool mIsRemove = false;
    const float mWhenRemove; /// Removal threshold: time or position

public:
    virtual ~GuiObject() {}
    GuiObject() : mWhenRemove(0.0f) {}
    explicit GuiObject(float pWhenRemove) : mWhenRemove(pWhenRemove) {}

    virtual void Draw(std::unique_ptr<sf::RenderWindow>& pWindow) = 0;
    virtual void Update() = 0;

    [[nodiscard]] bool IsRemove() const { return mIsRemove; }
};
