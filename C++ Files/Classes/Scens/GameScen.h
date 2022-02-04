#pragma once

#include <list>
#include "Scen.h"
#include "../GameClasses/Blocks/Block.h"
#include "../GameClasses/Landscape.h"
#include "../GameClasses/Mario.h"
#include "../GameClasses/PhysicalObject.h"

using namespace std;

class CGameScen: public CScen
{
public:

    enum class GamePlayStates
    {
        MAIN_GAME,
        WARNING,
        CHANGING_LVL_MARIO,
        MARIO_DEAD,
        LVL_COMPLETED,
        BOWSER_DEFEAT,
        FIGHT_WITH_BOWSER,
        ENTERING_TO_PIPE,
        LEAVING_UNDERGROUND_WORLD_BY_PIPE,
        RETURN_TO_BEFORE_WORLD
    };

    enum class GameStates
    {
        GAMEPLAY,
        GAME_OVER,
        WIN_GAME
    };

    friend class CEnteringPipe;
    friend class CReturnPipe;

    CGameScen(int,string,string,shared_ptr<std::vector<CLandScape>>,shared_ptr<std::list<unique_ptr<CBlock>>>,
    shared_ptr<std::list<unique_ptr<CPhysicaltObject>>>,shared_ptr<std::list<unique_ptr<CGameObject>>>);

    ~CGameScen();

    ///----METHODS
    void draw(const unique_ptr<sf::RenderWindow>&) override;
    void update() override;

    static void addPoints(int);
    static void resetPoints(){m_points=0;}

    ///-----SETTERY
    void setGameState(GameStates state);
    bool setGamePlayState(GamePlayStates state);
    void marioInCastle(bool valuee){m_mario_in_castle=valuee;}
    void changePositionView(float);

    ///-----GETTERY
    bool isMarioDead()const{return !m_mario||m_mario->iamDead();}
    GamePlayStates getGamePlayState()const{return m_game_play_state;}
    const sf::Vector2f& getMarioPosition()const;
    const shared_ptr<const std::list<unique_ptr<CBlock>>> getBlocks() const{ return m_blocks; }

private:

    enum class LevelCompletedStates
    {
        COUNTING_POINTS_FOR_LVL_TIME,
        COUNTING_POINTS_FOR_LIVES_MARIO,
        NEXT_LEVEL

    }m_level_completed_state=LevelCompletedStates::COUNTING_POINTS_FOR_LVL_TIME;

    static int m_points;

    shared_ptr<std::vector<CLandScape>> m_landscapes;
    shared_ptr<std::list<unique_ptr<CBlock>>> m_blocks;
    shared_ptr<std::list<unique_ptr<CGameObject>>> m_not_physical_objs;
    shared_ptr<std::list<unique_ptr<CPhysicaltObject>>> m_physical_objs;

    vector<CGameObject*> &m_my_removed_objects;
    vector<CGameObject*> &m_my_new_objects;

    /// POTRZERBNE PRZY MECHANIZMIE RUR
    string m_texture_background_name;

    sf::View m_view;
    sf::FloatRect m_bounds_view;
    unique_ptr<CMario> m_mario;

    GameStates m_game_state=GameStates::GAMEPLAY;
    GamePlayStates m_game_play_state=GamePlayStates::MAIN_GAME;

    bool m_bowser_is_dying;
    bool m_mario_in_castle=false;
    bool m_before_warning=true;

    const float m_when_activate_warning;
    const short m_warning_length_time=4;
    const short m_gameover_length_time=8;
    const short m_win_game_length_time=8;
    const short m_mario_dead_length_time=5;
    const short m_bowser_defeat_length_time=2;
    const short m_entering_to_pipe_length_time=2;

    float m_game_time;
    float m_timer;
    float m_sounds_timer=0.0f;

    int m_number_lives_to_count=0;

    ///----METHODS
    inline void setTimer(float time){m_timer=CScen::s_duration_scen+time;}
    inline void setDoubleBoundsView();
    inline void createExtinguishingEfect(string);
    inline void moveCamera();

    inline void updateGameTime();
    inline bool isObjectVisible(const sf::FloatRect&,bool=false);
    inline void gameUpdate();

    inline CBlock* whichHitBlock(CBlock*,CBlock*,const CPhysicaltObject*);
    inline map<CGameObject::KindCollision,CBlock*> howCollisionsWithBlocks(const CPhysicaltObject*,bool &);

    inline void marioBlockCollision();
    inline void physicalObjBlockCollision();
    inline void marioGameObjsCollision();
    inline void gameObjsGameObjsCollisions();

    inline void updateRemovedGameObjects();
    inline void updateNewGameObjects();
    inline void removeNewGameObjects();
};
