#include "Animator.h"

using namespace std;

/// -----------------------------
/// Create a single-frame animation entry from one rectangle.
void Animations::Create(int pName, const sf::Texture& pTexture, const sf::IntRect& pRect, float pScale, bool pBottomOrigin)
{
    assert(mAnimations.end() == mAnimations.find(pName));

    sf::Sprite spriteToLoad(pTexture, pRect);

    if (pBottomOrigin) {
        spriteToLoad.setOrigin(pRect.width / 2.0f, pRect.height);
    }

    spriteToLoad.setScale(pScale, pScale);

    mAnimations[pName] = {{spriteToLoad}, 0.0f};

    /// Play by default
    Play(pName, {0, 0});
}

/// -----------------------
/// Create a multi-frame animation entry from a list of rectangles.
void Animations::Create(int pName, const sf::Texture& pTexture, const std::vector<sf::IntRect>& pRects, float pSpeed, float pScale, bool pBottomOrigin)
{
    assert(mAnimations.end() == mAnimations.find(pName));
    assert(pSpeed > 0);

    vector<sf::Sprite> sprites;

    for (const auto& frame : pRects) {
        sf::Sprite spriteToLoad(pTexture, frame);

        if (pBottomOrigin) {
            spriteToLoad.setOrigin(frame.width / 2.0f, frame.height);
        }

        spriteToLoad.setScale(pScale, pScale);
        sprites.push_back(spriteToLoad);
    }

    mAnimations[pName] = {sprites, pSpeed};

    /// Play by default
    Play(pName, {0, 0});
}

/// -------------------
/// Update animation frame and set position.
void Animations::Update(const sf::Vector2f& pPosition)
{
    this->Update();
    mCurrentFrame->setPosition(pPosition);
}

/// -------------------
/// Advance animation frame based on elapsed time.
void Animations::Update()
{
    if (mCountFrames > 1) {
        mTimer += 0.1f;
        if (mTimer >= mCurrentSpeed) {
            mIndexFrame++;
            if (mIndexFrame >= mCountFrames) {
                mIndexFrame = 0;
                mIsLastFrame = true;
            } else {
                mIsLastFrame = false;
            }

            SetCurrentFrame();
            mTimer = 0.0f;
        }
    }
}

/// ---------------------
/// Change playback speed of the current animation.
void Animations::SetSpeed(float pNewSpeed)
{
    assert(pNewSpeed > 0);
    assert(mAnimations.end() != mAnimations.find(mCurrentAnimation));

    mCurrentSpeed = mAnimations[mCurrentAnimation].second = pNewSpeed;
}

/// ---------
/// Switch to a named animation and reset playback state.
void Animations::Play(int pName, const sf::Vector2f& pPosition)
{
    assert(mAnimations.end() != mAnimations.find(pName));

    mCurrentAnimation = pName;
    mIndexFrame = 0;
    mCurrentSpeed = mAnimations[mCurrentAnimation].second;
    mTimer = 0.0f;
    mCountFrames = (mAnimations[mCurrentAnimation].first).size();

    /// If the animation has only one frame, mark it as the last frame
    mIsLastFrame = mCountFrames == 1;

    SetCurrentFrame();

    mCurrentFrame->setPosition(pPosition);
}

/// -----------------------
/// Point mCurrentFrame to the active sprite in the animation map.
inline void Animations::SetCurrentFrame()
{
    mCurrentFrame = &(mAnimations[mCurrentAnimation].first)[mIndexFrame];
}

/// -----------
/// Jump to a specific frame index and reposition.
void Animations::SetIndexFrame(int pNewIndex, const sf::Vector2f& pPosition)
{
    assert(pNewIndex >= 0 && pNewIndex < mCountFrames);

    mIndexFrame = pNewIndex;
    SetCurrentFrame();

    mCurrentFrame->setPosition(pPosition);
}
