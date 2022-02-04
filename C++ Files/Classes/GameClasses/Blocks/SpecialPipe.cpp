#include "SpecialPipe.h"

#include "../../Scens/GameScen.h"
#include "../../Scens/LoadingGameScen.h"

#include "../Mario.h"

vector<CEnteringPipe*> CEnteringPipe::s_activate_entering_pipes;

///-----------CEnteringPipe---------------///
CEnteringPipe::CEnteringPipe(const pair<sf::Vector2f,CPipe::KindPipe> &data_pipe,string underground_lvl_name,CPipe* return_pipe)
:CPipe(data_pipe.first,data_pipe.second)
,m_underground_lvl_name(underground_lvl_name)
,m_return_pipe(return_pipe)
{

}

///----
CEnteringPipe::CEnteringPipe(const pair<sf::Vector2f,CPipe::KindPipe> &data_pipe,string underground_lvl_name)
:CPipe(data_pipe.first,data_pipe.second)
,m_underground_lvl_name(underground_lvl_name)
{
    m_return_pipe=this;
}

///-----
void CEnteringPipe::update()
{
    if(!m_active)
        return;

    CGameScen &game_scen=CMarioGame::instance().getScen<CGameScen>();

    /// MUSZE TAK BO INACZEJ NIE DZIALA
    const float entry_pos = m_current_position.y - getBounds().height;
    
    if(!game_scen.isMarioDead())
    if(game_scen.m_mario->iAmCrouching())
    if(game_scen.getMarioPosition().y> entry_pos&&
       game_scen.getMarioPosition().x>=m_current_position.x-m_entering_range&&
       game_scen.getMarioPosition().x<=m_current_position.x+m_entering_range)
    if(game_scen.setGamePlayState(CGameScen::GamePlayStates::ENTERING_TO_PIPE))
    {
        setEnteringToPipeAnimation(false);
        s_activate_entering_pipes.push_back(this);
    }
}

///------
void CEnteringPipe::setBeforeWorld()
{
    CGameScen &game_scen=CMarioGame::instance().getScen<CGameScen>();

    assert(game_scen.m_game_play_state==CGameScen::GamePlayStates::LEAVING_UNDERGROUND_WORLD_BY_PIPE);

    /// PRZYWRACAM DAWNE OBIEKTY NADRZEDNEGO POZIOMU I USUWAM TE Z OBECNEGO

    m_landscapes.swap(game_scen.m_landscapes);
    m_landscapes.reset();

    ///------
    m_blocks.swap(game_scen.m_blocks);
    m_blocks.reset();

    ///----
    m_not_physical_objs.swap(game_scen.m_not_physical_objs);
    m_not_physical_objs.reset();

    ///----
    m_physical_objs.swap(game_scen.m_physical_objs);
    m_physical_objs.reset();

    ///---
    game_scen.m_background.setTexture(CMarioGame::s_texture_manager[m_background_lvl_name]);
    game_scen.m_texture_background_name=m_background_lvl_name;

    ///- USTAWIAM WIDOK I GRANICE WIDOKU
    float new_x_center;

    if(m_return_pipe->getCurrentPosition().x>CMarioGame::s_size_window.x/2.0f)
        new_x_center=m_return_pipe->getCurrentPosition().x;
    else
        new_x_center=CMarioGame::s_size_window.x/2.0f;

    game_scen.m_view.setCenter(new_x_center,CMarioGame::s_size_window.y/2.0f);
    game_scen.m_bounds_view.left=new_x_center-game_scen.m_bounds_view.width/2.0f;
    game_scen.m_background.setPosition(new_x_center-CMarioGame::s_size_window.x/2.0f,0);

    CGUI::setPositionMainLabels(new_x_center);

    /// ROBIE TAK ZAMIAST METODA BO TAM JEST WARUNEK KTORY UNIEMOZLIWI ZMIANE STANU
    game_scen.m_game_play_state=CGameScen::GamePlayStates::RETURN_TO_BEFORE_WORLD;
    CMarioGame::s_music_manager.resume();

    setLeavingPipeAnimation();

    /// WAZNE USTAWIAM MOJA POZYCJE TAK BY BYLA WIDOCZNA W OKNIE 
    /// BY MOC RUSOWAC ANIMACJE WYCHODZENIA. POZNIEJ PRZYWRACAM PIERWOTNA POZYCJE
    if(m_return_pipe!=this&&!CMarioGame::instance().getScen<CGameScen>().isObjectVisible(getBounds()))
    {
        m_current_position.x=m_return_pipe->getCurrentPosition().x;
        m_current_position.y=0;
        m_animator->setPosition(m_current_position);
    }

    /// BY KWIATEK NIE BYL RYSOWANY
    m_return_pipe->resetFlower();
}

