#include "MoveableBlock.h"

vector<CMoveableBlock*> CMoveableBlock::s_all_moveable_blocks;

CMoveableBlock::~CMoveableBlock()
{
    s_all_moveable_blocks.erase(std::find(s_all_moveable_blocks.begin(),s_all_moveable_blocks.end(),this));
}

///-----
CMoveableBlock::CMoveableBlock(sf::Vector2f pos,Direction dir)
:CStaticBlock({13,659,90,16},pos)
,m_dir(dir)
,m_basic_position(pos)
{
    s_all_moveable_blocks.push_back(this);
}

///------
void CMoveableBlock::update()
{
    m_previous_position=m_current_position;

    switch(m_dir)
    {
    case Direction::UP:
    {
        m_current_position.y-=m_speed;

        if(m_current_position.y<=m_basic_position.y-m_range)
        {
            m_current_position.y=m_basic_position.y-m_range;
            m_dir=Direction::DOWN;
        }

        break;
    }

    case Direction::DOWN:
    {
        m_current_position.y+=m_speed;

        if(m_current_position.y>=m_basic_position.y+m_range)
        {
            m_current_position.y=m_basic_position.y+m_range;
            m_dir=Direction::UP;
        }

        break;
    }

    case Direction::LEFT:
    {
        m_current_position.x-=m_speed;

        if(m_current_position.x<=m_basic_position.x-m_range)
        {
            m_current_position.x=m_basic_position.x-m_range;
            m_dir=Direction::RIGHT;
        }
        break;
    }

    case Direction::RIGHT:
    {
        m_current_position.x+=m_speed;

        if(m_current_position.x>=m_basic_position.x+m_range)
        {
            m_current_position.x=m_basic_position.x+m_range;
            m_dir=Direction::LEFT;
        }

        break;
    }

    }

    m_animator->setPosition(m_current_position);

    if(m_objects_on_me.size()!=0)
    {
        for(auto it=m_objects_on_me.begin();it!=m_objects_on_me.end();)
        {
            if((*it)->isJumping())
                it=m_objects_on_me.erase(it);
            else
            {
                const sf::Vector2f diference_movement(m_current_position.x-m_previous_position.x,
                                m_current_position.y-m_previous_position.y);

                (*it)->movePosition(diference_movement);

                it++;
            }

        }

    }
}

///---------
void CMoveableBlock::actOnObject(CGameObject* obj,KindCollision kind_collision)
{
    CPhysicaltObject *phy_obj=dynamic_cast<CPhysicaltObject*>(obj);

    if(!phy_obj)
        return;

    for(const auto obj_on_me:m_objects_on_me)
        if(obj_on_me==obj)
            return;

    if(kind_collision==KindCollision::TOP&&phy_obj->isInBottomCollision())
    {
        /// CZYLI GDY PRZYGNIOTE
        if(obj->getParentage()==Parentage::MARIO)
            obj->actOnMe(KindAction::HIT);
        else
            removeObject(obj);
    }else
    {
       this->corectObjectPositionOnMe(*obj,kind_collision);

       if(kind_collision==KindCollision::BOTTOM)
             m_objects_on_me.push_back(phy_obj);
    }
}

///-------
void CMoveableBlock::removeDeadObjOnMoveableBlock(const CPhysicaltObject *obj)
{
    for(auto moveable_block:s_all_moveable_blocks)
    {
        auto &objs_on_moveable_block=moveable_block->m_objects_on_me;
        auto it=std::find(objs_on_moveable_block.begin(),objs_on_moveable_block.end(),obj);

        if(it!=objs_on_moveable_block.end())
            objs_on_moveable_block.erase(it);
    }
}
