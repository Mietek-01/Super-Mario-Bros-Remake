#include "Gun.h"
#include "../../Scens/GameScen.h"

Gun::Gun(sf::Vector2f pPos, KindGun pKindGun)
    : StaticBlock({224, 96, 32, static_cast<int>(pKindGun)}, pPos)
{
    SetWhenShoot();
}

///------
void Gun::Update()
{
    if (mWhenShoot < Scene::GetDurationScene())
    {
        const bool rightDir = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x > mCurrentPosition.x;

        AddNewObject(new GunBullet({mCurrentPosition.x, mCurrentPosition.y - GetBounds().height}, rightDir));

        SetWhenShoot();
    }
}

///----
inline void Gun::SetWhenShoot()
{
    mWhenShoot = Scene::GetDurationScene() + rand() % 4 + 1;
}
