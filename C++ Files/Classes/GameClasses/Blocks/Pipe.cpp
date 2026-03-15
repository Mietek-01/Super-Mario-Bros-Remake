#include "Pipe.h"

Pipe::Pipe(sf::Vector2f pPos, KindPipe pKindPipe)
    : StaticBlock({183, 532, 66, static_cast<int>(pKindPipe)}, pPos)
{
    mMyFlower = new Flower(this, {pPos.x, pPos.y - GetBounds().height});
    AddNewObject(mMyFlower);
}

///-----
void Pipe::ResetFlower()
{
    if (mMyFlower)
        mMyFlower->Reset();
}
