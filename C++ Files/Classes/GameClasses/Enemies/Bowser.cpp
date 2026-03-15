#include "Bowser.h"
#include "../../Scens/GameScen.h"
#include "../Blocks/MysterBox.h"
#include "../Items/FireballBullets.h"
#include "../Mario.h"
#include "Enemies.h"
#include "../../Scens/LoadingGameScen.h"

#include <ctime>
#include <cmath>
#include <vector>

///------------CreatorEnemies------------------///

Bowser::CreatorEnemies::CreatorEnemies(int pBeginTiles, int pEndTiles)
    : mNumberTiles(abs(pEndTiles - pBeginTiles) / (int)Scene::sTileSize)
    , mFirstTilePosition(pBeginTiles + Scene::sTileSize)
{
    SetWhenNextEnemy();
}

///-----
PhysicalObject* Bowser::CreatorEnemies::GetCreatedEnemy()
{
    if (mWhenCreateEnemy < Scene::GetDurationScene())
    {
        SetWhenNextEnemy();

        int whichTile;

        do
        {
            whichTile = Gui::Rand(mNumberTiles, 0);

        } while (whichTile == mLastRandTile);

        mLastRandTile = whichTile;

        const sf::Vector2f position(whichTile * Scene::sTileSize + mFirstTilePosition, -10);

        if (Gui::Rand(4, 0) == 2)
            return new RedTurtle(position, static_cast<PhysicalObject::KindMovement>(Gui::Rand(2, 0)), true);
        else
            return new Goombas(position, static_cast<PhysicalObject::KindMovement>(Gui::Rand(2, 0)), true);
    }

    return nullptr;
}

///--------
inline void Bowser::CreatorEnemies::SetWhenNextEnemy()
{
    mWhenCreateEnemy = Scene::GetDurationScene() + mCreatingSpeed;
}

///--------------Bowser--------------///

bool Bowser::sInstance = false;

Bowser::~Bowser()
{
    sInstance = false;

    if (!MarioGame::Instance().IsThisScene<GameScene>()) return;

    sf::Sprite* hitObj = new sf::Sprite(mAnimator->GetSprite());

    hitObj->setRotation(180);
    hitObj->setPosition(hitObj->getPosition().x, hitObj->getPosition().y - hitObj->getGlobalBounds().height);

    GuiObject* obj = new SpecialEffects(new SpriteAnimator(hitObj), SpecialEffects::KindUpdate::JUMP, MarioGame::sSizeWindow.y + Scene::sTileSize, {0, -1000});
    Gui::AddGuiObject(obj);

    mBarseToPrincess->Activate();

    AddNewObject(new Princess(mPrincessPosition));
}

///--------
Bowser::Bowser(sf::Vector2f pPos)
    : PhysicalObject(new Animations, Parentage::Enemy, pPos, KindMovement::LeftRun, 1000)
    , Shield(4500)
    , mWhereBarse(400)
    , mLeftBarsePos(mCurrentPosition.x - (mWhereBarse + MarioGame::sSizeWindow.x / 2.0f + 200))
    , mRightBarsePos(mCurrentPosition.x + mWhereBarse)
    , mPrincessPosition({pPos.x + mWhereBarse + MarioGame::sSizeWindow.x / 2.0f + 100, (float)(MarioGame::sSizeWindow.y - Scene::sTileSize * 2)})
{
    if (sInstance && MarioGame::Instance().IsThisScene<LoadingGameScene>())
    {
        std::cout << " MORE THAN ONE BOWSER !!!!!!!!!!!!!!!" << std::endl;
        assert(0);
    }

    sInstance = true;

    this->CreateAnimations();
    mAnimations->Play(Animations::LeftMove, mCurrentPosition);

    mValueAcceleration = mBasicSpeed;

    AddNewObject(new MysteryBox({mLeftBarsePos - (Scene::sTileSize / 2.0f + Scene::sTileSize * 5), (float)(MarioGame::sSizeWindow.y - (5 * Scene::sTileSize))}, MysteryBox::MyItem::Flower));
}

///-------------
void Bowser::Update()
{
    if (!mEnable)
    {
        this->SetTrapForMario();
        PhysicalObject::Update();
    }
    else
    {
        ChangeReversal();

        if (mAttack)
            (this->*mAttack)();

        if (!mAttack && mWhenAttack < Scene::GetDurationScene() && !mChangingReversal)
            SetAttack();

        PhysicalObject::Update();

        mShieldPointer->Update();

        if (mShieldPointer->IsShieldDepleted())
            MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::BowserDefeat);
    }
}

