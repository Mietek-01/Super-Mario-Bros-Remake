#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>

using namespace std;

class CEventHandler final
{

public:

    enum class Event
    {
        PAUSED_MENU
    };

    enum class DecisionsPlayer
    {
        CLICK_LEFT_RUN,
        CLICK_RIGHT_RUN,
        CLICK_JUMP,
        CLICK_CROUCH,
        CLICK_SHOOT
    };

    CEventHandler();

    void handleEvents(const unique_ptr<sf::RenderWindow>&);
    void addEvent(Event,function<void()>);

    static map<DecisionsPlayer,int>& getRegisteredKeys(){return s_registered_keys;}
    static map<DecisionsPlayer,bool>& getEnabledDecisions(){return s_enabled_decisions;}
    static map<DecisionsPlayer,bool>& getDeactivateDecisions(){return s_deactivate_decisions;}

    static void resetDecisions();
    static void setDefaultRegisteredKeys();
    static void changeRegisteredKeys(DecisionsPlayer,int);
    static bool isOccupyThisKeyCode(int);
    static string convertKeyToString(int);

private:

    map<Event,function<void()>> m_all_events;

    static map<DecisionsPlayer,int> s_registered_keys;
    static map<DecisionsPlayer,bool> s_enabled_decisions;
    static map<DecisionsPlayer,bool> s_deactivate_decisions;
};




