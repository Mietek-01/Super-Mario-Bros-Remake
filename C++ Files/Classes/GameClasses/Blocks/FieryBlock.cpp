#include "FieryBlock.h"

CFieryBlock::CFieryBlock(sf::Vector2f pos,const bool right_dir)
:CStaticBlock({0,32,32,32},pos)
{
    CFieryLine::Direction dir;

    if(right_dir)
        dir=CFieryLine::Direction::RIGHT;
    else
        dir=CFieryLine::Direction::LEFT;

    addNewObject(new CFieryLine({pos.x,pos.y-getBounds().height/2.0f},dir));
}

///----------CFieryLine---------///

CFieryBlock::CFieryLine::CFieryLine(sf::Vector2f pos,Direction dir)
:CGameObject(new CSprite(CGUI::createSprite("Tiles",{static_cast<int>(dir),667,14,96},pos,m_scale_to_tile,true)),Parentage::ITEM,pos)
,m_dir(dir)
{
}

///-------
void CFieryBlock::CFieryLine::update()
{
    if(m_dir==Direction::RIGHT)
        m_animator->getSprite().rotate(m_speed);
    else
        m_animator->getSprite().rotate(-m_speed);

    m_animator->update(m_current_position);
}

///------
void CFieryBlock::CFieryLine::actOnObject(CGameObject* obj,KindCollision kind_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        obj->actOnMe(KindAction::HIT);
}

