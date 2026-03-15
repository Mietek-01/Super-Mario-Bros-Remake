#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Scen.h"
#include "../GameClasses/Blocks/Block.h"
#include "../GameClasses/Landscape.h"
#include "../GameClasses/Mario.h"
#include "../GameClasses/PhysicalObject.h"

class GameScene : public Scene {
public:

    enum class GamePlayStates {
        MainGame,
        Warning,
        ChangingLevelMario,
        MarioDead,
        LevelCompleted,
        BowserDefeat,
        FightWithBowser,
        EnteringToPipe,
        LeavingUndergroundWorldByPipe,
        ReturnToBeforeWorld
    };

    enum class GameStates {
        Gameplay,
        GameOver,
        WinGame
    };

    friend class EnteringPipe;
    friend class ReturnPipe;

    GameScene(int pLevelTime, std::string pMusicName, std::string pBackgroundName,
        std::shared_ptr<std::vector<Landscape>> pLandscapes,
        std::shared_ptr<std::list<std::unique_ptr<Block>>> pBlocks,
        std::shared_ptr<std::list<std::unique_ptr<PhysicalObject>>> pPhysicalObjs,
        std::shared_ptr<std::list<std::unique_ptr<GameObject>>> pNotPhysicalObjs);

    ~GameScene();

    ///----METHODS
    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) override;
    void Update() override;

    static void AddPoints(int pPoints);
    static void ResetPoints() { mPoints = 0; }

    ///-----SETTERS
    void SetGameState(GameStates pState);
    bool SetGamePlayState(GamePlayStates pState);
    void MarioInCastle(bool pValue) { mMarioInCastle = pValue; }
    void ChangePositionView(float pValue);

    ///-----GETTERS
    bool IsMarioDead() const { return !mMario || mMario->IsDead(); }
    GamePlayStates GetGamePlayState() const { return mGamePlayState; }
    const sf::Vector2f& GetMarioPosition() const;
    const std::shared_ptr<const std::list<std::unique_ptr<Block>>> GetBlocks() const { return mBlocks; }

private:

    enum class LevelCompletedStates {
        CountingPointsForLevelTime,
        CountingPointsForLivesMario,
        NextLevel

    } mLevelCompletedState = LevelCompletedStates::CountingPointsForLevelTime;

    static int mPoints;

    std::shared_ptr<std::vector<Landscape>> mLandscapes;
    std::shared_ptr<std::list<std::unique_ptr<Block>>> mBlocks;
    std::shared_ptr<std::list<std::unique_ptr<GameObject>>> mNotPhysicalObjs;
    std::shared_ptr<std::list<std::unique_ptr<PhysicalObject>>> mPhysicalObjs;

    std::vector<GameObject*>& mMyRemovedObjects;
    std::vector<GameObject*>& mMyNewObjects;

    /// NEEDED FOR PIPE MECHANISM
    std::string mTextureBackgroundName;

    sf::View mView;
    sf::FloatRect mBoundsView;
    std::unique_ptr<Mario> mMario;

    GameStates mGameState = GameStates::Gameplay;
    GamePlayStates mGamePlayState = GamePlayStates::MainGame;

    bool mBowserIsDying;
    bool mMarioInCastle = false;
    bool mBeforeWarning = true;

    const float mWhenActivateWarning;
    const short mWarningLengthTime = 4;
    const short mGameoverLengthTime = 8;
    const short mWinGameLengthTime = 8;
    const short mMarioDeadLengthTime = 5;
    const short mBowserDefeatLengthTime = 2;
    const short mEnteringToPipeLengthTime = 2;

    float mGameTime;
    float mTimer;
    float mSoundsTimer = 0.0f;

    int mNumberLivesToCount = 0;

    ///----METHODS
    inline void SetTimer(float pTime) { mTimer = sDurationScene + pTime; }
    inline void SetDoubleBoundsView();
    inline void CreateExtinguishingEffect(std::string pName);
    inline void MoveCamera();

    inline void UpdateGameTime();
    inline bool IsObjectVisible(const sf::FloatRect& pBounds, bool pLargerBounds = false);
    inline void GameUpdate();

    inline Block* WhichHitBlock(Block* pFirstBlock, Block* pSecondBlock, const PhysicalObject* pObj);
    inline std::map<GameObject::KindCollision, Block*> HowCollisionsWithBlocks(const PhysicalObject* pObj, bool& pNoneCollision);

    inline void MarioBlockCollision();
    inline void PhysicalObjBlockCollision();
    inline void MarioGameObjsCollision();
    inline void GameObjsGameObjsCollisions();

    inline void UpdateRemovedGameObjects();
    inline void UpdateNewGameObjects();
    inline void RemoveNewGameObjects();
};
