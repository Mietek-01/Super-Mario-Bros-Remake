#include "WaterWaves.h"

CAnimations CWaterWaves::s_static_animator;

CWaterWaves::CWaterWaves(sf::Vector2f pos)
:CGameObject(new CSprite(CGUI::createSprite("Tiles",{0,0,32,32},pos,m_scale_to_tile,true)),Parentage::ITEM,pos)
{

}

///-----
void CWaterWaves::setStaticAnimation()
{
    vector<sf::IntRect> m_frames_animation;

    m_frames_animation.push_back(sf::IntRect(0,32,32,32));
    m_frames_animation.push_back(sf::IntRect(32,32,32,32));
    m_frames_animation.push_back(sf::IntRect(64,32,32,32));
    m_frames_animation.push_back(sf::IntRect(96,32,32,32));

    s_static_animator.create(CAnimations::STANDARD,CMarioGame::s_texture_manager["AnimTiles"],m_frames_animation,2.0f,m_scale_to_tile,true);
}

///------
void CWaterWaves::draw(const unique_ptr<sf::RenderWindow>& window)
{
    s_static_animator.setPosition(m_current_position);
    s_static_animator.draw(window);
}

