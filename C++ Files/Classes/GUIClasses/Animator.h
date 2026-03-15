#pragma once
#include <SFML/Graphics.hpp>
#include <cassert>
#include <map>
#include <memory>
#include <vector>

/// Base class providing sprite-based drawing and position management.
class Animator {
public:
    virtual ~Animator() {}

    void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow) { pWindow->draw(*mCurrentFrame); }
    virtual void Update(const sf::Vector2f& pPosition) = 0;

    void SetPosition(const sf::Vector2f& pPosition) { mCurrentFrame->setPosition(pPosition); }

    [[nodiscard]] sf::FloatRect GetGlobalBounds() const { return mCurrentFrame->getGlobalBounds(); }
    [[nodiscard]] sf::Vector2f GetPosition() const { return mCurrentFrame->getPosition(); }
    [[nodiscard]] sf::Sprite& GetSprite() const { assert(mCurrentFrame); return *mCurrentFrame; }

protected:
    sf::Sprite* mCurrentFrame = nullptr;
};

/// ----------------------------------------------------------- ///
/// Single-sprite animator with no animation logic.
class SpriteAnimator : public Animator {
public:
    explicit SpriteAnimator(sf::Sprite* pSprite) { mCurrentFrame = pSprite; }
    ~SpriteAnimator() { delete mCurrentFrame; }

    void Update(const sf::Vector2f& pPosition) override { mCurrentFrame->setPosition(pPosition); }
};

/// ----------------------------------------------------------- ///
/// Multi-frame animator supporting named animation sequences.
class Animations : public Animator {
    int mCurrentAnimation = -1;
    std::map<int, std::pair<std::vector<sf::Sprite>, float>> mAnimations;

    int mIndexFrame = 0;
    int mCountFrames = 0;
    float mCurrentSpeed = 0;
    float mTimer = 0.0f;
    bool mIsLastFrame = false;

    inline void SetCurrentFrame();

public:
    /// Animation direction/state identifiers.
    /// Values start at 1000 so derived-class enums can use lower ranges.
    enum BasicKindsAnimations {
        LeftMove = 1000,
        RightMove,
        LeftStanding,
        RightStanding,
        LeftJump,
        RightJump,
        Standard
    };

    ~Animations() {}

    void Create(int pName, const sf::Texture& pTexture, const sf::IntRect& pRect, float pScale, bool pBottomOrigin = true);
    void Create(int pName, const sf::Texture& pTexture, const std::vector<sf::IntRect>& pRects, float pSpeed, float pScale, bool pBottomOrigin = true);

    void Update(const sf::Vector2f& pPosition) override;
    void Update();

    void Play(int pName, const sf::Vector2f& pPosition);
    void Reset() { mAnimations.clear(); mCurrentFrame = nullptr; }

    void SetSpeed(float pNewSpeed);
    void SetIndexFrame(int pNewIndex, const sf::Vector2f& pPosition);

    [[nodiscard]] int GetIndexFrame() const { return mIndexFrame; }
    [[nodiscard]] int GetAnimationName() const { return mCurrentAnimation; }
    [[nodiscard]] bool IsLastFrame() const { return mIsLastFrame; }
};
