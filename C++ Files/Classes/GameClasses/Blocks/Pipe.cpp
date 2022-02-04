#include "Pipe.h"

CPipe::CPipe(sf::Vector2f pos,KindPipe kind_pipe)
:CStaticBlock({183,532,66,static_cast<int>(kind_pipe)},pos)
{
    m_my_flower=new CFlower(this,{pos.x,pos.y-getBounds().height});
    addNewObject(m_my_flower);
}

///-----
void CPipe::resetFlower()
{
    if(m_my_flower)
        m_my_flower->reset();
}

