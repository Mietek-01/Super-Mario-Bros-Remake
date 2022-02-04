#include "Gun.h"
#include "../../Scens/GameScen.h"

CGun::CGun(sf::Vector2f pos,KindGun kind_gun)
:CStaticBlock({224,96,32,static_cast<int>(kind_gun)},pos)
{
    setWhenShoot();
}

///------
void CGun::update()
{
    if(m_when_shoot<CScen::getDurationScen())
    {
        const bool right_dir=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x>m_current_position.x;

        addNewObject(new CBullet({m_current_position.x,m_current_position.y-getBounds().height},right_dir));

        setWhenShoot();
    }
}

///----
inline void CGun::setWhenShoot()
{
    m_when_shoot=CScen::getDurationScen()+rand()%4+1;
}

