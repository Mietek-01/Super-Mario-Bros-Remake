#include "GameScen.h"
#include "LoadingGameScen.h"
#include "MenuScen.h"

#include "../GameClasses/Items/Coin.h"
#include "../GameClasses/Items/WaterWaves.h"
#include "../GameClasses/Items/LavaWaves.h"
#include "../GameClasses/Blocks/MysterBox.h"
#include "../GameClasses/Blocks/MoveableBlock.h"

#include "../GUIClasses/FlowText.h"
#include "../GUIClasses/GUI.h"
#include "../MarioGame.h"
#include "../GUIClasses/SpecialEffects.h"

int CGameScen::m_points=0;

///------------------
CGameScen::CGameScen(int lvl_time,string music_name,string bcg_name,shared_ptr<std::vector<CLandScape>> landscapes,shared_ptr<std::list<unique_ptr<CBlock>>> blocks,
shared_ptr<std::list<unique_ptr<CPhysicaltObject>>>physical_objs,shared_ptr<std::list<unique_ptr<CGameObject>>>not_physical_objs)
:
CScen(bcg_name)
,m_view(sf::FloatRect(0,0,CMarioGame::s_size_window.x,CMarioGame::s_size_window.y))
,m_my_removed_objects(CGameObject::getRemovedObjects())
,m_my_new_objects(CGameObject::getNewObjects())
,m_when_activate_warning(lvl_time/4.0f)
,m_game_time(lvl_time)
,m_mario(new CMario(sf::Vector2f(s_tile_size*2.5f,CMarioGame::s_size_window.y-(s_tile_size*2))))
,m_texture_background_name(bcg_name)
{
    /// PRZYPISUJE STWORZONE OBIEKTY
    m_landscapes=landscapes;
    m_blocks=blocks;
    m_physical_objs=physical_objs;
    m_not_physical_objs=not_physical_objs;

    ///USTAWIAM GRANICE WIDOKU
    //m_view.zoom(2);
    m_bounds_view.left-=static_cast<int>(s_tile_size*2);
    m_bounds_view.top-=static_cast<int>(s_tile_size*10);
    m_bounds_view.width=CMarioGame::s_size_window.x+s_tile_size*4;
    m_bounds_view.height=CMarioGame::s_size_window.y+s_tile_size*20;

    CMarioGame::s_music_manager.play(music_name);

    updateNewGameObjects();/// DODA OBIEKTY DO KONTENEROW
}

///------
CGameScen::~CGameScen()
{
    /// DESTRUKTOR WYKONUJE SIE PO STWORZENIU LOADINGSCEN
    for(auto obj:m_my_removed_objects)delete obj;
        m_my_removed_objects.clear();

    CGUI::setPositionMainLabels(CMarioGame::s_size_window.x/2.0f);
    CGUI::resetGuiObjects();
    CGUI::setVisibleMainLables(true);

    CMarioGame::instance().resetView();

    CMarioGame::s_sound_manager.stop();
}

///------
void CGameScen::setGameState(GameStates state)
{
    m_game_state=state;

    switch(state)
    {
    ///----
    case GameStates::GAME_OVER:
    {
        setTimer(m_gameover_length_time);

        CMarioGame::s_sound_manager.play("game_over");
        this->createExtinguishingEfect("GAME OVER");

        break;
    }

    ///----
    case GameStates::WIN_GAME:
        {
            setTimer(m_win_game_length_time);

            CMarioGame::s_music_manager.resetAllMusic();
            this->createExtinguishingEfect("YOU WIN !!!");
            CMario::resetLvlMario();
            break;
        }
    }
}