///-----
inline void Bowser::SetAttack(KindsAttacks pWhichAttack)
{
    if (pWhichAttack == KindsAttacks::RANDOM)
        pWhichAttack = static_cast<KindsAttacks>(Gui::Rand(6, 0));

    switch (pWhichAttack)
    {
    case KindsAttacks::SPECIAL_SIN_FIREBALL_ATTACK:
        {
            (this->mAttack) = &Bowser::SpecialSinFireBallAttack;

            if (mIsRightDirReversal)
                mAnimations->Play(MyKindsAnimations::R_SPECIAL_SIN_FIRE_BALL_ATTACK, mCurrentPosition);
            else
                mAnimations->Play(MyKindsAnimations::L_SPECIAL_SIN_FIRE_BALL_ATTACK, mCurrentPosition);

            mAnimations->SetSpeed(mShootingSpeed);

            mKindMovement = KindMovement::Standing;

            mTimer = Scene::GetDurationScene() + mFireballSinLengthAttack;
            break;
        }

    case KindsAttacks::SPECIAL_CIRCLE_FIREBALL_ATTACK:
        {
            (this->mAttack) = &Bowser::SpecialCircleFireBallAttack;

            if (mIsRightDirReversal)
                mAnimations->Play(MyKindsAnimations::R_SPECIAL_CIRCLE_FIRE_BALL_ATTACK, mCurrentPosition);
            else
                mAnimations->Play(MyKindsAnimations::L_SPECIAL_CIRCLE_FIRE_BALL_ATTACK, mCurrentPosition);

            mAnimations->SetSpeed(mShootingSpeed);

            mKindMovement = KindMovement::Standing;

            break;
        }

    case KindsAttacks::BASIC_FIREBALL_ATTACK:
        {
            (this->mAttack) = &Bowser::BasicFireBallAttack;

            if (mIsRightDirReversal)
                mAnimations->Play(MyKindsAnimations::R_BASIC_FIRE_BALL_ATTACK, mCurrentPosition);
            else
                mAnimations->Play(MyKindsAnimations::L_BASIC_FIRE_BALL_ATTACK, mCurrentPosition);

            mAnimations->SetSpeed(mShootingSpeed);

            mKindMovement = KindMovement::Standing;

            break;
        }

    case KindsAttacks::LEAP_ON_MARIO:
        {
            (this->mAttack) = &Bowser::LeapOnMario;

            const float lengthBetweenMarioAndBowser = fabs(mCurrentPosition.x - MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x);

            mMyPosBeforeJump = mCurrentPosition;
            mSecondZeroPlace = lengthBetweenMarioAndBowser;

            if (mIsRightDirReversal)
                mXCoordinate = 0.0f;
            else
                mXCoordinate = mSecondZeroPlace;

            float howHighJump = 150;

            if (lengthBetweenMarioAndBowser > 400)
            {
                howHighJump *= 2;
                mValueAcceleration *= 7.0f;
            } else
                mValueAcceleration *= 5.0f;

            mACoefficient = (4 * howHighJump) / -(mSecondZeroPlace * mSecondZeroPlace);

            Jump();
            mBlockChangingReversal = true;

            break;
        }

    case KindsAttacks::CHARGE:
        {
            mSecondZeroPlace = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x;

            if (fabs(mSecondZeroPlace - mCurrentPosition.x) > 300)
            {
                (this->mAttack) = &Bowser::Charge;
                mValueAcceleration = 5.0f;
                mBlockChangingReversal = true;
            } else
                SetAttack();

            break;
        }

    case KindsAttacks::ENEMIES_RAID:
        {
            (this->mAttack) = &Bowser::EnemiesRaid;

            if (mIsRightDirReversal)
                mAnimations->Play(MyKindsAnimations::L_CHANGING_TO_REVERSAL, mCurrentPosition);
            else
                mAnimations->Play(MyKindsAnimations::R_CHANGING_TO_REVERSAL, mCurrentPosition);

            mKindMovement = KindMovement::Standing;
            mBlockChangingReversal = true;

            mTimer = Scene::GetDurationScene() + mEnemiesRaidLengthAttack;

            const int marioPosition = MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x;

            int beginTiles = marioPosition - mRaidLength;
            int endTiles = marioPosition + mRaidLength;

            if (beginTiles < mLeftBarsePos + Scene::sTileSize * 2)
            {
                beginTiles = mLeftBarsePos + Scene::sTileSize * 2;
                endTiles = beginTiles + mRaidLength * 1.5f;
            }

            if (endTiles > mRightBarsePos - Scene::sTileSize * 2)
            {
                endTiles = mRightBarsePos - Scene::sTileSize * 2;
                beginTiles = endTiles - mRaidLength * 1.5f;
            }

            mCreatorEnemies.reset(new CreatorEnemies(beginTiles, endTiles));

            break;
        }
    }
}

