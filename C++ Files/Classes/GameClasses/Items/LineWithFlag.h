#pragma once
#include "../GameObject.h"

using namespace std;

class CLineWithFlag: public CGameObject
{

    enum MyKindsAnimations
    {
        FLAG_ANIMATION,
        MOVEMENT_MARIO,
        R_RATCHETING_BY_MARIO,
        L_RATCHETING_BY_MARIO,

    };

    enum class MarioAnimationsStates
    {
        LEFT_RATCHETING,
        RIGHT_RATHETING,
        GO_TO_CASTLE

    }m_ratcheting_flag_state=MarioAnimationsStates::LEFT_RATCHETING;


    unique_ptr<sf::Sprite> m_castle;
    unique_ptr<CAnimations> m_castle_flag;
    unique_ptr<CAnimations> m_mario;

    CAnimations m_line_flag;

    bool m_is_flag_ratcheting=false;

    const float m_castle_position;
    const float m_castle_flag_speed=0.3f;
    const float m_flag_speed_ratcheting=2.2f;
    const float m_mario_speed_ratcheting=2.5f;
    const float m_mario_speed=2.8f;

    sf::Vector2f m_line_flag_position,m_mario_position,m_castle_flag_position;

public:

    CLineWithFlag(sf::Vector2f);
    ~CLineWithFlag(){}

    void draw(const unique_ptr<sf::RenderWindow>&) override;
    void update() override;

    void actOnObject(CGameObject*,KindCollision)override;
    void actOnMe(KindAction)override;
};

