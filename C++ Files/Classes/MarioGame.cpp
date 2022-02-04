#include <iostream>
#include <assert.h>

#include "MarioGame.h"
#include "Scens/GameScen.h"
#include "GameClasses/Blocks/MysterBox.h"
#include "GameClasses/Items/Coin.h"
#include "GameClasses/Items/WaterWaves.h"
#include "GameClasses/Items/LavaWaves.h"
#include "GameClasses/Blocks/SpecialPipe.h"
#include "Scens/MenuScen.h"
#include <fstream>
#include <ctime>

using namespace std;

CMarioGame *CMarioGame::s_instance = nullptr;
const sf::Vector2u CMarioGame::s_size_window{1280, 720};

CFontManager CMarioGame::s_font_manager;
CTextureManager CMarioGame::s_texture_manager;
CMusicManager CMarioGame::s_music_manager;
CSoundManager CMarioGame::s_sound_manager;

CMarioGame &CMarioGame::instance()
{
    if (!s_instance)
        s_instance = new CMarioGame();

    return *s_instance;
}

///------
CMarioGame::~CMarioGame()
{
    /// ZAPISUJE LICZBE ZALICZONYCH POZIOMOW
    fstream file;

    file.open("Maps/Completed_lvls_number.txt", ios::out);
    file << m_completed_lvls_number;

    m_current_scen.reset(); /// WAZNE!!!

    s_instance = nullptr;
}

///------
CMarioGame::CMarioGame()
    : m_levels_names(loadLevelsNames())
{
    srand(time(NULL));

    loadCompletedLvlsNumber();

    /// USTAWIAM STATICI
    CMarioGame::setMenagers();

    CMysterBox::setStaticAnimation();
    CCoin::setStaticAnimation();
    CWaterWaves::setStaticAnimation();
    CLavaWaves::setStaticAnimation();

    ///---
    m_gui.reset(new CGUI);
    m_event_hadler.reset(new CEventHandler);

    ///----
    initEvents();
    creatWindow();

    m_current_scen.reset(new CMenuScen());
}

///------
void CMarioGame::initEvents()
{
    /// JEDYNYM EVENTEM JEST AKTYWOWANIE MENU PODCZAS ROZGRYWKI
    std::function<void()> paused_menu_action = []()
    {
        if (!(CMarioGame::instance().isThisScen<CGameScen>()) || CGUI::isMenuActive())
            return;

        CMarioGame::s_music_manager.pause();
        CMarioGame::s_sound_manager.pause();

        sf::Text *name;

        const sf::Vector2f center_pos(CMarioGame::instance().getWindow()->getView().getCenter());

        const sf::IntRect bounds_menu(center_pos.x, center_pos.y, 300, 400);

        sf::Vector2f pos_label(center_pos.x, bounds_menu.top - bounds_menu.height / 2.0f + CMenu::s_value_margin * 2.5);

        vector<CLabel *> labels;

        /// ETYKIETKI

        ///- RESUME
        name = CGUI::createText("REASUME", pos_label, sf::Color(250, 250, 255, 100), "menu_font");
        labels.push_back(new CLabel(name, [](int ascii)
                                    {
                                        CGUI::removeCurrentMenu();
                                        CMarioGame::s_music_manager.resume();
                                        CMarioGame::s_sound_manager.resume(); }));

        ///- OPTIONS
        pos_label.y += CMenu::s_value_margin * 2;

        labels.push_back(CMenu::createOptionLabel(pos_label, center_pos, sf::Color(250, 250, 255, 100)));

        ///- BACK TO MAIN MENU
        pos_label.y += CMenu::s_value_margin * 2;

        name = CGUI::createText("BACK TO MENU", pos_label, sf::Color(250, 250, 255, 100), "menu_font");
        labels.push_back(new CLabel(name, [](int ascii)
                                    { CGUI::addMenu(CGUI::creatAreYouSureMenu([](int ascii)
                                                                              {
                                                                                    CGUI::removeAllMenus();

                                                                                    CMarioGame::instance().changeScen(new CMenuScen());
                                                                                    CEnteringPipe::resetActivatedPipes();
                                                                                    CMario::resetLvlMario();

                                                                                    CMarioGame::s_music_manager.resetAllMusic();
                                                                                    CMarioGame::s_sound_manager.stop(); })); }));

        sf::RectangleShape *background = CGUI::createRectangleShape(bounds_menu, sf::Color::Black, true, true);
        CGUI::addMenu(new CMenu(true, labels, background, "PAUSED", true));
    };

    m_event_hadler->addEvent(CEventHandler::Event::PAUSED_MENU, paused_menu_action);
}

///---
void CMarioGame::resetView()
{
    m_window->setView(sf::View(sf::FloatRect(0, 0, s_size_window.x, s_size_window.y)));
}