///----
void Bowser::EnemiesRaid()
{
    if (mAnimations->IsLastFrame())
        if (mAnimations->GetAnimationName() == MyKindsAnimations::R_CHANGING_TO_REVERSAL)
            mAnimations->Play(MyKindsAnimations::L_CHANGING_TO_REVERSAL, mCurrentPosition);
        else
            mAnimations->Play(MyKindsAnimations::R_CHANGING_TO_REVERSAL, mCurrentPosition);

    PhysicalObject* newEnemy = mCreatorEnemies->GetCreatedEnemy();

    if (newEnemy)
        AddNewObject(newEnemy);

    if (mTimer < Scene::GetDurationScene())
    {
        SetWhenAttack();
        mCreatorEnemies.reset();
    }
}

///------
void Bowser::Charge()
{
    if ((mIsRightDirReversal && mCurrentPosition.x > mSecondZeroPlace) ||
        (!mIsRightDirReversal && mCurrentPosition.x < mSecondZeroPlace))
    {
        SetWhenAttack();
        mValueAcceleration = mBasicSpeed;
    }
}

///---
void Bowser::BasicFireBallAttack()
{
    if (mAttackInterrupted)
    {
        SetWhenAttack();
        return;
    }

    if (mAnimations->GetIndexFrame() != mTimer)
        mShoot = false;

    mTimer = mAnimations->GetIndexFrame();

    Shoot({1, 2, 3}, 1, KindBullet::BASIC);

    if (mAnimations->IsLastFrame())
        SetWhenAttack();
}

///---
void Bowser::SpecialCircleFireBallAttack()
{
    if (mAttackInterrupted)
    {
        SetWhenAttack();
        return;
    }

    Shoot({1, 3, 5}, 1, KindBullet::CIRCLE_MOVEMENT);

    if (mAnimations->IsLastFrame())
        SetWhenAttack();
}

///------
void Bowser::LeapOnMario()
{
    if (!mIsJump)
    {
        SetWhenAttack();
        mValueAcceleration = mBasicSpeed;
    }
}

///----------
void Bowser::SpecialSinFireBallAttack()
{
    if (mAttackInterrupted)
        if (mChangingReversal)
            return;
        else
        {
            mAttackInterrupted = false;

            const float time = mTimer;

            SetAttack(KindsAttacks::SPECIAL_SIN_FIREBALL_ATTACK);

            mTimer = time;

            if (mTimer < Scene::GetDurationScene()) mTimer + 3.0f;
        }

    Shoot({1, 3}, 3, KindBullet::SIN_MOVEMENT);

    if (mAnimations->IsLastFrame() && mTimer < Scene::GetDurationScene())
        SetWhenAttack();
}

///-----
inline void Bowser::Shoot(const std::vector<int> pForWhichFrameShoot, int pSmallFrame, KindBullet pKindBullet)
{
    bool isItFrameToShoot = false;
    int frameToShoot;

    for (const auto& forWhichFrame : pForWhichFrameShoot)
        if (mAnimations->GetIndexFrame() == forWhichFrame)
        {
            isItFrameToShoot = true;
            frameToShoot = forWhichFrame;
            break;
        }

    if (isItFrameToShoot)
    {
        if (!mShoot)
        {
            mShoot = true;
            sf::Vector2f bulletPos = mCurrentPosition;

            if (frameToShoot != pSmallFrame)
                bulletPos.y -= GetSize().y * 0.7f;
            else
                bulletPos.y -= GetSize().y * 0.2f;

            if (mIsRightDirReversal)
                bulletPos.x += GetSize().x / 2.0f;
            else
                bulletPos.x -= GetSize().x / 2.0f;

            switch (pKindBullet)
            {
            case KindBullet::BASIC:
                AddNewObject(new CBasicBullet(bulletPos, mIsRightDirReversal));
                break;

            case KindBullet::SIN_MOVEMENT:
                AddNewObject(new CSinBullet(bulletPos, mIsRightDirReversal));
                break;

            case KindBullet::CIRCLE_MOVEMENT:
                AddNewObject(new CCircleBullet(bulletPos, mIsRightDirReversal));
                break;
            }
        }
    } else
        mShoot = false;
}

