#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include <map>
#include <string>

/// Translates raw SFML input events into game-level player decisions
/// and dispatches registered event callbacks (e.g. pause menu).
class EventHandler final {

public:

    /// High-level game events that can have callbacks attached.
    enum class Event {
        PausedMenu
    };

    /// Logical player actions bound to physical keys.
    enum class DecisionsPlayer {
        ClickLeftRun,
        ClickRightRun,
        ClickJump,
        ClickCrouch,
        ClickShoot
    };

    EventHandler();

    void HandleEvents(const std::unique_ptr<sf::RenderWindow>& pWindow);
    void AddEvent(Event pEvent, std::function<void()> pAction);

    [[nodiscard]] static std::map<DecisionsPlayer, int>& GetRegisteredKeys() { return sRegisteredKeys; }
    [[nodiscard]] static std::map<DecisionsPlayer, bool>& GetEnabledDecisions() { return sEnabledDecisions; }
    [[nodiscard]] static std::map<DecisionsPlayer, bool>& GetDeactivateDecisions() { return sDeactivateDecisions; }

    static void ResetDecisions();
    static void SetDefaultRegisteredKeys();
    static void ChangeRegisteredKeys(DecisionsPlayer pDecision, int pKeyCode);
    [[nodiscard]] static bool IsOccupyThisKeyCode(int pKeyCode);
    [[nodiscard]] static std::string ConvertKeyToString(int pKeyCode);

private:

    std::map<Event, std::function<void()>> mAllEvents;

    static std::map<DecisionsPlayer, int> sRegisteredKeys;
    static std::map<DecisionsPlayer, bool> sEnabledDecisions;
    static std::map<DecisionsPlayer, bool> sDeactivateDecisions;
};
