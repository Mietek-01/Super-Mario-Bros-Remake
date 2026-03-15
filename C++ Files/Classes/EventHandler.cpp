#include "EventHandler.h"
#include "GUIClasses/GUI.h"
#include <iostream>
#include <cassert>

using namespace std;

std::map<EventHandler::DecisionsPlayer, int> EventHandler::sRegisteredKeys;
std::map<EventHandler::DecisionsPlayer, bool> EventHandler::sEnabledDecisions;
std::map<EventHandler::DecisionsPlayer, bool> EventHandler::sDeactivateDecisions;

///----------
EventHandler::EventHandler() {
    EventHandler::SetDefaultRegisteredKeys();
    EventHandler::ResetDecisions();
}

///---------
void EventHandler::HandleEvents(const unique_ptr<sf::RenderWindow>& pWindow) {
    sf::Event sfEvent;

    while (pWindow->pollEvent(sfEvent)) {
        switch (sfEvent.type) {
            case sf::Event::Closed: {
                pWindow->close();
                break;
            }

            case sf::Event::KeyPressed: {
                if (sfEvent.key.code == sf::Keyboard::Escape) {
                    mAllEvents[Event::PausedMenu]();
                    break;
                }

                if (Gui::IsMenuActive())
                    Gui::HandleMenus(sfEvent.key.code);
                else {
                    for (auto& obj : sRegisteredKeys)
                        if (obj.second == int(sfEvent.key.code)) {
                            sEnabledDecisions[obj.first] = true;
                            break;
                        }
                }

                break;
            }

            case sf::Event::KeyReleased: {
                for (auto& obj : sRegisteredKeys)
                    if (obj.second == int(sfEvent.key.code)) {
                        sDeactivateDecisions[obj.first] = true;
                        break;
                    }
                break;
            }
        }
    }
}

///-------
bool EventHandler::IsOccupyThisKeyCode(int pKeyCode) {
    for (auto i : sRegisteredKeys)
        if (i.second == pKeyCode) return true;

    return false;
}

///----------
void EventHandler::ChangeRegisteredKeys(DecisionsPlayer pDecision, int pKeyCode) {
    sRegisteredKeys[pDecision] = pKeyCode;
}

///-----------
void EventHandler::SetDefaultRegisteredKeys() {
    sRegisteredKeys[DecisionsPlayer::ClickJump] = sf::Keyboard::Space;
    sRegisteredKeys[DecisionsPlayer::ClickLeftRun] = sf::Keyboard::Left;
    sRegisteredKeys[DecisionsPlayer::ClickRightRun] = sf::Keyboard::Right;
    sRegisteredKeys[DecisionsPlayer::ClickCrouch] = sf::Keyboard::Down;
    sRegisteredKeys[DecisionsPlayer::ClickShoot] = sf::Keyboard::S;
}

///---------
void EventHandler::ResetDecisions() {
    for (auto& obj : sEnabledDecisions) obj.second = false;
    for (auto& obj : sDeactivateDecisions) obj.second = false;
}

///------
void EventHandler::AddEvent(Event pEvent, std::function<void()> pAction) {
    assert(mAllEvents.find(pEvent) == mAllEvents.end());

    mAllEvents[pEvent] = pAction;
}

///----
string EventHandler::ConvertKeyToString(int pKeyCode) {
    switch (pKeyCode) {

        case sf::Keyboard::Space: return "SPACE";
        case sf::Keyboard::Left: return "LEFT ARROW";
        case sf::Keyboard::Right: return "RIGHT ARROW";
        case sf::Keyboard::Down: return "BOTTOM ARROW";
        case sf::Keyboard::Up: return "TOP ARROW";
        case sf::Keyboard::RControl: return "RIGHT CTRL";
        case sf::Keyboard::LControl: return "LEFT CTRL";
        case sf::Keyboard::RShift: return "RIGHT SHIFT";
        case sf::Keyboard::LShift: return "LEFT SHIFT";

        case sf::Keyboard::A: return "A";
        case sf::Keyboard::B: return "B";
        case sf::Keyboard::C: return "C";
        case sf::Keyboard::D: return "D";
        case sf::Keyboard::E: return "E";
        case sf::Keyboard::F: return "F";
        case sf::Keyboard::G: return "G";
        case sf::Keyboard::H: return "H";
        case sf::Keyboard::I: return "I";
        case sf::Keyboard::J: return "J";
        case sf::Keyboard::K: return "K";
        case sf::Keyboard::L: return "L";
        case sf::Keyboard::M: return "M";
        case sf::Keyboard::N: return "N";
        case sf::Keyboard::O: return "O";
        case sf::Keyboard::P: return "P";
        case sf::Keyboard::R: return "R";
        case sf::Keyboard::S: return "S";
        case sf::Keyboard::T: return "T";
        case sf::Keyboard::W: return "W";
        case sf::Keyboard::U: return "U";
        case sf::Keyboard::Y: return "Y";
        case sf::Keyboard::Z: return "Z";
        case sf::Keyboard::X: return "X";
        case sf::Keyboard::Q: return "Q";
        case sf::Keyboard::V: return "V";

        default: return "";
    }
}
