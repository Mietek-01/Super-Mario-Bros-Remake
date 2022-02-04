#pragma once
#include "Pipe.h"

#include "../Landscape.h"
#include "../PhysicalObject.h"

#include <list>

///------
class CEnteringPipe:public CPipe
{
    /// POZWALA UPDATOWAC ROZGRYWKE GDY MARIO WCHODZI WYCHODZI Z RURY
    static vector<CEnteringPipe*> s_activate_entering_pipes;

    const string m_underground_lvl_name;
    CPipe* m_return_pipe;

    const float m_entering_range=15.0f;
    const float m_entering_speed=0.5f;

    bool m_active=true;
    bool m_end_entering_animation=false;
    bool m_up_entering_animation;
    float m_end_animation_pos;

    std::unique_ptr<sf::Sprite> m_mario_entering_animation;

    ///------DANE POTRZEBNE DO ZAPISU POZIOMU-----///

    shared_ptr<std::vector<CLandScape>> m_landscapes;
    shared_ptr<std::list<unique_ptr<CBlock>>> m_blocks;
    shared_ptr<std::list<unique_ptr<CGameObject>>> m_not_physical_objs;
    shared_ptr<std::list<unique_ptr<CPhysicaltObject>>> m_physical_objs;

    string m_background_lvl_name;

    float m_duration_scen;

    void setEnteringToPipeAnimation(bool);
    void setLeavingPipeAnimation();

    friend class CReturnPipe;

public:

    CEnteringPipe(const pair<sf::Vector2f,CPipe::KindPipe>&,string,CPipe*);
    CEnteringPipe(const pair<sf::Vector2f,CPipe::KindPipe>&,string);

    ~CEnteringPipe(){}

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&)override;

    void setUndergroundLvl();
    void setBeforeWorld();
    void reasumeGame();

    void enteringAnimation();

    bool isTheEndEnteringAnimation()const {return m_end_entering_animation;}
    bool isVisible()const override {return true;}

    static CEnteringPipe* getActivatePipe();
    static void resetActivatedPipes();

};

///--------
class CReturnPipe:public CStaticBlock
{
    const float m_entering_range=15;

public:

    CReturnPipe(sf::Vector2f);
    ~CReturnPipe(){}

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&)override;

};


