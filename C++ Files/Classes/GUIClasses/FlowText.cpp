#include "FlowText.h"

using namespace std;

///------
FlowText::FlowText(sf::Text* pText, float pWhenRemove, float pSpeedChanging)
    : mText(pText)
    , GuiObject(pWhenRemove)
    , mSpeedChangingTransparency(pSpeedChanging)
{
    mColor = mText->getFillColor();

    /// LOWER - transparency decreases
    /// TALLER - transparency increases

    /// Sign is only needed to determine the appropriate option
    if (mSpeedChangingTransparency < 0) {
        mKindChangingTransparency = KindChangingTransparency::Lower;
    } else if (mSpeedChangingTransparency > 0) {
        mKindChangingTransparency = KindChangingTransparency::Taller;
    } else {
        mKindChangingTransparency = KindChangingTransparency::None;
    }
}

///----
FlowText::FlowText(sf::Text* pText, std::function<bool(unique_ptr<sf::Text>&)> pSpecialUpdate, float pSpeedChanging)
    : mText(pText)
    , mSpeedChangingTransparency(pSpeedChanging)
{
    mSpecialUpdate = pSpecialUpdate;
    mColor = mText->getFillColor();

    if (mSpeedChangingTransparency < 0) {
        mKindChangingTransparency = KindChangingTransparency::Lower;
    } else if (mSpeedChangingTransparency > 0) {
        mKindChangingTransparency = KindChangingTransparency::Taller;
    } else {
        mKindChangingTransparency = KindChangingTransparency::None;
    }
}

///--------------
void FlowText::Update()
{
    if (mSpecialUpdate) {
        mIsRemove = mSpecialUpdate(mText);
    } else {
        mText->move(0, -1);
        if (mText->getPosition().y <= mWhenRemove) {
            mIsRemove = true;
        }
    }

    /// Perform the selected kind of transparency change

    switch (mKindChangingTransparency) {
    case KindChangingTransparency::Lower:
        {
            mColor.a = 255 + mValueTransparency;
            mText->setFillColor(mColor);

            mValueTransparency += mSpeedChangingTransparency;

            if (mValueTransparency < -255) {
                mColor.a = 0;
                mText->setFillColor(mColor);
                mKindChangingTransparency = KindChangingTransparency::None;
            }

            break;
        }

    case KindChangingTransparency::Taller:
        {
            mColor.a = 0 + mValueTransparency;
            mText->setFillColor(mColor);

            mValueTransparency += mSpeedChangingTransparency;

            if (mValueTransparency > 255) {
                mColor.a = 255;
                mText->setFillColor(mColor);
                mKindChangingTransparency = KindChangingTransparency::None;
            }

            break;
        }
    }
}
