#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

/// Manages a damageable shield with an associated visual health-bar pointer.
class Shield {
protected:

    /// Visual indicator that tracks remaining shield health as a shrinking bar.
    class ShieldPointer {

    public:

        enum class KindsOrigin {
            Left,
            Right,
            Bottom
        };

        void Draw(const std::unique_ptr<sf::RenderWindow>& pWindow, sf::Vector2f pPos);
        void Update();

        [[nodiscard]] bool IsShieldDepleted() const { return mIsShieldDepleted; }

    private:

        static const short sMargin;

        const KindsOrigin mKindOrigin;

        std::unique_ptr<sf::Sprite> mSprite;

        std::unique_ptr<sf::RectangleShape> mFirstRect;
        std::unique_ptr<sf::RectangleShape> mSecondRect;
        std::unique_ptr<sf::RectangleShape> mDamageRect;

        const float mReducingShieldSpeed = 1.0f;
        const float mBasicShieldValue;
        const float mShieldPointerLength;
        const sf::Color mColorDamageRect = sf::Color::White;

        bool mIsShieldDepleted = false;
        float mExtraSpeed = 1.0f;

        /// Methods
        friend class Shield;
        ShieldPointer(sf::RectangleShape* pFirstRect, sf::Color pColor, KindsOrigin pKindOrigin, int pShieldValue, sf::Sprite* pSprite = nullptr);

        inline void UpdatePosition(sf::Vector2f pPos);
        inline void CreateDamageRectangle(float pLengthDamage);

        void ReduceShieldPointer(float pDamage);
    };

    std::unique_ptr<ShieldPointer> mShieldPointer;

    void CreateShieldPointer(sf::RectangleShape* pRect, sf::Color pColor, ShieldPointer::KindsOrigin pKindOrigin, sf::Sprite* pSprite = nullptr);
    void ReduceShield();

private:

    int mInflictedDamage = 0;
    int mShieldValue;
    const int mBasicShieldValue;

public:

    explicit Shield(int pShieldValue);
    void SetDamage(int pDamage) { mInflictedDamage = pDamage; }
    [[nodiscard]] int GetShieldValue() const { return mShieldValue; }

};
