#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Managers.h"
#include "Scens/Scen.h"
#include "EventHandler.h"
#include "GUIClasses/GUI.h"

using namespace std;

class CMarioGame final
{
    static CMarioGame *s_instance;

    const string m_title = "MARIO";
    const bool m_full_screan = false;
    const bool m_write_framerate = false;

    unique_ptr<CScen> m_current_scen;
    unique_ptr<sf::RenderWindow> m_window;

    unique_ptr<CEventHandler> m_event_hadler;
    unique_ptr<CGUI> m_gui;

    const vector<string> m_levels_names;
    const int m_framerate = 110;
    int m_current_level_index = 0;
    int m_completed_lvls_number;

    /// METHODS

    CMarioGame();
    ~CMarioGame();

    vector<string> loadLevelsNames();
    void loadCompletedLvlsNumber();
    void initEvents();
    void creatWindow();
    void writeFrameRate(int&, float&, float);

    static void setMenagers();

public:
    static const sf::Vector2u s_size_window;

    static CFontManager s_font_manager;
    static CTextureManager s_texture_manager;
    static CMusicManager s_music_manager;
    static CSoundManager s_sound_manager;

    /// METHODS

    static CMarioGame &instance();

    template <class T>
    bool isThisScen() const
    {
        return (bool)(dynamic_cast<T *>(m_current_scen.get()));
    }

    void run();
    void changeScen(CScen *new_scen) { m_current_scen.reset(new_scen); }
    void resetView();

    /// SETTER

    void setIndexLevel(int which_lvl)
    {
        assert(which_lvl < m_levels_names.size() && which_lvl >= 0);
        m_current_level_index = which_lvl;
    }
    void setNextlevel();
    void increaseCompletedLvlsNumber() { m_completed_lvls_number++; }

    /// GETTERY

    template <class T>
    T &getScen() const
    {
        assert(m_current_scen);
        assert(isThisScen<T>());

        return *(dynamic_cast<T *>(m_current_scen.get()));
    }

    const unique_ptr<sf::RenderWindow> &getWindow() const { return m_window; }
    sf::Vector2f getViewPosition() const { return m_window->getView().getCenter(); }

    string getCurrentLevelName() const { return m_levels_names[m_current_level_index]; }
    const vector<string> &getLevelsNames() const { return m_levels_names; }

    int getCompletedLvlsNumber() const { return m_completed_lvls_number; }
    int getLvlIndex() const { return m_current_level_index; }

    bool isItLastLevel() const { return m_current_level_index == m_levels_names.size() - 1; }
};