///----
void CMarioGame::creatWindow()
{
    if (!m_full_screan)
        m_window.reset(new sf::RenderWindow(sf::VideoMode({s_size_window.x, s_size_window.y}), m_title, sf::Style::Titlebar | sf::Style::Close));
    else
    {
        m_window.reset(new sf::RenderWindow(sf::VideoMode({s_size_window.x, s_size_window.y}), m_title, sf::Style::Fullscreen));
        m_window->setMouseCursorVisible(false);
    }

    m_window->setFramerateLimit(m_framerate);
    m_window->setKeyRepeatEnabled(false);

    sf::Image icon_image;
    icon_image.loadFromFile("res/Textures/MarioIcon.png");
    m_window->setIcon(25, 34, icon_image.getPixelsPtr());
}

///-----
void CMarioGame::run()
{
    sf::Time timer = sf::Time::Zero;
    sf::Clock time;

    int frameCounter = 0;
    float secondTimer = 0;

    while (m_window->isOpen())
    {
        timer = time.restart();
        m_event_hadler->handleEvents(m_window);

        if (m_write_framerate)
            writeFrameRate(frameCounter, secondTimer, timer.asSeconds());

        ///--- -DRAW
        m_window->clear();

        m_current_scen->draw(m_window);
        m_gui->draw(m_window);

        ///----- UPDATE
        if (!CGUI::isMenuActive())
        {
            CScen::updateFrameTime(timer.asSeconds());

            m_current_scen->update();
            m_gui->update();
        }

        m_window->display();
    }

    delete this;
}

///--------------
void CMarioGame::setMenagers()
{
    const string MARIO_RES_PATH = "res/";

    /// LOAD TEXTURES
    const string textures_dir = MARIO_RES_PATH + "Textures/";
    for (string texture : {"Menumshr", "MarioIcon", "LoadingMarioImage", "Mario_left", "Mario_right", "Tiles", "AnimTiles", "Enemies_left", "Enemies_right", "Bowser_left", "Bowser_right", "Items", "TheEndLvlLine", "Castle", "Barse"})
        s_texture_manager.loadFromFile(texture, textures_dir + texture + ".png");

    for (string texture : {"Sky", "LoadingScen", "Underground", "MenuBackground", "Water"})
        s_texture_manager.loadFromFile(texture, textures_dir + "backgrounds\\" + texture + ".png");

    /// LOAD SOUNDS
    const std::string sounds_dir = MARIO_RES_PATH + "Sounds/";
    for (string sound : {"breakblock", "bump", "coin", "fireball", "jump_super", "kick", "stomp", "powerup_appears",
                         "powerup", "pipe", "flagpole", "bowser_falls", "bowser_fire", "mario_die", "stage_clear", "game_over", "1-up", "warning", "world_clear"})
        s_sound_manager.loadFromFile(sound, sounds_dir + sound + ".wav");

    /// LOAD MUSICS
    const std::string music_dir = MARIO_RES_PATH + "Music/";
    for (string music : {"overworld", "underworld", "bowsercastle", "underwater", "invincibility"})
        s_music_manager.loadFromFile(music, music_dir + music + ".ogg");

    /// LOAD FONTS
    const std::string fonts_dir = MARIO_RES_PATH + "Fonts/";
    for (string font : {"arial", "menu_font", "main_font", "score_font", "some_font"})
        s_font_manager.loadFromFile(font, fonts_dir + font + ".ttf");
}

///-----------
void CMarioGame::setNextlevel()
{
    if (!isItLastLevel())
        m_current_level_index++;
}

///------
std::vector<string> CMarioGame::loadLevelsNames()
{
    fstream file;

    std::vector<string> names_lvls;
    file.open("Maps/Lvls_names.txt", ios::in);

    if (!file.good())
        throw runtime_error("-----------NIE MOGE WCZYTAC MAP!!!----------");

    string name;
    int number_lines = 1;

    while (getline(file, name))
    {
        if (number_lines == 6 || name.size() == 0)
            continue;

        names_lvls.push_back(name);
        number_lines++;
    }

    return names_lvls;
}

///----
void CMarioGame::loadCompletedLvlsNumber()
{
    fstream file;

    file.open("Maps/Completed_lvls_number.txt", ios::in);

    if (file.good())
    {
        file >> m_completed_lvls_number;

        if (m_completed_lvls_number > m_levels_names.size() || m_completed_lvls_number < 0)
            m_completed_lvls_number = 0;
    }
    else
        m_completed_lvls_number = 0;
}

///----
void CMarioGame::writeFrameRate(int &frameCounter, float &secondTimer, float frameTime)
{
    if (secondTimer >= 1)
    {
        cout << frameCounter << endl;
        frameCounter = 0;
        secondTimer = 0;
    }
    else
    {
        secondTimer += frameTime;
        frameCounter++;
    }
}