///--------
inline void Bowser::ChangeReversal()
{
    if (mBlockChangingReversal)
        return;

    if (mChangingReversal)
    {
        if (mAnimations->IsLastFrame())
        {
            if (mIsRightDirReversal)
                mAnimations->Play(Animations::RightMove, mCurrentPosition);
            else
                mAnimations->Play(Animations::LeftMove, mCurrentPosition);

            mChangingReversal = false;
        }

        return;
    }

    if (!mIsRightDirReversal && MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x > mCurrentPosition.x)
    {
        mAnimations->Play(MyKindsAnimations::R_CHANGING_TO_REVERSAL, mCurrentPosition);
        mChangingReversal = true;
        mIsRightDirReversal = true;
        mKindMovement = KindMovement::RightRun;

        if (mAttack)
            mAttackInterrupted = true;
    }
    else
    if (mIsRightDirReversal && MarioGame::Instance().GetScene<GameScene>().GetMarioPosition().x < mCurrentPosition.x)
    {
        mAnimations->Play(MyKindsAnimations::L_CHANGING_TO_REVERSAL, mCurrentPosition);
        mChangingReversal = true;
        mIsRightDirReversal = false;
        mKindMovement = KindMovement::LeftRun;

        if (mAttack)
            mAttackInterrupted = true;
    }
}

///------
inline void Bowser::SetTrapForMario()
{
    mEnable = true;

    MarioGame::Instance().GetScene<GameScene>().SetGamePlayState(GameScene::GamePlayStates::FightWithBowser);

    this->SetWhenAttack();

    AddNewObject(new Barse({mLeftBarsePos, (float)MarioGame::sSizeWindow.y}, true, true));

    mBarseToPrincess = new Barse({mRightBarsePos, 0}, false, false);
    AddNewObject(mBarseToPrincess);

    CreateShieldPointer(Gui::CreateRectangleShape({0, 0, 500, 30}, sf::Color::Black, false, true),
        sf::Color::Red, ShieldPointer::KindsOrigin::Right, Gui::CreateSprite("Bowser_right", {49, 2, 84, 80}, {0, 0}, 0.5f, true));
}

///----
inline void Bowser::SetWhenAttack()
{
    if (!mChangingReversal)
        if (mIsRightDirReversal)
        {
            mAnimations->Play(Animations::RightMove, mCurrentPosition);
            mKindMovement = KindMovement::RightRun;
        }
        else
        {
            mAnimations->Play(Animations::LeftMove, mCurrentPosition);
            mKindMovement = KindMovement::LeftRun;
        }

    if (!MarioGame::Instance().GetScene<GameScene>().IsMarioDead())
        mWhenAttack = Scene::GetDurationScene() + rand() % 2 + 1;
    else
        mWhenAttack = Scene::GetDurationScene() + 20;

    (this->mAttack) = nullptr;

    mAttackInterrupted = false;
    mBlockChangingReversal = false;
    mShoot = false;
}

///-----
void Bowser::MakeJump()
{
    if (mAttack == &Bowser::LeapOnMario)
    {
        if (mIsRightDirReversal)
            mXCoordinate += mValueAcceleration;
        else
            mXCoordinate -= mValueAcceleration;

        mCurrentPosition.y = mMyPosBeforeJump.y + (-(mACoefficient * (mXCoordinate - 0) * (mXCoordinate - mSecondZeroPlace)));
    } else
        PhysicalObject::MakeJump();
}

///-------------
void Bowser::ActOnMe(KindAction pHowAction)
{
    if (pHowAction == KindAction::Hit)
        ReduceShield();
}

///-------------
void Bowser::ActOnObject(GameObject* pObj, KindCollision pHowCollision)
{
    if (pObj->GetParentage() == Parentage::Mario)
    {
        Mario* mario = dynamic_cast<Mario*>(pObj);
        mario->SetDamage(mDamageValue);

        pObj->ActOnMe(KindAction::Hit);

        if (mIsJump && !mario->IsJumping())
            pObj->ActOnMe(KindAction::Hop);
    }
}