///------------
bool CGameScen::setGamePlayState(GamePlayStates state)
{
    if(isMarioDead())
        return false;

    if(m_game_play_state==GamePlayStates::LVL_COMPLETED)
        return false;

    if(m_game_play_state==GamePlayStates::BOWSER_DEFEAT)
        return false;

    if(m_game_play_state==GamePlayStates::CHANGING_LVL_MARIO&&state!=GamePlayStates::MAIN_GAME)
        return false;

    ///------------//

    if(m_game_play_state==GamePlayStates::CHANGING_LVL_MARIO)
    s_duration_scen-=m_mario->getHowLongChangingLvl();

    m_game_play_state=state;

    switch(state)
    {
    ///-----
    case GamePlayStates::ENTERING_TO_PIPE:
    {
        setTimer(m_entering_to_pipe_length_time);
        CMarioGame::s_music_manager.pause();
        CMarioGame::s_sound_manager.play("pipe");

        break;
    }

        ///-----
    case GamePlayStates::LEAVING_UNDERGROUND_WORLD_BY_PIPE:
    {
        setTimer(m_entering_to_pipe_length_time);
        CMarioGame::s_music_manager.stop();
        CMarioGame::s_sound_manager.play("pipe");

        break;
    }

    ///-----
    case GamePlayStates::MARIO_DEAD:
    {
        CMario::createDeathAnimation(m_mario->getCurrentPosition());

        setTimer(m_mario_dead_length_time);
        CMarioGame::s_music_manager.resetAllMusic();
        CMarioGame::s_sound_manager.play("mario_die");

        break;
    }

    ///----
    case GamePlayStates::BOWSER_DEFEAT:
    {
        setTimer(m_bowser_defeat_length_time);
        CMarioGame::s_music_manager.resetAllMusic();

        m_bowser_is_dying=true;
        break;
    }

    ///---
    case GamePlayStates::FIGHT_WITH_BOWSER:
    {
        /// WAZNE BO POZWALA NA UPDATE WSZYSTKICH OBIEKTOW NAWET JAK NIE SA WIDOCZNE W OKNNIE
        /// A W WALCE Z BOSEM NA TYM MI ZLAERZY
        setDoubleBoundsView();
        CGUI::setVisibleMainLables(false);

        break;
    }

    ///-----
    case GamePlayStates::WARNING:
    {
        CMarioGame::s_music_manager.pause();
        CMarioGame::s_sound_manager.play("warning");
        setTimer(m_warning_length_time);

        m_before_warning=false;

        break;
    }

    ///----
    case GamePlayStates::LVL_COMPLETED:
    {
        CMarioGame::s_music_manager.resetAllMusic();
        CMarioGame::s_sound_manager.play("stage_clear");
        CMarioGame::s_sound_manager.play("flagpole");

        CMario::resetLvlMario();
        CMario::resetLivesMario();

        break;
    }

    }

    return true;
}

