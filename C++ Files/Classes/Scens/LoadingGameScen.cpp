#include "LoadingGameScen.h"

#include "../GUIClasses/GUI.h"
#include "GameScen.h"
#include "../MarioGame.h"

#include "../GameClasses/Blocks/Blocks.h"
#include "../GameClasses/Enemies/Enemies.h"
#include "../GameClasses/Items/Items.h"
#include "../GameClasses/Mario.h"

#include <windows.h>

const float CLoadingGameScen::s_loading_duration=1.5f;

CLoadingGameScen::CLoadingGameScen(string lvl_name,const CEnteringPipe* entering_pipe)
:
CScen("LoadingScen")
,m_level_name(lvl_name)
,m_landscapes(new std::vector<CLandScape>)
,m_blocks(new std::list<unique_ptr<CBlock>>)
,m_physical_objs(new std::list<unique_ptr<CPhysicaltObject>>)
,m_not_physical_objs(new std::list<unique_ptr<CGameObject>>)
{
    startLoadingLevel();

    if(entering_pipe)
        CMarioGame::instance().changeScen(new CGameScen(m_lvl_time,m_music_name,m_background_name,m_landscapes,m_blocks,m_physical_objs,m_not_physical_objs));
    else
    {
        CGUI::setWorldName(lvl_name);

        m_mario_image.reset(CGUI::createSprite("LoadingMarioImage",{0,0,50,90},{0,0},1.2f,true));
        m_mario_image->setPosition({CMarioGame::s_size_window.x/2.0f,CMarioGame::s_size_window.y/2.0f+m_mario_image->getGlobalBounds().height/2.0f});

        ///--- TEXTS

        const sf::Vector2f pos_texts={m_mario_image->getPosition().x,m_mario_image->getPosition().y-m_mario_image->getGlobalBounds().height*1.5f};

        m_information_about_activate_menu.reset(CGUI::createText(m_information_how_activate_menu,{pos_texts.x,pos_texts.y+CMarioGame::s_size_window.y-400},sf::Color::White,"menu_font",true,30));

        m_lvl_text_name.reset(CGUI::createText(lvl_name,pos_texts,sf::Color::White,"menu_font",true,30));

        m_number_lives.reset(CGUI::createText("x"+CGUI::toString(CMario::getLivesMario()),{pos_texts.x+60,pos_texts.y+m_mario_image->getGlobalBounds().height+35},sf::Color::White,"menu_font",true));
    }
}

void CLoadingGameScen::startLoadingLevel()
{
    /// WCZYTUJE NAZWE TLA
    m_file.open("Maps/"+m_level_name+"/"+m_background_file_name+".txt",ios::in);
    assert(m_file.good());
    m_file>>m_background_name;

    m_file.close();

    /// WCZYTUJE NAZWE MUZYKI
    m_file.open("Maps/"+m_level_name+"/"+m_music_file_name+".txt",ios::in);
    assert(m_file.good());
    m_file>>m_music_name;

    m_file.close();

    /// WCZYTUJE CZAS POZIOMU

    m_file.open("Maps/"+CMarioGame::instance().getCurrentLevelName()+"/"+m_lvl_time_file_name+".txt",ios::in);
    assert(m_file.good());
    m_file>>m_lvl_time;

    assert(m_lvl_time>=60&&m_lvl_time<=240);

    m_file.close();

    /// WCZYTUJE OBIEKTY GRY
    for(int i=0;i<4;i++)
    {
        string name_file="Maps/"+m_level_name+"/"+m_kinds_objects_files_names[i]+".txt";

        m_file.open(name_file,ios::in);
        assert(m_file.good());

        m_current_kind_object=static_cast<KindsObject>(i);

        loadObjects();

        m_file.close();

    }

    setAllPipes();
}

///------
void CLoadingGameScen::defineLoadingObject(int& id,int& how_many)
{
    m_file>>id;
    if(id==0)return;

    assert(id>=0&&id<=50000);

    if(id>100)
    {
        for(int i=2000;i<100000;i+=1000)
        if(id<i)
        {
            how_many=id-(i-1000);/// NAWIAS BARDZO WAZNY
            id=i/1000;
            id--;
            break;
        }
    }
}

///----------
void CLoadingGameScen::loadObjects()
{
    for(int y=0;y<15;++y)
    {

    int id=0,x=0;

    while(true)
    {
        int m_how_many=1;
        defineLoadingObject(id,m_how_many);

        if(id==0)break;

        switch(id)
        {
            case 1:
            {
                for(int i=0;i<m_how_many;++i)
                    x++;

                break;
            }

            default:
            {
                for(int i=0;i<m_how_many;++i,++x)
                    createObject(id,sf::Vector2f(x*s_tile_size,y*s_tile_size));

                break;
            }
        }
    }

    }
}

