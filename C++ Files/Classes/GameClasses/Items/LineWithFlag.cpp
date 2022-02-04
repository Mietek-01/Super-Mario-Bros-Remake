#include "LineWithFlag.h"
#include "../../Scens/GameScen.h"
#include "../Blocks/Block.h"
#include "../Mario.h"

CLineWithFlag::CLineWithFlag(sf::Vector2f pos)
:CGameObject(new CSprite(CGUI::createSprite("TheEndLvlLine", { 17,0,18,(int)((CMarioGame::s_size_window.y - CScen::s_tile_size * 5) / m_scale_to_tile) }, pos, m_scale_to_tile, true)), Parentage::ITEM, pos)
,m_castle(CGUI::createSprite("Castle",{pos.x+CScen::s_tile_size*13,pos.y},1.0f,true))
,m_castle_position(pos.x+CScen::s_tile_size*13)
{
    /// TWORZE FLAGE
    vector<sf::IntRect> m_frame_animation;

	m_frame_animation.push_back({ 0,180,32,32 });
	m_frame_animation.push_back({ 32,180,32,32 });
	m_frame_animation.push_back({ 64,180,32,32 });
	m_frame_animation.push_back({ 96,180,32,32 });

    m_line_flag.create(MyKindsAnimations::FLAG_ANIMATION,CMarioGame::s_texture_manager["Items"],m_frame_animation,1.5f,m_scale_to_tile,false);

    m_line_flag_position=m_current_position;
    m_line_flag_position.y-=getBounds().height;
    m_line_flag_position.y+=22;
    m_line_flag_position.x+=3;

    m_line_flag.setPosition(m_line_flag_position);
}

///--------
void CLineWithFlag::update()
{
    ///---- ETAPY MARIA
    if(m_mario)
    {
        switch(m_ratcheting_flag_state)
        {
        case MarioAnimationsStates::LEFT_RATCHETING:
        {
            m_mario_position.y+=m_mario_speed_ratcheting;

            if(m_mario_position.y>=m_current_position.y)
            {
                m_mario->play(MyKindsAnimations::R_RATCHETING_BY_MARIO,m_current_position);
                m_mario_position.x=m_current_position.x+m_mario->getGlobalBounds().width/2.0f;
                m_mario_position.y=m_current_position.y;

                m_ratcheting_flag_state=MarioAnimationsStates::RIGHT_RATHETING;
            }

            break;
        }

        case MarioAnimationsStates::RIGHT_RATHETING:
        {
            break;
        }

        case MarioAnimationsStates::GO_TO_CASTLE:
        {
            m_mario_position.x+=m_mario_speed;
            CMarioGame::instance().getScen<CGameScen>().changePositionView(m_mario_speed);

            if(m_mario_position.x>=m_castle_position)
            {
                m_mario.reset();
                CMarioGame::instance().getScen<CGameScen>().marioInCastle(true);

                /// TWORZE FLAGE ZAMKU

                m_castle_flag.reset(new CAnimations);

                vector<sf::IntRect> m_frame_animation;

                m_frame_animation.push_back(sf::IntRect(0,147,32,32));
                m_frame_animation.push_back(sf::IntRect(32,147,32,32));
                m_frame_animation.push_back(sf::IntRect(64,147,32,32));
                m_frame_animation.push_back(sf::IntRect(96,147,32,32));

                m_castle_flag->create(MyKindsAnimations::FLAG_ANIMATION,CMarioGame::s_texture_manager["Items"],m_frame_animation,1.5f,m_scale_to_tile,false);

                m_castle_flag_position=m_castle->getPosition();

                m_castle_flag_position.y-=m_castle->getGlobalBounds().height;
                m_castle_flag_position.y+=m_castle_flag->getGlobalBounds().height/2.0f;

                m_castle_flag_position.x+=m_castle_flag->getGlobalBounds().width/2.4f;

                m_castle_flag->setPosition(m_castle_flag_position);
            }

            break;
        }
        }

        ///----- OPUSZCZANIE FLAGI
        if(m_is_flag_ratcheting)
        {
            m_line_flag_position.y+=m_flag_speed_ratcheting;

            if(m_line_flag_position.y>=m_current_position.y-m_line_flag.getGlobalBounds().height)
            {
                m_is_flag_ratcheting=false;
                m_ratcheting_flag_state=MarioAnimationsStates::GO_TO_CASTLE;

                m_mario->play(MyKindsAnimations::MOVEMENT_MARIO,m_current_position);
            }
        }
    }

    ///--- UPDATY OBIEKTOW
    m_line_flag.update(m_line_flag_position);

    if(m_castle_flag)
    {
        if(m_castle_flag_position.y>=m_castle->getPosition().y-m_castle->getGlobalBounds().height-m_castle_flag->getGlobalBounds().height)
            m_castle_flag_position.y-=m_castle_flag_speed;

        m_castle_flag->update(m_castle_flag_position);
    }

    if(m_mario)
        m_mario->update(m_mario_position);
}