///----------------------
void CGameScen::update()
{
    switch(m_game_state)
    {

    ///--------------------------
    case GameStates::GAMEPLAY:
    {
        switch(m_game_play_state)
        {
        ///------
        case GamePlayStates::MAIN_GAME:
        {
            updateGameTime();

            gameUpdate();

            if(m_before_warning)
            {
                if(m_game_time<=m_when_activate_warning)
                    CGameScen::setGamePlayState(GamePlayStates::WARNING);
            }
            else if(m_game_time<=0)
            {
                if(setGamePlayState(CGameScen::GamePlayStates::MARIO_DEAD))
                    m_mario.reset();
            }

            break;
        }

        ///-----
        case GamePlayStates::WARNING:
        {
            updateGameTime();

            gameUpdate();

            if(m_game_play_state==GamePlayStates::WARNING)
            {
                if(m_timer<CScen::s_duration_scen)
                {
                    CMarioGame::s_music_manager.resume();

                    setGamePlayState(GamePlayStates::MAIN_GAME);
                }

            }else
                CMarioGame::s_music_manager.resume();

            break;
        }

        ///-----
        case GamePlayStates::MARIO_DEAD:
        {
            if(m_timer<CScen::s_duration_scen)
            {
                if(CMario::getLivesMario()<=0)
                    CGameScen::setGameState(GameStates::GAME_OVER);
                else
                {
                    removeNewGameObjects();
                    CMarioGame::instance().changeScen(new CLoadingGameScen(CMarioGame::instance().getCurrentLevelName()));
                    CEnteringPipe::resetActivatedPipes();
                    return;
                }
            }else
                gameUpdate();

            break;
        }

        ///----
        case GamePlayStates::ENTERING_TO_PIPE:
        {
            if(m_timer<CScen::s_duration_scen)
                CEnteringPipe::getActivatePipe()->setUndergroundLvl();
            else
                CEnteringPipe::getActivatePipe()->enteringAnimation();

            break;
        }

        ///----
        case GamePlayStates::LEAVING_UNDERGROUND_WORLD_BY_PIPE:
        {
            if(m_timer<CScen::s_duration_scen)
                CEnteringPipe::getActivatePipe()->setBeforeWorld();
            else
                CEnteringPipe::getActivatePipe()->enteringAnimation();

            break;
        }

        ///----
        case GamePlayStates::RETURN_TO_BEFORE_WORLD:
        {
            if(CEnteringPipe::getActivatePipe()->isTheEndEnteringAnimation())
                CEnteringPipe::getActivatePipe()->reasumeGame();
            else
               CEnteringPipe::getActivatePipe()->enteringAnimation();

            break;
        }

        ///----
        case GamePlayStates::CHANGING_LVL_MARIO:
        {
            m_mario->changingLvl();
            break;
        }

        ///-------
        case GamePlayStates::LVL_COMPLETED:
        {
            if(m_mario_in_castle)
            {
                switch(m_level_completed_state)
                {
                case LevelCompletedStates::COUNTING_POINTS_FOR_LVL_TIME:
                {
                    const int is_minute=m_game_time-1;
                    m_game_time-=18.0f*s_frame_time;

                    if(m_game_time<=0)
                    {
                        m_level_completed_state=LevelCompletedStates::COUNTING_POINTS_FOR_LIVES_MARIO;
                        m_game_time=0;
                        setTimer(0.3f);
                    }

                    CGUI::setTime(m_game_time);

                    if((int)m_game_time==is_minute)
                        this->addPoints(50);

                    break;
                }

                case LevelCompletedStates::COUNTING_POINTS_FOR_LIVES_MARIO:
                {
                    if(m_timer<CScen::s_duration_scen)
                    {
                        if(CMario::getLivesMario()-m_number_lives_to_count>0)
                        {
                            setTimer(0.3f);
                            CMarioGame::s_sound_manager.play("kick");
                            addPoints(1000);
                            m_number_lives_to_count++;
                        }
                        else
                        {
                            m_number_lives_to_count=0;
                            setTimer(2.0f);
                            m_level_completed_state=LevelCompletedStates::NEXT_LEVEL;
                        }
                    }

                    break;
                }

                case LevelCompletedStates::NEXT_LEVEL:
                {
                    if(m_timer<CScen::s_duration_scen)
                    {
                        if(CMarioGame::instance().isItLastLevel())
                            setGameState(GameStates::WIN_GAME);
                        else
                        {
                            if(CMarioGame::instance().getCompletedLvlsNumber()==CMarioGame::instance().getLvlIndex())
                                CMarioGame::instance().increaseCompletedLvlsNumber();

                            removeNewGameObjects();

                            CMarioGame::instance().setNextlevel();
                            CMarioGame::instance().changeScen(new CLoadingGameScen(CMarioGame::instance().getCurrentLevelName()));
                            CEnteringPipe::resetActivatedPipes();

                            return;
                        }
                    }

                    break;
                }
                }
            }

            gameUpdate();
            break;
        }

        ///--------
        case GamePlayStates::BOWSER_DEFEAT:
        {
            if(m_bowser_is_dying)
            {
                if(m_timer<CScen::s_duration_scen)
                {
                    m_physical_objs->clear();
                    m_not_physical_objs->clear();

                    setTimer(1.0f);
                    m_bowser_is_dying=false;

                    CMarioGame::s_sound_manager.play("bowser_falls");
                }
                else if(m_timer-0.1f>CScen::s_duration_scen&&m_sounds_timer<CScen::s_duration_scen)
                {
                    CMarioGame::s_sound_manager.play("bump");
                    m_sounds_timer=CScen::s_duration_scen+0.4f;
                }

            }
            else
            {
                if(m_mario)
                {
                    if(m_timer<CScen::s_duration_scen)
                    {
                        CMarioGame::s_sound_manager.play("world_clear");
                        CGUI::setVisibleMainLables(true);

                        m_mario.reset();
                    }
                }
                else gameUpdate();
            }

            break;
        }

        ///-----
        case GamePlayStates::FIGHT_WITH_BOWSER:
        {
            gameUpdate();
            break;
        }

        }

        break;
    }

    ///--------------------------
    case GameStates::GAME_OVER:
    {
        if(m_timer<CScen::s_duration_scen)
        {
            removeNewGameObjects();
            CMarioGame::instance().changeScen(new CMenuScen());
            CEnteringPipe::resetActivatedPipes();
            return;

        }
        else
            gameUpdate();

        break;
    }

    ///--------------------------
    case GameStates::WIN_GAME:
    {
        if(m_timer<CScen::s_duration_scen)
        {
            removeNewGameObjects();
            CMarioGame::instance().changeScen(new CMenuScen());
            CEnteringPipe::resetActivatedPipes();
            return;
        }

        break;
    }
    }
}