///--------
void CLoadingGameScen::setAllPipes()
{
    for(auto pipe:m_all_pipes)
        m_blocks->push_back(unique_ptr<CPipe>(pipe));

    if(m_potentials_entering_pipes.size()==0)
        return;

    m_file.open("Maps/"+m_level_name+"/"+m_seting_switching_pipes_file_name+".txt",ios::in);

    if(m_file.good())
    {
        /// POZWALA ZACHOWAC ODPOWIEDNIA KOLEJNOS PRZY OKRESLANIU Z KTOREJ RURY WYJSC
        if(m_all_pipes.size()>=2)
            std::sort(m_all_pipes.begin(),m_all_pipes.end(),[](const CPipe * l_pipe,const CPipe* r_pipe)
                {return l_pipe->getCurrentPosition().x<r_pipe->getCurrentPosition().x;});

        auto which_entering_pipe=m_potentials_entering_pipes.begin();

        while(!m_file.eof()&&which_entering_pipe!=m_potentials_entering_pipes.end())
        {
            string underground_lvl_name;
            int return_pipe;

            m_file>>underground_lvl_name;
            m_file>>return_pipe;

            if(return_pipe==-1)/// CZYLI WCHODZACA RURA BD TEZ POWROTNA
                m_blocks->push_back(unique_ptr<CBlock>(new CEnteringPipe(*which_entering_pipe,underground_lvl_name)));
            else
            if(return_pipe<0||return_pipe>=m_all_pipes.size())
            {
                cout<<" ---- BLEDNE DANE OKRESLAJACE RURY"<<endl;
                assert(0);
            }else
                m_blocks->push_back(unique_ptr<CBlock>(new CEnteringPipe(*which_entering_pipe,underground_lvl_name,m_all_pipes[return_pipe])));

            which_entering_pipe++;
        }
    }else
    {
        cout<<"--- NIE MA PLIKU OKRESLAJACEGO PRZENOSZACE RURY-------"<<endl;
        assert(0);
    }

    m_file.close();
}

///------
void CLoadingGameScen::update()
{
    if(s_duration_scen>=s_loading_duration)
        CMarioGame::instance().changeScen(new CGameScen(m_lvl_time,m_music_name,m_background_name,m_landscapes,m_blocks,m_physical_objs,m_not_physical_objs));
}

///-----
void CLoadingGameScen::draw(const unique_ptr<sf::RenderWindow>& window)
{
    window->draw(m_background);
    window->draw(*m_lvl_text_name);
    window->draw(*m_number_lives);
    window->draw(*m_information_about_activate_menu);
    window->draw(*m_mario_image);
}

