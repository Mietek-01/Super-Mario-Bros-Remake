#include "Block.h"

using namespace std;

///--------------------CBlock---------------///
CBlock::CBlock(const sf::IntRect &position_texture,const sf::Vector2f &pos,const string name)
:CGameObject(new CSprite(CGUI::createSprite(name,position_texture,pos,m_scale_to_tile,true)),Parentage::BLOCK,pos)
{
}

///------
void CBlock::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    this->corectObjectPositionOnMe(*obj,how_collision);
}

///--------------------CDynamicBlock---------------///
const float CDynamicBlock::s_block_gravitation=2000.0f;

CDynamicBlock::CDynamicBlock(sf::IntRect bounds,sf::Vector2f pos,string name)
:CBlock(bounds,pos,name)
,m_basic_y_positon(pos.y)
,m_force(m_jump_force)
{
}

///-------
void CDynamicBlock::makeJump()
{
    m_previous_position.y=m_current_position.y;

    m_force+=s_block_gravitation*CScen::getFrameTime();
    m_current_position.y+=m_force*CScen::getFrameTime();

    if(m_current_position.y>=m_basic_y_positon)
    {
        m_current_position.y=m_basic_y_positon;
        m_force=m_jump_force;
        m_hit=false;
    }

    m_animator->setPosition(m_current_position);
}

///------
void CDynamicBlock::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    switch(which_action)
    {
        case KindAction::DESTROYED:
        {
            creatRemoveBlock();
            CMarioGame::s_sound_manager.play("breakblock");
            break;
        }

        case KindAction::HIT:
        {
            m_force=m_jump_force;
            m_hit=true;
            break;
        }
    }
}

///------
void CDynamicBlock::creatRemoveBlock()
{
    static const sf::Vector2f forces[]={{1,-300},{-1,-300},{1,-450},{-1,-450}};

    for(int i=0;i<4;i++)
    {
        sf::Sprite *sprite=new sf::Sprite(m_animator->getSprite());

        sprite->setScale(0.5,0.5);
        sprite->setOrigin(sprite->getGlobalBounds().width/2,sprite->getGlobalBounds().height/2);
        sprite->setPosition(m_current_position.x,m_current_position.y-getSize().y/2.0f);

        CSpecialEffects * obj=new CSpecialEffects(new CSprite(sprite),CSpecialEffects::KindUpdate::JUMP,CMarioGame::s_size_window.y+CScen::s_tile_size,forces[i]);
        obj->activeRotate();

        CGUI::addGuiObject(obj);
    }

    removeObject(this);
}
