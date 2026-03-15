#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

/// Base class for all game scenes (levels, menus, etc.).
/// Owns the background sprite and tracks per-frame timing.
class Scene {
protected:

    static float sDurationScene;
    static float sFrameTime;

    sf::Sprite mBackground;

public:

    static const size_t sTileSize;

    /// Methods
    virtual ~Scene() {}
    explicit Scene(std::string pNameBackground);

    virtual void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) = 0;
    virtual void Update() = 0;

    static void UpdateFrameTime(float pTime) { sDurationScene += pTime; sFrameTime = pTime; }

    [[nodiscard]] static float GetFrameTime() { return sFrameTime; }
    [[nodiscard]] static float GetDurationScene() { return sDurationScene; }
};
