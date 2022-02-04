#pragma once

#include <fstream>
#include <list>

#include "../GameClasses/Landscape.h"
#include "../GameClasses/Blocks/Block.h"
#include "../GameClasses/PhysicalObject.h"
#include "../GameClasses/Blocks/SpecialPipe.h"

#include "Scen.h"

using namespace std;

class CLoadingGameScen:public CScen
{
    enum class KindsObject
    {
        BLOCKS,
        ENEMIES,
        ITEMS,
        LANDSCAPES,
    }m_current_kind_object;

    /// !!! WAZNE MUSI ZACZYNAC SIE OD 2
    enum BLOCKS
    {
       Ground=2,
       Dark_ground,
       Brick,
       Dark_brick,
       Static_brick,
       Dark_static_brick,
       Myster_box_coin,
       Myster_box_mushroom,
       Bridge,
       Metal_block,
       Left_moveable_block,
       Right_moveable_block,
       Up_moveable_block,
       Down_moveable_block,
       Left_fiery_block,
       Right_fiery_block,
       Small_pipe,
       Big_pipe,
       Return_pipe,
       Entering_big_pipe,

       Entering_small_pipe
    };

    enum ENEMIES
    {
        Goombas=2,
        Turtle,
        Red_turtle,
        Armored_turtle,
        Flying_turtle,
        Armed_turtle,
        Creator_red_turtles,

        Bowser
    };

    enum ITEMS
    {
       Coin=2,
       Small_gun,
       Big_gun,
       Trampoline,
       Water_waves,
       Lava_waves,
       Lava_bullet,

       Line_with_flag

    };

    enum LANDSCAPES
    {
        Cloud=2,
        Pink_flower,
        Blue_flower,
        First_small_bush,
        Second_small_bush,
        First_big_bush,
        Second_big_bush,
        First_hurdle,
        Second_hurdle,
        Small_tree,
        Big_Tree,
        Water,

        Lava
    };

    const static float s_loading_duration;

    unique_ptr<sf::Sprite> m_mario_image;

    unique_ptr<sf::Text> m_lvl_text_name;
    unique_ptr<sf::Text> m_number_lives;
    unique_ptr<sf::Text> m_information_about_activate_menu;

    const string m_level_name;
    const string m_information_how_activate_menu="PRESS \"ESC\" TO ACTIVATE MENU IN DURRING GAME";
    const string m_kinds_objects_files_names[4]={"Blocks","Enemies","Items","Landscape"};

    const string m_music_file_name="Music name";
    const string m_background_file_name="Background name";
    const string m_lvl_time_file_name="Lvl time";
    const string m_seting_switching_pipes_file_name="Underground levels names";

    fstream m_file;

    /// DANE KTORE WCZYTUJE
    string m_background_name;
    string m_music_name;
    int m_lvl_time;

    shared_ptr<vector<CLandScape>> m_landscapes;
    shared_ptr<list<unique_ptr<CBlock>>> m_blocks;
    shared_ptr<list<unique_ptr<CGameObject>>> m_not_physical_objs;
    shared_ptr<list<unique_ptr<CPhysicaltObject>>> m_physical_objs;

    vector<pair<sf::Vector2f,CPipe::KindPipe>> m_potentials_entering_pipes;
    vector<CPipe*> m_all_pipes;/// POTRZEBUJE TEN KONTENER DLA STWORZENIA WCHODZACYCH RUR

    ///-- METHODS
    void startLoadingLevel();
    void defineLoadingObject(int&,int&);
    void loadObjects();
    void createObject(int,sf::Vector2f);
    void setAllPipes();

public:

    CLoadingGameScen(string,const CEnteringPipe* =nullptr);
    ~CLoadingGameScen(){}

    void draw(const unique_ptr<sf::RenderWindow>&) override;
    void update() override;
};


