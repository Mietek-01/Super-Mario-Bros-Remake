#include "SpecialEffects.h"
#include "../Scens/Scen.h"
#include "GUI.h"

bool CSpecialEffects::s_deactivation_death_animation=false;

CSpecialEffects::~CSpecialEffects()
{
    if(m_death_animation)
    {
        if(s_deactivation_death_animation)
            m_death_animation.reset();
        else
            CGUI::addGuiObject(m_death_animation.release());
    }
}

///-----------
CSpecialEffects::CSpecialEffects(CAnimator* animator,KindUpdate kind_update,float when_remove,sf::Vector2f force)
:CGuiObject(when_remove)
,m_animator(animator)
,m_kind_update(kind_update)
{
    m_current_pos=m_animator->getPosition();
    m_force=force;
}

///--------
CSpecialEffects::CSpecialEffects(CAnimator* animator,std::function<bool(unique_ptr<CAnimator>&)> special_update)
:m_animator(animator)
,m_kind_update(static_cast<KindUpdate>(-1))
{
   m_special_update=special_update;
   m_current_pos=m_animator->getPosition();
}

///--------------------------
void CSpecialEffects::update()
{
    if(m_rotate)
        m_animator->getSprite().rotate(7);

    m_animator->update(m_current_pos);

    if(m_special_update)
        m_remove=m_special_update(m_animator);
    else
        switch(m_kind_update)
        {
        case KindUpdate::JUMP:
        {
            m_current_pos.x+=m_force.x;

            m_force.y+=2200.0f*CScen::getFrameTime();
            m_current_pos.y+=m_force.y*CScen::getFrameTime();

            if(m_current_pos.y>=m_when_remove)
                m_remove=true;

            break;
        }

        case KindUpdate::ONE_LOOP_ANIMATION:
        {
            CAnimations * animations=dynamic_cast<CAnimations*>(m_animator.get());

            assert(animations);

            if(animations->islastFrame())
                m_remove=true;

            break;
        }

        case KindUpdate::STANDIG:
        {
            if(m_when_remove<=CScen::getDurationScen())
                m_remove=true;

            break;
        }

        case KindUpdate::CONST_MOVE:
        {
            m_current_pos.y+=m_force.y;

            if(m_current_pos.y<=m_when_remove)
                m_remove=true;

            break;
        }
        }
}