///-------
void CLoadingGameScen::createObject(int id,sf::Vector2f pos)
{
    pos={pos.x+CScen::s_tile_size/2.0f,pos.y+CScen::s_tile_size};

    switch(m_current_kind_object)
    {
    case KindsObject::BLOCKS:
        {
            switch(id)
            {

            case BLOCKS::Ground:
            m_blocks->push_back(unique_ptr<CBlock>(new CStaticBlock({0,0,32,32},pos)));break;

            case BLOCKS::Dark_ground:
            m_blocks->push_back(unique_ptr<CBlock>(new CStaticBlock({58,477,32,32},pos)));break;

            case BLOCKS::Brick:
            m_blocks->push_back(unique_ptr<CBlock>(new CDynamicBlock({32,0,32,32},pos)));break;

            case BLOCKS::Dark_brick:
            m_blocks->push_back(unique_ptr<CBlock>(new CDynamicBlock({15,521,32,32},pos)));break;

            case BLOCKS::Static_brick:
            m_blocks->push_back(unique_ptr<CBlock>(new CStaticBlock({96,0,32,32},pos)));break;

            case BLOCKS::Dark_static_brick:
            m_blocks->push_back(unique_ptr<CBlock>(new CStaticBlock({13,477,32,32},pos)));break;

            case BLOCKS::Bridge:
            m_blocks->push_back(unique_ptr<CBlock>(new CStaticBlock({224,0,32,32},pos)));break;

            case BLOCKS::Metal_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CStaticBlock({32,32,32,32},pos)));break;

            case BLOCKS::Myster_box_coin:
            m_blocks->push_back(unique_ptr<CBlock>(new CMysterBox(pos,CMysterBox::MyItem::COIN)));break;

            case BLOCKS::Myster_box_mushroom:
            m_blocks->push_back(unique_ptr<CBlock>(new CMysterBox(pos,CMysterBox::MyItem::MUSHROOM)));break;

            case BLOCKS::Big_pipe:
            m_all_pipes.push_back(new CPipe(pos,CPipe::KindPipe::BIG));break;

            case BLOCKS::Small_pipe:
            m_all_pipes.push_back(new CPipe(pos,CPipe::KindPipe::SMALL));break;

            case BLOCKS::Entering_big_pipe:
            m_potentials_entering_pipes.push_back({pos,CPipe::KindPipe::BIG});break;

            case BLOCKS::Entering_small_pipe:
            m_potentials_entering_pipes.push_back({pos,CPipe::KindPipe::SMALL});break;

            case BLOCKS::Return_pipe:/// CHODZI O KOLEJNOSC RYSOWANIA
            m_blocks->push_front(unique_ptr<CBlock>(new CReturnPipe(pos)));break;

            case BLOCKS::Left_fiery_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CFieryBlock(pos,false)));break;

            case BLOCKS::Right_fiery_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CFieryBlock(pos,true)));break;

            case BLOCKS::Left_moveable_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CMoveableBlock(pos,CMoveableBlock::Direction::LEFT)));break;

            case BLOCKS::Right_moveable_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CMoveableBlock(pos,CMoveableBlock::Direction::RIGHT)));break;

            case BLOCKS::Up_moveable_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CMoveableBlock(pos,CMoveableBlock::Direction::UP)));break;

            case BLOCKS::Down_moveable_block:
            m_blocks->push_back(unique_ptr<CBlock>(new CMoveableBlock(pos,CMoveableBlock::Direction::DOWN)));break;

            }

            break;
        }

    case KindsObject::ENEMIES:
        {
            switch(id)
            {

            case ENEMIES::Goombas:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CGoombas(pos)));break;

            case ENEMIES::Turtle:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CTurtle(pos)));break;

            case ENEMIES::Red_turtle:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CRedTurtle(pos)));break;

            case ENEMIES::Armored_turtle:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CArmoredTurtle(pos)));break;

            case ENEMIES::Flying_turtle:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CFlyingTurtle(pos)));break;

            case ENEMIES::Armed_turtle:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CArmedTurtle(pos)));break;

            case ENEMIES::Bowser:
            m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new CBowser(pos)));break;

            case ENEMIES::Creator_red_turtles:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CCreatorRedTurltes(pos)));break;

            }

            break;
        }
    case KindsObject::ITEMS:
        {
            switch(id)
            {
            case ITEMS::Coin:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CCoin(pos)));break;

            case ITEMS::Line_with_flag:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CLineWithFlag(pos)));break;

            case ITEMS::Small_gun:
            m_blocks->push_back(unique_ptr<CBlock>(new CGun(pos,CGun::KindGun::SMALL)));break;

            case ITEMS::Big_gun:
            m_blocks->push_back(unique_ptr<CBlock>(new CGun(pos,CGun::KindGun::BIG)));break;

            case ITEMS::Trampoline:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CTrampoline(pos)));break;

            case ITEMS::Water_waves:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CWaterWaves(pos)));break;

            case ITEMS::Lava_waves:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CLavaWaves(pos)));break;

            case ITEMS::Lava_bullet:
            m_not_physical_objs->push_back(unique_ptr<CGameObject>(new CLavaBullet(pos)));break;

            }

            break;
        }

    case KindsObject::LANDSCAPES:
        {
            switch(id)
            {

            case LANDSCAPES::Cloud:
            m_landscapes->push_back(CLandScape(pos,{82,160,65,42}));break;

            case LANDSCAPES::Big_Tree:
            m_landscapes->push_back(CLandScape(pos,{0,160,32,96},2));break;

            case LANDSCAPES::Small_tree:
            m_landscapes->push_back(CLandScape(pos,{32,160,32,32}));break;

            case LANDSCAPES::Pink_flower:
            m_landscapes->push_back(CLandScape(pos,{160,225,30,30}));break;

            case LANDSCAPES::Blue_flower:
            m_landscapes->push_back(CLandScape(pos,{225,225,30,30}));break;

            case LANDSCAPES::First_hurdle:
            m_landscapes->push_back(CLandScape(pos,{224,160,32,32}));break;

            case LANDSCAPES::Second_hurdle:
            m_landscapes->push_back(CLandScape(pos,{160,175,32,16}));break;

            case LANDSCAPES::First_big_bush:
            m_landscapes->push_back(CLandScape(pos,{1,362,97,42}));break;

            case LANDSCAPES::Second_big_bush:
            m_landscapes->push_back(CLandScape(pos,{7,414,89,45}));break;

            case LANDSCAPES::First_small_bush:
            m_landscapes->push_back(CLandScape(pos,{82,225,61,30}));break;

            case LANDSCAPES::Second_small_bush:
            m_landscapes->push_back(CLandScape(pos,{192,262,32,26}));break;

            case LANDSCAPES::Water:
            m_landscapes->push_back(CLandScape(pos,{32,220,32,32}));break;

            case LANDSCAPES::Lava:
            m_landscapes->push_back(CLandScape(pos,{64,320,32,32}));break;

            }

            break;
        }
    }
}