///-------------------------
inline map<CGameObject::KindCollision,CBlock*> CGameScen::howCollisionsWithBlocks(const CPhysicaltObject* obj,bool &none_collision)
{
    map<CGameObject::KindCollision,CBlock*> collisions;
    collisions[CGameObject::KindCollision::TOP]=nullptr;
    collisions[CGameObject::KindCollision::BOTTOM]=nullptr;
    collisions[CGameObject::KindCollision::RIGHT_SIDE]=nullptr;
    collisions[CGameObject::KindCollision::LEFT_SIDE]=nullptr;

    for(const auto &block:*m_blocks)
    {
        if(!block->isVisible())continue;

        const CGameObject::KindCollision how_collision=obj->howCollision(*block);

        if(how_collision!=CGameObject::KindCollision::NONE)
        {
            none_collision=false;

            switch(how_collision)
            {
                case CGameObject::KindCollision::TOP:
                {
                   if(!collisions[CGameObject::KindCollision::TOP])
                        collisions[CGameObject::KindCollision::TOP]=block.get();
                   else
                    {
                        /// CZYLI GDY MAM GORNA KOLIZJE Z DWOMA BLOKAMI
                        if(block->isHit())
                            break;
                        else
                        if(collisions[CGameObject::KindCollision::TOP]->isHit())
                            collisions[CGameObject::KindCollision::TOP]=block.get();
                        else
                            collisions[CGameObject::KindCollision::TOP]=whichHitBlock(collisions[CGameObject::KindCollision::TOP],block.get(),obj);
                    }

                   break;
                }

                case CGameObject::KindCollision::BOTTOM:
                {
                   if(obj->isInBottomCollision()&&!(block->isHit()||block->iamDead()))break;

                   if(!collisions[CGameObject::KindCollision::BOTTOM])
                        collisions[CGameObject::KindCollision::BOTTOM]=block.get();
                   else
                        collisions[CGameObject::KindCollision::BOTTOM]=whichHitBlock(collisions[CGameObject::KindCollision::BOTTOM],block.get(),obj);

                   break;
                }

                case CGameObject::KindCollision::RIGHT_SIDE:
                {
                    if(!collisions[CGameObject::KindCollision::RIGHT_SIDE])
                        collisions[CGameObject::KindCollision::RIGHT_SIDE]=block.get();

                    break;
                }

                case CGameObject::KindCollision::LEFT_SIDE:
                {
                    if(!collisions[CGameObject::KindCollision::LEFT_SIDE])
                        collisions[CGameObject::KindCollision::LEFT_SIDE]=block.get();

                    break;
                }

            }
        }
    }

    return collisions;
}

