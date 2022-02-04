#include "EventHandler.h"
#include "GUIClasses/GUI.h"
#include <iostream>
#include <assert.h>

std::map<CEventHandler::DecisionsPlayer,int> CEventHandler::s_registered_keys;
std::map<CEventHandler::DecisionsPlayer,bool> CEventHandler::s_enabled_decisions;
std::map<CEventHandler::DecisionsPlayer,bool> CEventHandler::s_deactivate_decisions;

///----------
CEventHandler::CEventHandler()
{
    CEventHandler::setDefaultRegisteredKeys();
    CEventHandler::resetDecisions();
}

///---------
void CEventHandler::handleEvents(const unique_ptr<sf::RenderWindow>&window)
{
        sf::Event event;

        while(window->pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window->close();
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    if(event.key.code==sf::Keyboard::Escape)
                    {
                        m_all_events[Event::PAUSED_MENU]();
                        break;
                    }

                    if(CGUI::isMenuActive())
                        CGUI::handleMenus(event.key.code);
                    else
                    {
                        for(auto& obj:s_registered_keys)
                        if(obj.second==int(event.key.code))
                        {
                            s_enabled_decisions[obj.first]=true;
                            break;
                        }
                    }

                    break;
                }

                case sf::Event::KeyReleased:
                {
                    for(auto& obj:s_registered_keys)
                    if(obj.second==int(event.key.code))
                    {
                        s_deactivate_decisions[obj.first]=true;
                        break;
                    }
                    break;
                }
        }
    }

}

///-------
bool CEventHandler::isOccupyThisKeyCode(int key_code)
{
    for(auto i:s_registered_keys)
        if(i.second==key_code)return true;

    return false;
}

///----------
void CEventHandler::changeRegisteredKeys(DecisionsPlayer how_decision_to_change,int for_how_key)
{
    s_registered_keys[how_decision_to_change]=for_how_key;
}

///-----------
void CEventHandler::setDefaultRegisteredKeys()
{
    s_registered_keys[DecisionsPlayer::CLICK_JUMP]=sf::Keyboard::Space;
    s_registered_keys[DecisionsPlayer::CLICK_LEFT_RUN]=sf::Keyboard::Left;
    s_registered_keys[DecisionsPlayer::CLICK_RIGHT_RUN]=sf::Keyboard::Right;
    s_registered_keys[DecisionsPlayer::CLICK_CROUCH]=sf::Keyboard::Down;
    s_registered_keys[DecisionsPlayer::CLICK_SHOOT]=sf::Keyboard::S;
}

///---------
void CEventHandler::resetDecisions()
{
    for(auto &obj:s_enabled_decisions)obj.second=false;
    for(auto &obj:s_deactivate_decisions)obj.second=false;
}

///------
void CEventHandler::addEvent(Event event,std::function<void()>action)
{
    assert(m_all_events.find(event)==m_all_events.end());

    m_all_events[event]=action;
}

///----
string CEventHandler::convertKeyToString(int key_code)
{
    switch(key_code)
    {

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

        default:return "";
    }
}
