#include "Scen.h"
#include "../MarioGame.h"

///STATICS

const size_t CScen::s_tile_size=720/15;/// WYSOKOSC OKNA PRZEZ LICZBE KAFLI W PIONIE
float CScen::s_duration_scen=0.0f;
float CScen::s_frame_time=0.0f;

CScen::CScen(std::string name_bcg)
{
    s_duration_scen=0.0f;
    m_background.setTexture(CMarioGame::s_texture_manager[name_bcg]);
}
