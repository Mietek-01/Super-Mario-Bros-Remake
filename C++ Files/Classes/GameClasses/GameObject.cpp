#include "GameObject.h"
#include "../Scens/GameScen.h"
#include "../GUIClasses/FlowText.h"

#include <math.h>

using namespace std;

vector<GameObject*> GameObject::sRemovedObjects;
vector<GameObject*> GameObject::sNewObjects;

const float GameObject::kScaleToTile = 48.0f / 32.0f;

void GameObject::AddNewObject(GameObject* pObject) {
    sNewObjects.push_back(pObject);
}

///------
void GameObject::RemoveObject(GameObject* pObjectToRemove) {
    /// Prevents adding duplicates
    for (auto obj : sRemovedObjects)
        if (obj == pObjectToRemove)
            return;

    pObjectToRemove->mIsDead = true;
    sRemovedObjects.push_back(pObjectToRemove);
}

///------------------GameObject------------------///
GameObject::GameObject(Animator* pAnimator, Parentage pParentage, const sf::Vector2f& pPosition, int pPoints)
:
mAnimator(pAnimator)
, mValuePoints(pPoints)
, mParentage(pParentage)
, mAnimations(dynamic_cast<Animations*>(pAnimator))
{
    mPreviousPosition = mCurrentPosition = pPosition;
}

///---
void GameObject::CorrectObjectPositionOnMe(GameObject& pObject, KindCollision pCollision) const {
    switch (pCollision) {
        case KindCollision::Bottom: {
            pObject.SetPosition({pObject.GetCurrentPosition().x, mCurrentPosition.y - GetSize().y + 0.1f});
            break;
        }

        case KindCollision::Top: {
            pObject.SetPosition({pObject.GetCurrentPosition().x, mCurrentPosition.y + pObject.GetBounds().height + 0.5f});
            break;
        }

        case KindCollision::RightSide: {
            pObject.SetPosition({((mCurrentPosition.x - GetSize().x / 2.0f) - pObject.GetBounds().width / 2.0f) - 0.2f, pObject.GetCurrentPosition().y});
            break;
        }

        case KindCollision::LeftSide: {
            pObject.SetPosition({(mCurrentPosition.x + GetSize().x / 2.0f + pObject.GetBounds().width / 2.0f) + 0.2f, pObject.GetCurrentPosition().y});
            break;
        }
    }
}

///------
GameObject::KindCollision GameObject::SpecialCheckingHowCollision(const GameObject& pObject) const {
    const sf::FloatRect& objPrevBounds = pObject.GetPreviousBounds();

    /// When previous position of object overlaps with my current position
    if (this->GetBounds().intersects(objPrevBounds)) {
        if (this->mPreviousPosition.y < objPrevBounds.top)
            return KindCollision::Bottom;
        else
        if ((this->mPreviousPosition.y - this->GetSize().y) > objPrevBounds.top + objPrevBounds.height)
            return KindCollision::Top;
        else {
            if (this->mPreviousPosition.x < objPrevBounds.left)
                return KindCollision::RightSide;
            else
                return KindCollision::LeftSide;
        }

    } else {
        const KindCollision howCollision = pObject.HowCollision(*this);

        switch (howCollision) {
            case KindCollision::Bottom: return KindCollision::Top;
            case KindCollision::Top: return KindCollision::Bottom;
            case KindCollision::LeftSide: return KindCollision::RightSide;
            case KindCollision::RightSide: return KindCollision::LeftSide;
        }
    }
}

///-----
GameObject::KindCollision GameObject::HowCollision(const GameObject& pObject) const {
    /// Returns what collision I have with the passed object

    const sf::FloatRect& objBounds = pObject.GetBounds();

    if (this->GetBounds().intersects(objBounds)) {
        /// This is the pessimistic case
        /// because previous position overlaps with current position of object
        if (this->GetPreviousBounds().intersects(objBounds))
            return SpecialCheckingHowCollision(pObject);
        else
        if (this->mPreviousPosition.y < objBounds.top)
            return KindCollision::Bottom;
        else
        if ((this->mPreviousPosition.y - this->GetSize().y) > objBounds.top + objBounds.height)
            return KindCollision::Top;
        else {
            if (mPreviousPosition.x < objBounds.left)
                return KindCollision::RightSide;
            else
                return KindCollision::LeftSide;
        }

    } else return KindCollision::None;
}

///---
void GameObject::CreatePoints() const {
    sf::Text* text = Gui::CreateText(Gui::ToString(mValuePoints), {0, 0}, sf::Color::Red);
    text->setPosition(mCurrentPosition.x, mCurrentPosition.y - this->GetBounds().height / 2.0f);

    Gui::AddGuiObject(new FlowText(text, mCurrentPosition.y - GetBounds().height / 2.0f - Scene::sTileSize * 3));

    GameScene::AddPoints(mValuePoints);
}

///------
void GameObject::CreateBeatObjectEffect(float pHopForce) {
    sf::Sprite* hitObj = new sf::Sprite(mAnimator->getSprite());

    hitObj->setRotation(180);
    hitObj->setPosition(hitObj->getPosition().x, hitObj->getPosition().y - hitObj->getGlobalBounds().height);

    GuiObject* obj = new SpecialEffects(new SpriteAnimator(hitObj), SpecialEffects::Jump, MarioGame::sSizeWindow.y + Scene::sTileSize, {0, pHopForce});
    Gui::AddGuiObject(obj);

    CreatePoints();
    MarioGame::sSoundManager.play("stomp");

    RemoveObject(this);
}

///------------
void GameObject::CheckUnderMap() {
    if (mCurrentPosition.y > MarioGame::sSizeWindow.y + Scene::sTileSize * 3) {
        this->ActOnMe(KindAction::UnderMap);
        RemoveObject(this);
    }
}

///------
void GameObject::MovePosition(sf::Vector2f pValue) {
    mCurrentPosition.x += pValue.x;
    mCurrentPosition.y += pValue.y;
    mAnimator->setPosition(mCurrentPosition);
}

///------
sf::FloatRect GameObject::GetPreviousBounds() const {
    return {mPreviousPosition.x - GetBounds().width / 2.0f,
    mPreviousPosition.y - GetBounds().height, GetBounds().width, GetBounds().height};
}

///------------------
void GameObject::Draw(const unique_ptr<sf::RenderWindow>& pWindow) {
    mAnimator->draw(pWindow);
}