///-----------------
inline void CGameScen::physicalObjBlockCollision()
{
    for(auto &obj:*m_physical_objs)
    {
        if(!obj->isVisible())continue;

        bool none_collision=true;
        map<CGameObject::KindCollision,CBlock*> collisions_with_blocks=howCollisionsWithBlocks(obj.get(),none_collision);
        obj->physicGameWithBlock(collisions_with_blocks,none_collision);
    }
}

///--------------------
inline void CGameScen::marioBlockCollision()
{
    bool none_collision=true;
    map<CGameObject::KindCollision,CBlock*> collisions_with_blocks=howCollisionsWithBlocks(m_mario.get(),none_collision);
    m_mario->physicGameWithBlock(collisions_with_blocks,none_collision);
}

///-----------------
inline void CGameScen::marioGameObjsCollision()
{
    for(auto &obj:*m_not_physical_objs)
    {
        if(!obj->isVisible())continue;

        const CGameObject::KindCollision how_collision=m_mario->howCollision(*obj);

        if(how_collision!=CGameObject::KindCollision::NONE)
            obj->actOnObject(m_mario.get(),how_collision);
    }

    ///-------
    for(auto &obj:*m_physical_objs)
    {
        if(!obj->isVisible())continue;

        const CGameObject::KindCollision how_collision=m_mario->howCollision(*obj);

        if(how_collision!=CGameObject::KindCollision::NONE)
            obj->actOnObject(m_mario.get(),how_collision);
    }
}

///------------

inline void CGameScen::gameObjsGameObjsCollisions()
{
    ///FIZYCZNE Z FIZYCZNYMI
    for(auto &first_phy_obj:*m_physical_objs)
    {
        if(!first_phy_obj->isVisible())continue;

        for(auto &second_phy_obj:*m_physical_objs)
        {
            if(!second_phy_obj->isVisible())continue;

            if(first_phy_obj!=second_phy_obj)
            {
               CGameObject::KindCollision how_collision=second_phy_obj->howCollision(*first_phy_obj);

               if(how_collision!=CGameObject::KindCollision::NONE)
                   first_phy_obj->actOnObject(second_phy_obj.get(),how_collision);
            }
        }
    }

    ///FIZYCZNE Z NIEFIZYCZNYMI
    for(auto &phy_obj:*m_physical_objs)
    {
        if(!phy_obj->isVisible())continue;

        for(auto &not_phy_obj:*m_not_physical_objs)
        {
            if(!not_phy_obj->isVisible())continue;

            CGameObject::KindCollision how_collision=phy_obj->howCollision(*not_phy_obj);

            if(how_collision!=CGameObject::KindCollision::NONE)
                not_phy_obj->actOnObject(phy_obj.get(),how_collision);

            /// REAKCJA DZIALA W DWIE STRONY
            how_collision=not_phy_obj->howCollision(*phy_obj);

            if(how_collision!=CGameObject::KindCollision::NONE)
                phy_obj->actOnObject(not_phy_obj.get(),how_collision);
        }
    }
}

///------------------------
void CGameScen::draw(const unique_ptr<sf::RenderWindow>& window)
{
    window->setView(m_view);
    window->draw(m_background);

    for(auto &obj:*m_blocks)
    if(isObjectVisible(obj->getBounds(),true))
    {
        obj->setVisible(true);
        obj->draw(window);

    }else obj->setVisible(false);

    for(auto &obj:*m_landscapes)
    if(isObjectVisible(obj.getBounds()))
        obj.draw(window);

    for(auto &obj:*m_not_physical_objs)
    if(isObjectVisible(obj->getBounds()))
    {
        obj->setVisible(true);
        obj->draw(window);

    }else obj->setVisible(false);

    for(auto &obj:*m_physical_objs)
    if(isObjectVisible(obj->getBounds()))
    {
        obj->setVisible(true);
        obj->draw(window);

    }else obj->setVisible(false);

    if(m_mario)
        m_mario->draw(window);
}

