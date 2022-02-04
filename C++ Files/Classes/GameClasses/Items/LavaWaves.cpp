#include "LavaWaves.h"

CAnimations CLavaWaves::s_static_animator;

CLavaWaves::CLavaWaves(sf::Vector2f pos)
:CGameObject(new CSprite(CGUI::createSprite("Tiles",{0,0,32,32},{pos.x,pos.y+1},m_scale_to_tile,true)),Parentage::ITEM,pos)
{

}

///------
void CLavaWaves::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        obj->actOnMe(KindAction::HIT);
    else
        removeObject(obj);
}

///-----
void CLavaWaves::setStaticAnimation()
{
    vector<sf::IntRect> m_frames_animation;

    m_frames_animation.push_back(sf::IntRect(0,64,32,32));
    m_frames_animation.push_back(sf::IntRect(32,64,32,32));
    m_frames_animation.push_back(sf::IntRect(64,64,32,32));
    m_frames_animation.push_back(sf::IntRect(96,64,32,32));

    s_static_animator.create(CAnimations::STANDARD,CMarioGame::s_texture_manager["AnimTiles"],m_frames_animation,2.0f,m_scale_to_tile,true);
}

///------
void CLavaWaves::draw(const unique_ptr<sf::RenderWindow>& window)
{
    s_static_animator.setPosition(m_current_position);
    s_static_animator.draw(window);
}


