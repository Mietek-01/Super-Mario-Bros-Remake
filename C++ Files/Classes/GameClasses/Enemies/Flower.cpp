#include "../Blocks/Pipe.h"

#include <ctime>
#include <vector>

const float Pipe::Flower::sHowLongGnawing = 3.5f;
const float Pipe::Flower::sSpeed = 0.9;

Pipe::Flower::CFlower(Pipe* pPipe, sf::Vector2f pPos)
    : GameObject(new Animations, Parentage::Enemy, pPos)
    , mHowLongHiding(rand() % 4 + 3)
    , mMyPipe(pPipe)
    , mPosGnawing(pPipe->GetBounds().top)
{
    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({0, 80, 32, 48});
    frameAnimation.push_back({32, 80, 32, 48});

    mAnimations->Create(MyKindsAnimations::Gnawing, MarioGame::sTextureManager["Enemies_left"], frameAnimation, 1.1f, kScaleToTile);
    mAnimations->Create(MyKindsAnimations::MoveDown, MarioGame::sTextureManager["Enemies_left"], {0, 80, 32, 48}, kScaleToTile);
    mAnimations->Create(MyKindsAnimations::MoveUp, MarioGame::sTextureManager["Enemies_left"], {32, 80, 32, 48}, kScaleToTile);

    mCurrentPosition.y += GetBounds().height;
    mAnimations->Play(MyKindsAnimations::MoveUp, mCurrentPosition);

    mWhenMoveUpper = rand() % 3 + 1;
}

///-------------
void Pipe::Flower::Update()
{
    switch (mState)
    {
    case State::Gnawing:
        {
            if (mTimeGnawing < Scene::GetDurationScene() && mAnimations->IsLastFrame())
            {
                mState = State::MoveDown;
                mAnimations->Play(MyKindsAnimations::MoveDown, mCurrentPosition);
            }

            break;
        }
    case State::Hiding:
        {
            if (mWhenMoveUpper < Scene::GetDurationScene())
            {
                mState = State::MoveUp;
                mEnabled = true;
            }

            break;
        }

    case State::MoveDown:
        {
            mCurrentPosition.y += sSpeed;

            if (GetBounds().top > mPosGnawing)
                Reset();

            break;
        }

    case State::MoveUp:
        {
            mCurrentPosition.y -= sSpeed;

            if (mCurrentPosition.y < mPosGnawing)
            {
                mCurrentPosition.y = mPosGnawing;
                mState = State::Gnawing;
                mTimeGnawing = Scene::GetDurationScene() + sHowLongGnawing;
                mAnimations->Play(MyKindsAnimations::Gnawing, mCurrentPosition);
            }

            break;
        }
    }

    mAnimations->Update(mCurrentPosition);
}

///-------------
void Pipe::Flower::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (!mEnabled)
        return;

    if (pObj->GetParentage() == Parentage::Mario)
        pObj->ActOnMe(KindAction::Hit);
}

///-----
void Pipe::Flower::ActOnMe(KindAction pHowAction)
{
    if (!mEnabled)
        return;

    if (pHowAction == KindAction::Hit)
    {
        CreatePoints();
        RemoveObject(this);
    }
}

///--------
void Pipe::Flower::Reset()
{
    mCurrentPosition.y = mPosGnawing + GetBounds().height;
    mAnimations->Play(MyKindsAnimations::MoveUp, mCurrentPosition);
    mState = State::Hiding;
    mEnabled = false;

    SetWhenMoveUpper();
}

///---------
inline void Pipe::Flower::SetWhenMoveUpper()
{
    mWhenMoveUpper = Scene::GetDurationScene() + mHowLongHiding;
}

///-----------
void Pipe::Flower::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    mAnimator->Draw(pWindow);
    mMyPipe->Draw(pWindow);
}