///--------------------
inline void CGameScen::gameUpdate()
{
    /// WYWOLANIE STATYCZNYCH METOD
    CMysterBox::updateStaticAnimation();
    CCoin::updateStaticAnimation();
    CWaterWaves::updateStaticAnimation();
    CLavaWaves::updateStaticAnimation();

    /// UPDATE OBIEKTOW

    if(m_mario)
    {
        moveCamera();
        m_mario->update();
    }

    for(auto &obj:*m_blocks)
    if(obj->isVisible())obj->update();

    for(auto &obj:*m_physical_objs)
    if(obj->isVisible())obj->update();

    for(auto &obj:*m_not_physical_objs)
    if(obj->isVisible())obj->update();

    /// SPRAWDZANIE KOLIZJI Z BLOCKAMI
    if(m_mario)
        marioBlockCollision();

    physicalObjBlockCollision();

    /// SPRAWDZANIE KOLIZJI MIEDZY OBJEKTAMI
    if(m_mario)
        marioGameObjsCollision();

    gameObjsGameObjsCollisions();

    /// USUWANIE OBIEKTOW I DODAWANIE NOWYCH
    if(m_my_removed_objects.size()!=0)
    updateRemovedGameObjects();

    if(m_my_new_objects.size()!=0)
    updateNewGameObjects();
}

///-----------------------
inline void CGameScen::updateNewGameObjects()
{
    /// OKRESLAM DO KTOREGO KONTENERA OBIEKT WRZUCIC
    for(auto *new_obj:m_my_new_objects)
    {
        CBlock * new_block=dynamic_cast<CBlock*>(new_obj);

        if(new_block)
            m_blocks->push_front(unique_ptr<CBlock>(new_block));
        else
        {
            CPhysicaltObject * new_phy_obj=dynamic_cast<CPhysicaltObject*>(new_obj);

            if(new_phy_obj)
                m_physical_objs->push_back(unique_ptr<CPhysicaltObject>(new_phy_obj));
            else
                m_not_physical_objs->push_back(unique_ptr<CGameObject>(new_obj));
        }
    }

    m_my_new_objects.clear();
}

///------------------------
inline void CGameScen::updateRemovedGameObjects()
{
    /// OKRESLAM Z KTOREGO KONTENERA USUNAC
    for(auto *removed_obj:m_my_removed_objects)
    {
        CBlock * removed_block=dynamic_cast<CBlock*>(removed_obj);

        if(removed_block)
            m_blocks->erase(find_if(m_blocks->begin(),m_blocks->end(),[removed_block](const unique_ptr<CBlock> &obj)
            {return removed_block==obj.get();}));
        else
        {
            CMario * removed_mario=dynamic_cast<CMario*>(removed_obj);

            if(removed_mario)
                m_mario.reset();
            else
            {
                CPhysicaltObject * removed_phys_obj=dynamic_cast<CPhysicaltObject*>(removed_obj);

                if(removed_phys_obj)
                {
                    CMoveableBlock::removeDeadObjOnMoveableBlock(removed_phys_obj);

                    m_physical_objs->erase(find_if(m_physical_objs->begin(),m_physical_objs->end(),[removed_phys_obj](const unique_ptr<CPhysicaltObject> &obj)
                    {return removed_phys_obj==obj.get();}));

                }else
                    m_not_physical_objs->erase(find_if(m_not_physical_objs->begin(),m_not_physical_objs->end(),[removed_obj](const unique_ptr<CGameObject> &obj)
                    {return removed_obj==obj.get();}));
            }
        }
    }

    m_my_removed_objects.clear();
}

///-------- MNIEJ WAZNE METODY---------///