///-----------
void CLineWithFlag::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    CMario *mario=dynamic_cast<CMario*>(obj);

    if(mario)
    {
        vector<sf::IntRect> m_frame_animation;
        m_mario.reset(new CAnimations);

        switch(mario->getLevelMario())
        {
            case CMario::LevelMario::SMALL_MARIO:
            {
                m_frame_animation.push_back(sf::IntRect(35,96,25,32));
                m_frame_animation.push_back(sf::IntRect(68,96,25,32));
                m_frame_animation.push_back(sf::IntRect(97,96,30,32));

                m_mario->create(MyKindsAnimations::MOVEMENT_MARIO,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.2f,m_scale_to_tile);

                ///----
                m_frame_animation.clear();
                m_frame_animation.push_back(sf::IntRect(226,96,27,32));
                m_frame_animation.push_back(sf::IntRect(258,97,27,32));

                m_mario->create(MyKindsAnimations::R_RATCHETING_BY_MARIO,CMarioGame::s_texture_manager["Mario_left"],m_frame_animation,1.5f,m_scale_to_tile);

                ///------
                m_frame_animation.clear();
                m_frame_animation.push_back(sf::IntRect(258,96,27,32));
                m_frame_animation.push_back(sf::IntRect(226,97,27,32));

                m_mario->create(MyKindsAnimations::L_RATCHETING_BY_MARIO,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.5f,m_scale_to_tile);

                break;
            }

            case CMario::LevelMario::BIG_MARIO:
            {
                m_frame_animation.push_back(sf::IntRect(32,36,32,60));
                m_frame_animation.push_back(sf::IntRect(65,36,30,60));
                m_frame_animation.push_back(sf::IntRect(95,35,32,60));

                m_mario->create(MyKindsAnimations::MOVEMENT_MARIO,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.2f,m_scale_to_tile);

                ///----
                m_frame_animation.clear();
                m_frame_animation.push_back(sf::IntRect(192,39,30,57));
                m_frame_animation.push_back(sf::IntRect(226,44,30,52));

                m_mario->create(MyKindsAnimations::R_RATCHETING_BY_MARIO,CMarioGame::s_texture_manager["Mario_left"],m_frame_animation,1.5f,m_scale_to_tile);

                ///------
                m_frame_animation.clear();
                m_frame_animation.push_back(sf::IntRect(290,39,30,57));
                m_frame_animation.push_back(sf::IntRect(256,44,30,52));

                m_mario->create(MyKindsAnimations::L_RATCHETING_BY_MARIO,CMarioGame::s_texture_manager["Mario_right"],m_frame_animation,1.5f,m_scale_to_tile);

                break;
            }
        }

        m_mario->play(MyKindsAnimations::L_RATCHETING_BY_MARIO,m_current_position);

        m_mario_position=mario->getCurrentPosition();
        m_mario_position.x=m_current_position.x;
        m_mario_position.x-=m_mario->getGlobalBounds().width/2.0f;

        m_mario->setPosition(m_mario_position);

        actOnMe(KindAction::HIT);
        removeObject(obj);
    }
}

///-------
void CLineWithFlag::actOnMe(KindAction how_action)
{
    if(how_action==KindAction::HIT)
        if(CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::LVL_COMPLETED))
            m_is_flag_ratcheting=true;
}

///--------
void CLineWithFlag::draw(const unique_ptr<sf::RenderWindow>& window)
{
    m_animator->draw(window);
    m_line_flag.draw(window);

    if(m_castle_flag)
        m_castle_flag->draw(window);

    window->draw(*m_castle);

    if(m_mario)
        m_mario->draw(window);
}
