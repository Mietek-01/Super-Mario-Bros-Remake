#include "ArmedTurtle.h"
#include "../../Scens/GameScen.h"
#include <time.h>

const float CArmedTurtle::s_length_road=150.0f;
const float CArmedTurtle::s_correction_pos_hammer=10.0f;
const float CArmedTurtle::s_force_hop=-500.0f;

CArmedTurtle::CArmedTurtle(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,{pos.x,pos.y-1},CPhysicaltObject::KindMovement::LEFT_RUN,500)
,m_after_what_time_jump(rand()%2+2)
,m_after_what_time_throw_hammer(rand()%2+1)
{
    m_value_acceleration=0.8f;
    m_jump_force=-1200.0f;
    m_where_change_direction=m_current_position.x-s_length_road;

    /// TWORZE ANIMACJE
    vector<sf::IntRect> m_frame_animation;

    m_frame_animation.push_back(sf::IntRect(288,159,32,50));
    m_frame_animation.push_back(sf::IntRect(320,159,32,50));

    m_animations->create(MyKindsAnimations::R_MOVE_WITH_HAMMER,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,1.5f,m_scale_to_tile);

    ///------
    m_frame_animation.clear();
    m_frame_animation.push_back(sf::IntRect(384,159,32,50));
    m_frame_animation.push_back(sf::IntRect(352,159,32,50));
    m_frame_animation.push_back(sf::IntRect(384,159,32,50));

    m_animations->create(MyKindsAnimations::R_MOVE_THROWING_HAMMER,CMarioGame::s_texture_manager["Enemies_right"],m_frame_animation,2.3f,m_scale_to_tile);
    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back(sf::IntRect(160,159,32,50));
    m_frame_animation.push_back(sf::IntRect(192,159,32,50));

    m_animations->create(MyKindsAnimations::L_MOVE_WITH_HAMMER,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,1.5f,m_scale_to_tile);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back(sf::IntRect(96,159,32,50));
    m_frame_animation.push_back(sf::IntRect(128,159,32,50));
    m_frame_animation.push_back(sf::IntRect(96,159,32,50));

    m_animations->create(MyKindsAnimations::L_MOVE_THROWING_HAMMER,CMarioGame::s_texture_manager["Enemies_left"],m_frame_animation,2.3f,m_scale_to_tile);

    m_animations->play(MyKindsAnimations::L_MOVE_THROWING_HAMMER,m_current_position);
    m_animations->setIndexFrame(2,m_current_position);
}

///----------
inline void CArmedTurtle::createHammer()
{
    m_my_hammer=new CHammer({m_current_position.x,m_current_position.y-getSize().y/2.0f+s_correction_pos_hammer},m_right_dir_reversal);
    addNewObject(m_my_hammer);

    m_when_throw_hammer=CScen::getDurationScen()+m_after_what_time_throw_hammer;

    if(m_right_dir_reversal)
        m_animations->play(MyKindsAnimations::R_MOVE_WITH_HAMMER,m_current_position);
    else
        m_animations->play(MyKindsAnimations::L_MOVE_WITH_HAMMER,m_current_position);
}

///----------
inline bool CArmedTurtle::randKindJump()
{
    bool up_jump=rand()%2;

    /// SPRAWDZAM CZY MOGE WYKONAC SKOK DO GORY
    if(up_jump&&m_current_position.y<CScen::s_tile_size*4+1)
        up_jump=false;

    /// MECHANIZM OPRACOWUJACY WYLOSOWANY RODZAJ SKOKU
    if(up_jump)
        return true;
    else
    {
        ///WYKONUJE SYMULACJE SKOKU W DOL I OKRESLAM ILE ZLAPIE KOLIZJI
        ///DOLNYCH I CZY SPELNIAJA ONE PEWNE KRYTERIA

         const auto game_scen_blocks=CMarioGame::instance().getScen<CGameScen>().getBlocks();
         vector<int> potential_block_with_bottom_collision;

         const sf::Vector2f saved_current_position=m_current_position;
         const KindMovement saved_kind_movement=m_kind_movement;
         const float saved_change_direction_pos=m_where_change_direction;

         int prev_y_pos_block_with_bottom_collision=0;

         hop(s_force_hop);

         /// ZLICZAM ILOSC DOLNYCH KOLIZJI I ZAPISUJE TE KTORE NIE SPRAWIA BUGOW
         while(m_current_position.y<CMarioGame::s_size_window.y)
         {
            changeMovementDirection();
            CPhysicaltObject::update();

            /// SPRAWDZAM DOLNE KOLIZJE
            for(const auto &block:*game_scen_blocks)
            {
                if(!block->isVisible())continue;

                const KindCollision how_collision=howCollision(*block);

                 if(how_collision==KindCollision::BOTTOM)
                 {
                    /// GLOWNY WARUNEK CALEGO ALGORYTMU
                    if(m_previous_position.y>saved_current_position.y&&!block->isHit()&&
                      block->getBounds().top-this->getBounds().height>prev_y_pos_block_with_bottom_collision)
                    {
                        potential_block_with_bottom_collision.push_back(block->getCurrentPosition().y);
                    }

                    prev_y_pos_block_with_bottom_collision=block->getCurrentPosition().y;

                    break;
                }
            }

         }

         /// PRZYWRACAM DAWNA POZYCJE
         this->setPosition(saved_current_position);
         m_previous_position=saved_current_position;
         m_previous_position.y-=CPhysicaltObject::s_correction_to_bottom_collision;

         m_kind_movement=saved_kind_movement;
         m_where_change_direction=saved_change_direction_pos;

         m_jump=false;

         if(potential_block_with_bottom_collision.size()!=0)
         {
             m_for_which_block_bottom_collision=potential_block_with_bottom_collision
             [rand()%potential_block_with_bottom_collision.size()];

             m_down_jump=true;

             return false;
         }else
            return true;
    }
}

