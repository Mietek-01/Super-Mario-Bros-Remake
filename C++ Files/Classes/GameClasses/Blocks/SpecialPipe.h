#pragma once
#include "Pipe.h"

#include "../Landscape.h"
#include "../PhysicalObject.h"

#include <list>
#include <memory>
#include <string>
#include <vector>

///------
class EnteringPipe : public Pipe
{
    /// ALLOWS UPDATING THE GAMEPLAY WHEN MARIO ENTERS OR EXITS THE PIPE
    static std::vector<EnteringPipe*> sActivateEnteringPipes;

    const std::string mUndergroundLvlName;
    Pipe* mReturnPipe;

    const float mEnteringRange = 15.0f;
    const float mEnteringSpeed = 0.5f;

    bool mActive = true;
    bool mEndEnteringAnimation = false;
    bool mUpEnteringAnimation;
    float mEndAnimationPos;

    std::unique_ptr<sf::Sprite> mMarioEnteringAnimation;

    ///------DATA NEEDED FOR SAVING THE LEVEL-----///

    std::shared_ptr<std::vector<Landscape>> mLandscapes;
    std::shared_ptr<std::list<std::unique_ptr<Block>>> mBlocks;
    std::shared_ptr<std::list<std::unique_ptr<GameObject>>> mNotPhysicalObjs;
    std::shared_ptr<std::list<std::unique_ptr<PhysicalObject>>> mPhysicalObjs;

    std::string mBackgroundLvlName;

    float mDurationScene;

    void SetEnteringToPipeAnimation(bool);
    void SetLeavingPipeAnimation();

    friend class ReturnPipe;

public:

    EnteringPipe(const std::pair<sf::Vector2f, Pipe::KindPipe>&, std::string, Pipe*);
    EnteringPipe(const std::pair<sf::Vector2f, Pipe::KindPipe>&, std::string);

    ~EnteringPipe() {}

    void Update() override;
    void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

    void SetUndergroundLvl();
    void SetBeforeWorld();
    void ResumeGame();

    void EnteringAnimation();

    bool IsTheEndEnteringAnimation() const { return mEndEnteringAnimation; }
    bool IsVisible() const override { return true; }

    static EnteringPipe* GetActivatePipe();
    static void ResetActivatedPipes();

};

///--------
class ReturnPipe : public StaticBlock
{
    const float mEnteringRange = 15;

public:

    ReturnPipe(sf::Vector2f);
    ~ReturnPipe() {}

    void Update() override;
    void Draw(const std::unique_ptr<sf::RenderWindow>&) override;

};
