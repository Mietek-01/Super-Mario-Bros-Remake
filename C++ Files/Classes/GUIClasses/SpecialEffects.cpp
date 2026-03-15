#include "SpecialEffects.h"
#include "../Scens/Scen.h"
#include "GUI.h"

using namespace std;

bool SpecialEffects::sIsDeactivationDeathAnimation = false;

SpecialEffects::~SpecialEffects()
{
    if (mDeathAnimation) {
        if (sIsDeactivationDeathAnimation) {
            mDeathAnimation.reset();
        } else {
            Gui::AddGuiObject(mDeathAnimation.release());
        }
    }
}

///-----------
SpecialEffects::SpecialEffects(Animator* pAnimator, KindUpdate pKindUpdate, float pWhenRemove, sf::Vector2f pForce)
    : GuiObject(pWhenRemove)
    , mAnimator(pAnimator)
    , mKindUpdate(pKindUpdate)
{
    mCurrentPos = mAnimator->GetPosition();
    mForce = pForce;
}

///--------
SpecialEffects::SpecialEffects(Animator* pAnimator, std::function<bool(unique_ptr<Animator>&)> pSpecialUpdate)
    : mAnimator(pAnimator)
    , mKindUpdate(static_cast<KindUpdate>(-1))
{
    mSpecialUpdate = pSpecialUpdate;
    mCurrentPos = mAnimator->GetPosition();
}

///--------------------------
void SpecialEffects::Update()
{
    if (mIsRotate) {
        mAnimator->GetSprite().rotate(7);
    }

    mAnimator->Update(mCurrentPos);

    if (mSpecialUpdate) {
        mIsRemove = mSpecialUpdate(mAnimator);
    } else {
        switch (mKindUpdate) {
        case KindUpdate::Jump:
        {
            mCurrentPos.x += mForce.x;

            mForce.y += 2200.0f * Scene::GetFrameTime();
            mCurrentPos.y += mForce.y * Scene::GetFrameTime();

            if (mCurrentPos.y >= mWhenRemove) {
                mIsRemove = true;
            }

            break;
        }

        case KindUpdate::OneLoopAnimation:
        {
            Animations* animations = dynamic_cast<Animations*>(mAnimator.get());

            assert(animations);

            if (animations->IsLastFrame()) {
                mIsRemove = true;
            }

            break;
        }

        case KindUpdate::Standing:
        {
            if (mWhenRemove <= Scene::GetDurationScene()) {
                mIsRemove = true;
            }

            break;
        }

        case KindUpdate::ConstMove:
        {
            mCurrentPos.y += mForce.y;

            if (mCurrentPos.y <= mWhenRemove) {
                mIsRemove = true;
            }

            break;
        }
        }
    }
}
