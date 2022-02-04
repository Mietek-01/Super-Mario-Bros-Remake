#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

using namespace std;

class CScen
{
protected:

    static float s_duration_scen;
    static float s_frame_time;

    sf::Sprite m_background;

public:

    static const size_t s_tile_size;

    ///METHODS
    virtual ~CScen(){}
    CScen(std::string);

    virtual void draw(const unique_ptr<sf::RenderWindow>&)=0;
    virtual void update() =0;

    static void updateFrameTime(float time){s_duration_scen+=time;s_frame_time=time;}

    static float getFrameTime(){return s_frame_time;}
    static float getDurationScen(){return s_duration_scen;}
};