inline void CGameScen::moveCamera()
{
    if(m_mario->getCurrentPosition().x>=CMarioGame::s_size_window.x/2)
    {
        float road=m_mario->getCurrentPosition().x-m_mario->getPreviousPosition().x;

        m_background.move(road,0);
        m_bounds_view.left+=road;
        m_view.move(road,0);

        CGUI::moveMainLabels(road);

    }else
    {
        m_view.setCenter(sf::Vector2f(CMarioGame::s_size_window.x/2,CMarioGame::s_size_window.y/2));
        m_background.setPosition(0,0);
        CGUI::setPositionMainLabels(CMarioGame::s_size_window.x/2.0f);
    }
}

///-----------
inline CBlock* CGameScen::whichHitBlock(CBlock* first_block,CBlock* second_block,const CPhysicaltObject* obj)
{
    /// BARDZO WAZNE
    if(first_block->getCurrentPosition().x>second_block->getCurrentPosition().x)
        swap(first_block,second_block);

    if(obj->getCurrentPosition().x<=first_block->getCurrentPosition().x+first_block->getSize().x/2.0f)
        return first_block;
    else
        return second_block;
}

///----------
inline bool CGameScen::isObjectVisible(const sf::FloatRect& bounds,bool larger_bounds)
{
    if(larger_bounds)/// POZWALA WCZESNIEJ DANY OBIEKT ZACZAC RYSOWAC
        return bounds.intersects(sf::FloatRect(m_bounds_view.left-CScen::s_tile_size*2,m_bounds_view.top,m_bounds_view.width+CScen::s_tile_size*4,m_bounds_view.height));
    else
        return bounds.intersects(m_bounds_view);
}

///----
void CGameScen::setDoubleBoundsView()
{
    m_bounds_view.left-=static_cast<int>(CMarioGame::s_size_window.x);
    m_bounds_view.width+=CMarioGame::s_size_window.x*2;
}

///-----------
void CGameScen::addPoints(int points)
{
    m_points+=points;
    CGUI::setPoints(m_points);
}

///-----
inline void CGameScen::updateGameTime()
{
    m_game_time-=s_frame_time;
    CGUI::setTime(m_game_time);
}

///--------
void CGameScen::changePositionView(float valuee)
{
    m_background.move(valuee,0);
    m_view.move(valuee,0);
    m_bounds_view.left+=valuee;
    CGUI::moveMainLabels(valuee);
}

///-------------
const sf::Vector2f& CGameScen::getMarioPosition()const
{
    if(m_mario)
        return m_mario->getCurrentPosition();
    else
        return m_view.getCenter();
}

///--------
inline void CGameScen::createExtinguishingEfect(string name)
{
    /// SCIEMNIANIE TLA
    sf::Sprite *sprite=CGUI::createSprite("LoadingScen",sf::Vector2f(CMarioGame::instance().getWindow()->getView().getCenter().x,
    CMarioGame::s_size_window.y),1,true);

    sprite->setColor(sf::Color::Black);

    std::function<bool(std::unique_ptr<CAnimator>&)> sprite_action =[value_transparency=0.0f](std::unique_ptr<CAnimator>& animator)mutable
    {
        sf::Color color(animator->getSprite().getColor());

        if(value_transparency<255)
        {
            color.a=0+value_transparency;
            value_transparency+=0.5f;
        }
        else
            color.a=255;

        animator->getSprite().setColor(color);

        return (!CMarioGame::instance().isThisScen<CGameScen>());
    };

    CGUI::addGuiObject(new CSpecialEffects(new CSprite(sprite),sprite_action));

    /// POJAWIANIE SIE NAPISU
    sf::Text *text=CGUI::createText(name,m_view.getCenter(),sf::Color::White,"menu_font",true,50);

    std::function<bool(std::unique_ptr<sf::Text>&)> text_action =[](std::unique_ptr<sf::Text>& text)
    {
        return (!CMarioGame::instance().isThisScen<CGameScen>());
    };

    CGUI::addGuiObject(new CFlowText(text,text_action,0.5f));
}

///------------
inline void CGameScen::removeNewGameObjects()
{
    for (auto obj : m_my_new_objects)delete obj;
    m_my_new_objects.clear();
}