///------
void Bowser::Draw(const std::unique_ptr<sf::RenderWindow>& pWindow)
{
    mAnimator->Draw(pWindow);

    if (mShieldPointer)
        mShieldPointer->Draw(pWindow, {MarioGame::Instance().GetViewPosition().x + MarioGame::sSizeWindow.x / 2.0f - 40, 20});
}

///---------
inline void Bowser::CreateAnimations()
{
    std::vector<sf::IntRect> frameAnimation;
    frameAnimation.push_back({421, 4, 82, 76});
    frameAnimation.push_back({337, 2, 83, 78});
    frameAnimation.push_back({253, 1, 83, 79});
    frameAnimation.push_back({168, 4, 83, 76});
    frameAnimation.push_back({84, 6, 84, 74});
    frameAnimation.push_back({1, 3, 82, 77});

    mAnimations->Create(Animations::LeftMove, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 1.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({50, 4, 82, 76});
    frameAnimation.push_back({133, 2, 83, 78});
    frameAnimation.push_back({217, 1, 83, 79});
    frameAnimation.push_back({302, 4, 83, 76});
    frameAnimation.push_back({385, 6, 84, 74});
    frameAnimation.push_back({470, 3, 82, 77});

    mAnimations->Create(Animations::RightMove, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 1.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({381, 125, 74, 82});
    frameAnimation.push_back({458, 122, 71, 85});

    mAnimations->Create(MyKindsAnimations::R_CHANGING_TO_REVERSAL, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 3.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({98, 125, 74, 82});
    frameAnimation.push_back({24, 122, 71, 85});

    mAnimations->Create(MyKindsAnimations::L_CHANGING_TO_REVERSAL, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 3.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({266, 85, 84, 82});
    frameAnimation.push_back({184, 80, 80, 87});
    frameAnimation.push_back({95, 82, 84, 78});
    frameAnimation.push_back({0, 90, 91, 69});

    mAnimations->Create(MyKindsAnimations::L_SPECIAL_SIN_FIRE_BALL_ATTACK, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 2.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({203, 85, 84, 82});
    frameAnimation.push_back({289, 80, 80, 87});
    frameAnimation.push_back({374, 82, 84, 78});
    frameAnimation.push_back({462, 90, 91, 69});

    mAnimations->Create(MyKindsAnimations::R_SPECIAL_SIN_FIRE_BALL_ATTACK, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 2.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({467, 286, 84, 78});
    frameAnimation.push_back({368, 295, 91, 69});
    frameAnimation.push_back({276, 288, 92, 76});
    frameAnimation.push_back({191, 277, 81, 87});
    frameAnimation.push_back({112, 267, 67, 97});
    frameAnimation.push_back({6, 277, 81, 86});

    mAnimations->Create(MyKindsAnimations::L_SPECIAL_CIRCLE_FIRE_BALL_ATTACK, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 2.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({2, 286, 84, 78});
    frameAnimation.push_back({94, 295, 91, 69});
    frameAnimation.push_back({185, 288, 92, 76});
    frameAnimation.push_back({281, 277, 81, 87});
    frameAnimation.push_back({374, 267, 67, 97});
    frameAnimation.push_back({466, 277, 81, 86});

    mAnimations->Create(MyKindsAnimations::R_SPECIAL_CIRCLE_FIRE_BALL_ATTACK, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 2.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({277, 185, 83, 82});
    frameAnimation.push_back({182, 186, 91, 81});
    frameAnimation.push_back({97, 175, 80, 92});
    frameAnimation.push_back({19, 167, 63, 100});

    mAnimations->Create(MyKindsAnimations::L_BASIC_FIRE_BALL_ATTACK, MarioGame::sTextureManager["Bowser_left"], frameAnimation, 2.0f, mMyScale);

    ///-----
    frameAnimation.clear();
    frameAnimation.push_back({193, 185, 83, 82});
    frameAnimation.push_back({280, 186, 91, 81});
    frameAnimation.push_back({376, 175, 80, 92});
    frameAnimation.push_back({471, 167, 63, 100});

    mAnimations->Create(MyKindsAnimations::R_BASIC_FIRE_BALL_ATTACK, MarioGame::sTextureManager["Bowser_right"], frameAnimation, 2.0f, mMyScale);
}