///----
void CEnteringPipe::setUndergroundLvl()
{
    CGameScen &game_scen=CMarioGame::instance().getScen<CGameScen>();

    assert(game_scen.m_game_play_state==CGameScen::GamePlayStates::ENTERING_TO_PIPE);

    /// ZAPISUJE OBIEKTY GLOWNEGO POZIOMU
    game_scen.m_landscapes.swap(m_landscapes);
    game_scen.m_blocks.swap(m_blocks);
    game_scen.m_not_physical_objs.swap(m_not_physical_objs);
    game_scen.m_physical_objs.swap(m_physical_objs);

    const float game_time = game_scen.m_game_time;
    m_duration_scen = CGameScen::s_duration_scen;
    m_background_lvl_name = game_scen.m_texture_background_name;

    /// WCZYTUJE PODZIEMNY POZIOM I USTAWIAM NOWE DANE W GAMESCENIE

    string lvl_name=CMarioGame::instance().getCurrentLevelName()+"/";

    for(auto pipe:s_activate_entering_pipes)
        lvl_name+=(pipe->m_underground_lvl_name)+"/";

    CLoadingGameScen(lvl_name,this);/// TWORZE CHWILOWY OBIEKT BO CHCE BY MI TYLKO ZALADOWALO OBIEKTY

    /// USTAWIAM RESZTE DANYCH

    CMarioGame::instance().getScen<CGameScen>().m_mario->setPosition({CScen::s_tile_size*2.5f,0});
    CMarioGame::instance().getScen<CGameScen>().m_game_time=game_time;

    m_mario_entering_animation.reset();
}

///------
void CEnteringPipe::reasumeGame()
{
    CGameScen &game_scen=CMarioGame::instance().getScen<CGameScen>();

    assert(game_scen.m_game_play_state==CGameScen::GamePlayStates::RETURN_TO_BEFORE_WORLD);

    CGameScen::s_duration_scen=m_duration_scen;

    game_scen.m_mario.reset(new CMario({m_return_pipe->getCurrentPosition().x,m_return_pipe->getBounds().top}));

    game_scen.m_game_play_state=CGameScen::GamePlayStates::MAIN_GAME;

    m_mario_entering_animation.reset();

    m_active=false;

    s_activate_entering_pipes.pop_back();

    /// BY KWIATEK NIE UDERZYL W MARIA
    m_return_pipe->resetFlower();

    /// USTAWAIAM MOJA POPRZEDNIA PIERWOTNA POZYCJE 
    if(m_return_pipe!=this)
    {
        m_current_position=m_previous_position;
        m_animator->setPosition(m_current_position);
    }
}

///-------
void CEnteringPipe::setLeavingPipeAnimation()
{
    if(CMario::getLevelMario()==CMario::LevelMario::BIG_MARIO)
        m_mario_entering_animation.reset(CGUI::createSprite("Mario_right",{0,38,32,58},{0,0},CGameObject::m_scale_to_tile,true));
    else
        m_mario_entering_animation.reset(CGUI::createSprite("Mario_right",{4,96,24,32},{0,0},CGameObject::m_scale_to_tile,true));

    m_mario_entering_animation->setPosition(m_return_pipe->getCurrentPosition().x,m_return_pipe->getBounds().top+m_mario_entering_animation->getGlobalBounds().height);
    m_end_entering_animation=false;
    m_up_entering_animation=true;

    m_end_animation_pos = m_return_pipe->getBounds().top;

}

///------
void CEnteringPipe::setEnteringToPipeAnimation(bool up_animation)
{
    const CMario *mario=CMarioGame::instance().getScen<CGameScen>().m_mario.get();

    const sf::Vector2f mario_pos=mario->getCurrentPosition();
    const bool right_reversal_mario=mario->isInRightReversal();

    if(CMario::getLevelMario()==CMario::LevelMario::BIG_MARIO)
        if(!up_animation)
        {
            if(right_reversal_mario)
                m_mario_entering_animation.reset(CGUI::createSprite("Mario_right",{192,50,32,46},mario_pos,CGameObject::m_scale_to_tile,true));
            else
                m_mario_entering_animation.reset(CGUI::createSprite("Mario_left",{287,50,32,46},mario_pos,CGameObject::m_scale_to_tile,true));
        }else
        {
            if(right_reversal_mario)
                m_mario_entering_animation.reset(CGUI::createSprite("Mario_right",{0,38,32,58},mario_pos,CGameObject::m_scale_to_tile,true));
            else
                m_mario_entering_animation.reset(CGUI::createSprite("Mario_left",{480,38,32,58},mario_pos,CGameObject::m_scale_to_tile,true));
        }
    else
        if(right_reversal_mario)
            m_mario_entering_animation.reset(CGUI::createSprite("Mario_right",{4,96,24,32},mario_pos,CGameObject::m_scale_to_tile,true));
        else
            m_mario_entering_animation.reset(CGUI::createSprite("Mario_left",{484,96,24,32},mario_pos,CGameObject::m_scale_to_tile,true));


    removeObject(CMarioGame::instance().getScen<CGameScen>().m_mario.get());
    m_end_entering_animation=false;
    m_up_entering_animation=up_animation;

    if(up_animation)
        m_end_animation_pos=m_current_position.y-1;
    else
        m_end_animation_pos=this->getBounds().top + 1;
}

