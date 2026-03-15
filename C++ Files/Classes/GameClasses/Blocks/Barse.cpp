#include "../Enemies/Bowser.h"

Bowser::Barse::Barse(sf::Vector2f pPos, bool pActive, bool pGoUp)
    : StaticBlock({0, 0, 32, 720}, pPos, "Barse")
    , mActive(pActive)
    , mGoUp(pGoUp)
{
    mCurrentPosition.y += GetBounds().height;
    mPreviousPosition = mCurrentPosition;

    mAnimator->SetPosition(mCurrentPosition);
}

///--------
void Bowser::Barse::Update()
{
    if (mActive)
    {
        mPreviousPosition = mCurrentPosition;

        if (mGoUp)
        {
            mCurrentPosition.y -= mMoveSpeed;

            if (mCurrentPosition.y - GetBounds().height <= 0)
            {
                mCurrentPosition.y = GetBounds().height;
                mActive = false;
            }
        }
        else
        {
            mCurrentPosition.y += mMoveSpeed;

            if (mCurrentPosition.y - GetBounds().height >= GetBounds().height)
            {
                mCurrentPosition.y = GetBounds().height * 2.0f;
                mActive = false;
            }
        }

        mAnimator->SetPosition(mCurrentPosition);
    }
}