///----------
void CArmedTurtle::jump()
{
    m_can_jump=false;

    if(randKindJump())
        CPhysicaltObject::jump();
    else
        hop(s_force_hop);
}

///---------
void CArmedTurtle::update()
{
    /// ZMIANA KIERUNKU RUCHU
    changeMovementDirection();

    /// SKOK W OKRESLONYM CZASIE
    if(m_when_jump<CScen::getDurationScen()&&m_can_jump)
        this->jump();

    CPhysicaltObject::update();

    ///----------MECHANIZM MLOTKA-------///

    /// STWORZENIE NOWEGO MLOTKA
    if(!m_my_hammer&&m_animations->islastFrame())
        createHammer();

    /// RZUT MLOTKIEM
    if(m_my_hammer&&m_when_throw_hammer<CScen::getDurationScen())
    {
        m_my_hammer->setEnabled(m_jump);
        m_my_hammer=nullptr;

        if(m_right_dir_reversal)
            m_animations->play(MyKindsAnimations::R_MOVE_THROWING_HAMMER,m_current_position);
        else
            m_animations->play(MyKindsAnimations::L_MOVE_THROWING_HAMMER,m_current_position);
    }

    /// RUCH Z MLOTKIEM
    if(m_my_hammer)
        m_my_hammer->setPosition({m_current_position.x,m_current_position.y-getSize().y/2.0f-s_correction_pos_hammer});

    /// ZMIANA ZWROTU
    changeLookingDirection();
}

///-------
inline void CArmedTurtle::changeMovementDirection()
{
    /// IF TUTAJ BO WYWOLUJE TA FUNKCE ROWNIEZ GDY OKRESLAM SKOK W DOL
    if((m_kind_movement==KindMovement::LEFT_RUN&&m_current_position.x<m_where_change_direction)||(m_kind_movement==KindMovement::RIGHT_RUN&&m_current_position.x>m_where_change_direction))
    {
        if(m_kind_movement==KindMovement::LEFT_RUN)
        {
            m_where_change_direction=m_current_position.x+s_length_road;
            m_kind_movement=KindMovement::RIGHT_RUN;
        }else
        {
            m_where_change_direction=m_current_position.x-s_length_road;
            m_kind_movement=KindMovement::LEFT_RUN;
        }
    }
}

///-----
inline void CArmedTurtle::changeLookingDirection()
{
    if(!m_right_dir_reversal&&CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x>m_current_position.x)
    {
        if(m_my_hammer)
        {
            m_animations->play(MyKindsAnimations::R_MOVE_WITH_HAMMER,m_current_position);
            m_my_hammer->changeReversal(m_current_position);

        }else
        {
            const int saved_index_frame=m_animations->getIndexFrame();
            m_animations->play(MyKindsAnimations::R_MOVE_THROWING_HAMMER,m_current_position);
            m_animations->setIndexFrame(saved_index_frame,m_current_position);
        }

        m_right_dir_reversal=true;

    }else
        if(m_right_dir_reversal&&CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x<m_current_position.x)
        {
            if(m_my_hammer)
            {
                m_animations->play(MyKindsAnimations::L_MOVE_WITH_HAMMER,m_current_position);
                m_my_hammer->changeReversal(m_current_position);
            }else
            {
                const int saved_index_frame=m_animations->getIndexFrame();
                m_animations->play(MyKindsAnimations::L_MOVE_THROWING_HAMMER,m_current_position);
                m_animations->setIndexFrame(saved_index_frame,m_current_position);
            }

            m_right_dir_reversal=false;
        }
}

///----------
void CArmedTurtle::updateForCollisionWithBlock(KindCollision how_collision ,CBlock* block)
{
    if(how_collision==KindCollision::BOTTOM)
    {
        if(block->isHit()||block->iamDead())
            actOnMe(KindAction::HIT);
        else
        {
            /// TUTAJ SPRAWDZAM ZA KTORA DOLNA KOLIZJA MAM WYKONAC KORYGACJE
            if(m_down_jump)
            {
                if(static_cast<int>(block->getCurrentPosition().y)==m_for_which_block_bottom_collision)
                    m_down_jump=false;
                else
                    return;
            }

            m_in_bottom_collision=true;
            m_jump=false;

            setWhenJump();
        }

        block->actOnObject(this,how_collision);
    }
}

///-----
void CArmedTurtle::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        obj->actOnMe(KindAction::HIT);
}

///-----
void CArmedTurtle::actOnMe(KindAction which_action)
{
    if(m_dead)return;

    switch(which_action)
    {
    case KindAction::HIT:
        {
            if(m_my_hammer)
                removeObject(m_my_hammer);

            createBeatsObjSpecialEfect(-100.0f);

            break;
        }
    }
}

///------
inline void CArmedTurtle::setWhenJump()
{
    m_can_jump=true;
    m_when_jump=CScen::getDurationScen()+m_after_what_time_jump;
}

///---------
void CArmedTurtle::draw(const unique_ptr<sf::RenderWindow>& window)
{
    m_animations->draw(window);

    if(m_my_hammer)
        m_my_hammer->draw(window);
}