///------
void CEnteringPipe::enteringAnimation()
{
    if(!m_mario_entering_animation||m_end_entering_animation)
        return;

    if(m_up_entering_animation)
    {
        if(m_mario_entering_animation->getPosition().y>m_end_animation_pos)
            m_mario_entering_animation->move(0,-m_entering_speed);
        else
            m_end_entering_animation=true;
    }else
    {
        if(m_mario_entering_animation->getGlobalBounds().top<m_end_animation_pos)
            m_mario_entering_animation->move(0,m_entering_speed);
        else
            m_end_entering_animation=true;
    }
}

///------
CEnteringPipe* CEnteringPipe::getActivatePipe()
{
    if(s_activate_entering_pipes.size()==0)
        return nullptr;

    return s_activate_entering_pipes[s_activate_entering_pipes.size()-1];
}

///------
void CEnteringPipe::resetActivatedPipes()
{
    for(auto activate_pipe:s_activate_entering_pipes)
    {
        auto &blocks_container=activate_pipe->m_blocks;

        /// JEST TO SYTUACJA GDY USUWAM W MOMENCIE WCHODZENIA DO RURY
        /// WTEDY AKTYWUJE RURE ALE TAKA RURA NIE MA JESZCZE ZAPISANEGO POZIOMU
        if(!blocks_container)
            continue;

        auto my_pos_in_container=find_if(blocks_container->begin(),blocks_container->end(),
                             [activate_pipe](const unique_ptr<CBlock>& block){return block.get()==activate_pipe;});

        /// BARDZO WAZNE BO ZAPOBIEGA PODWOJNEMU USUNIECIU
        my_pos_in_container->release();
        delete activate_pipe;/// USUWAJAC RURE USUWAM ZAPISANY W NIEJ POZIOM!!!
    }

    s_activate_entering_pipes.clear();
}

///------
void CEnteringPipe::draw(const unique_ptr<sf::RenderWindow>& window)
{
    if(m_mario_entering_animation)
    {
        window->draw(*m_mario_entering_animation);

        /// MUSZE NADRYSOWYWAC RURE NA ANIMACJE
        if(m_return_pipe!=this&&CMarioGame::instance().getScen<CGameScen>().m_game_play_state==CGameScen::GamePlayStates::RETURN_TO_BEFORE_WORLD)
            m_return_pipe->draw(window);
    }

    m_animator->draw(window);
}

///-----------CReturnPipe---------------///
CReturnPipe::CReturnPipe(sf::Vector2f pos)
:CStaticBlock({187,366,64,128},{pos.x-CScen::s_tile_size/2.0f,pos.y})
{
}

///-----
void CReturnPipe::update()
{
    CGameScen &game_scen=CMarioGame::instance().getScen<CGameScen>();

    if(CEnteringPipe::getActivatePipe())
    if(!game_scen.isMarioDead())
    if(game_scen.m_mario->isJumping())
    if(game_scen.m_mario->getBounds().top-1<m_current_position.y&&
       game_scen.getMarioPosition().x>=m_current_position.x-m_entering_range&&
       game_scen.getMarioPosition().x<=m_current_position.x+m_entering_range)
    if(game_scen.setGamePlayState(CGameScen::GamePlayStates::LEAVING_UNDERGROUND_WORLD_BY_PIPE))
    {
        /// UZYWAM MECHANIZMU ANIMACJI Z WCHODZACEJ RURY
        CEnteringPipe::getActivatePipe()->setEnteringToPipeAnimation(true);
    }
}

///--------
void CReturnPipe::draw(const unique_ptr<sf::RenderWindow>&window)
{
    if(CEnteringPipe::getActivatePipe())/// UZYWAM MECHANIZMU ANIMACJI Z WCHODZACEJ RURY, TUTAJ TYLKO RYSUJE
    if(CEnteringPipe::getActivatePipe()->m_mario_entering_animation)
        window->draw(*(CEnteringPipe::getActivatePipe()->m_mario_entering_animation));

    m_animator->draw(window);
}


