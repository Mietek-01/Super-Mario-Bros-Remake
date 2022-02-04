#include "Bowser.h"
#include "../../Scens/GameScen.h"
#include "../Blocks/MysterBox.h"
#include "../Items/FireballBullets.h"
#include "../Mario.h"
#include <time.h>
#include <math.h>
#include "Enemies.h"
#include "../../Scens/LoadingGameScen.h"

///------------CCreatorEnemies------------------///

CBowser::CCreatorEnemies::CCreatorEnemies(int begin_tiles,int end_tiles)
:m_number_tiles(abs(end_tiles-begin_tiles)/(int)CScen::s_tile_size)
,m_first_tile_position(begin_tiles+CScen::s_tile_size)
{
    setWhenNextEnemy();
}

///-----
CPhysicaltObject * CBowser::CCreatorEnemies::getCreatedEnemy()
{
    if(m_when_create_enemie<CScen::getDurationScen())
    {
        setWhenNextEnemy();

        int which_tile;

        do
        {
            which_tile=CGUI::rand(m_number_tiles,0);

        }while(which_tile==m_last_rand_tile);

        m_last_rand_tile=which_tile;

        const sf::Vector2f position(which_tile*CScen::s_tile_size+m_first_tile_position,-10);

        if(CGUI::rand(4,0)==2)
            return new CRedTurtle(position,static_cast<CPhysicaltObject::KindMovement>(CGUI::rand(2,0)),true);
        else
            return new CGoombas(position,static_cast<CPhysicaltObject::KindMovement>(CGUI::rand(2,0)),true);
    }

    return nullptr;
}

///--------
inline void CBowser::CCreatorEnemies::setWhenNextEnemy()
{
    m_when_create_enemie=CScen::getDurationScen()+m_creating_speed;
}

///--------------CBowser--------------///

bool CBowser::s_instance=false;

CBowser::~CBowser()
{
    s_instance=false;

    if(!CMarioGame::instance().isThisScen<CGameScen>())return;

    sf::Sprite * hit_obj=new sf::Sprite(m_animator->getSprite());

    hit_obj->setRotation(180);
    hit_obj->setPosition(hit_obj->getPosition().x,hit_obj->getPosition().y-hit_obj->getGlobalBounds().height);

    CGuiObject* obj=new CSpecialEffects(new CSprite(hit_obj),CSpecialEffects::KindUpdate::JUMP,CMarioGame::s_size_window.y+CScen::s_tile_size,{0,-1000});
    CGUI::addGuiObject(obj);

    m_barse_to_princes->active();

    addNewObject(new CPrincess(m_princess_position));
}

///--------
CBowser::CBowser(sf::Vector2f pos)
:CPhysicaltObject(new CAnimations,Parentage::ENEMY,pos,KindMovement::LEFT_RUN,1000)
,CShield(4500)
,m_where_barse(400)
,m_left_barse_pos(m_current_position.x-(m_where_barse+CMarioGame::s_size_window.x/2.0f+200))
,m_right_barse_pos(m_current_position.x+m_where_barse)
,m_princess_position({ pos.x + m_where_barse + CMarioGame::s_size_window.x / 2.0f+100, (float)(CMarioGame::s_size_window.y - CScen::s_tile_size * 2 )})
{
    if(s_instance&&CMarioGame::instance().isThisScen<CLoadingGameScen>())
    {
        cout<<" WIECEJ NIZ JEDEN BOWSER !!!!!!!!!!!!!!!"<<endl;
        assert(0);
    }

    s_instance=true;

    this->createAnimations();
    m_animations->play(CAnimations::L_MOVE,m_current_position);

    m_value_acceleration=m_basic_speed;

    addNewObject(new CMysterBox({ m_left_barse_pos - (CScen::s_tile_size / 2.0f + CScen::s_tile_size * 5) , (float)(CMarioGame::s_size_window.y - (5 * CScen::s_tile_size))}, CMysterBox::MyItem::FLOWER));
}

///-------------
void CBowser::update()
{
    if (!m_enable)
    {
        this->setTrapForMario();
        CPhysicaltObject::update();
    }
    else
    {
        /// SPRAWDZAM CZY MARIO JEST ZA BOWSEREM
        changeReversal();

        if(m_atack)
            (this->*m_atack)();

        if(!m_atack&&m_when_atack<CScen::getDurationScen()&&!m_changing_reversal)
            setAtack();

        CPhysicaltObject::update();

        /// AKTUALIZUJE POZIOM OSLONY BOWSERA
        m_shield_pointer->update();

        if (m_shield_pointer->isTheEndShield())
            CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::BOWSER_DEFEAT);
  
    }
}

///-----
inline void CBowser::setAtack(KindsAtacks which_attack)
{
    if(which_attack==KindsAtacks::RANDOM)
        which_attack=static_cast<KindsAtacks>(CGUI::rand(6,0));

    switch(which_attack)
    {
    case KindsAtacks::SPECIAL_SIN_FIREBALL_ATACK:
        {
            (this->m_atack)=&CBowser::specialSinFireBallAtack;

            if(m_right_dir_reversal)
                m_animations->play(MyKindsAnimations::R_SPECIAL_SIN_FIRE_BALL_ATACK,m_current_position);
            else
                m_animations->play(MyKindsAnimations::L_SPECIAL_SIN_FIRE_BALL_ATACK,m_current_position);

            m_animations->setSpeed(m_shooting_speed);

            m_kind_movement=KindMovement::STANDING;

            m_timer=CScen::getDurationScen()+m_fireball_sin_lenght_attack;
            break;
        }

    case KindsAtacks::SPECIAL_CIRCLE_FIREBALL_ATACK:
        {
            (this->m_atack)=&CBowser::specialCircleFireBallAtack;

            if(m_right_dir_reversal)
                m_animations->play(MyKindsAnimations::R_SPECIAL_CIRCLE_FIRE_BALL_ATACK,m_current_position);
            else
                m_animations->play(MyKindsAnimations::L_SPECIAL_CIRCLE_FIRE_BALL_ATACK,m_current_position);

            m_animations->setSpeed(m_shooting_speed);

            m_kind_movement=KindMovement::STANDING;

            break;
        }

    case KindsAtacks::BASIC_FIREBALL_ATACK:
        {
            (this->m_atack)=&CBowser::basicFireBallAtack;

            if(m_right_dir_reversal)
                m_animations->play(MyKindsAnimations::R_BASIC_FIRE_BALL_ATACK,m_current_position);
            else
                m_animations->play(MyKindsAnimations::L_BASIC_FIRE_BALL_ATACK,m_current_position);

            m_animations->setSpeed(m_shooting_speed);

            m_kind_movement=KindMovement::STANDING;

            break;
        }

    case KindsAtacks::LEAP_ON_MARIO:
        {
            (this->m_atack)=&CBowser::leapOnMario;

            const float length_between_mario_and_bowser=fabs(m_current_position.x-CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x);

            m_my_pos_before_jump=m_current_position;
            m_second_zero_place=length_between_mario_and_bowser;

            if(m_right_dir_reversal)
                m_X_coordinate=0.0f;
            else
                m_X_coordinate=m_second_zero_place;

            float how_hight_jump=150;

            if(length_between_mario_and_bowser>400)
            {
                how_hight_jump*=2;
                m_value_acceleration*=7.0f;

            }else m_value_acceleration*=5.0f;

            /// OBLICZMA WSPOLCZYNNIK A KTORY OKRESLA MI WYSOKOSC SKOKU
            m_A_coefficient=(4*how_hight_jump)/-(m_second_zero_place*m_second_zero_place);

            jump();
            m_block_changing_reversal=true;

            break;
        }

    case KindsAtacks::CHARGE:
        {
            /// WYKORZYTSUJE ZMIENNA DO PRZECHOWANIA POZYCJI MARIA

            m_second_zero_place=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x;

            if(fabs(m_second_zero_place-m_current_position.x)>300)
            {
                (this->m_atack)=&CBowser::charge;
                m_value_acceleration=5.0f;
                m_block_changing_reversal=true;

            }else
                setAtack();

            break;
        }

    case KindsAtacks::ENEMIES_RAID:
        {
            (this->m_atack)=&CBowser::enemiesRaid;

            if(m_right_dir_reversal)
                m_animations->play(MyKindsAnimations::L_CHANGING_TO_REVERSAL,m_current_position);
            else
                m_animations->play(MyKindsAnimations::R_CHANGING_TO_REVERSAL,m_current_position);

            m_kind_movement=KindMovement::STANDING;
            m_block_changing_reversal=true;

            m_timer=CScen::getDurationScen()+m_enemies_raid_lenght_attack;

            /// TWORZE KREATOR WROGOW

            const int mario_position=CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x;

            int begin_tiles=mario_position-m_raid_lenght;
            int end_tiles=mario_position+m_raid_lenght;

            // GDY WYCHODZI POZA KRATY
            if(begin_tiles<m_left_barse_pos+CScen::s_tile_size*2)
            {
                begin_tiles=m_left_barse_pos+CScen::s_tile_size*2;
                end_tiles=begin_tiles+m_raid_lenght*1.5f;
            }

            if(end_tiles>m_right_barse_pos-CScen::s_tile_size*2)
            {
                end_tiles=m_right_barse_pos-CScen::s_tile_size*2;
                begin_tiles=end_tiles-m_raid_lenght*1.5f;
            }

            m_creator_enemies.reset(new CCreatorEnemies(begin_tiles,end_tiles));

            break;
        }
    }
}

///----
void CBowser::enemiesRaid()
{
    if(m_animations->islastFrame())
        if(m_animations->getAnimationName()==MyKindsAnimations::R_CHANGING_TO_REVERSAL)
            m_animations->play(MyKindsAnimations::L_CHANGING_TO_REVERSAL,m_current_position);
        else
            m_animations->play(MyKindsAnimations::R_CHANGING_TO_REVERSAL,m_current_position);

    CPhysicaltObject * new_enemy=m_creator_enemies->getCreatedEnemy();

    if(new_enemy)
        addNewObject(new_enemy);

    /// KONIEC TEGO ATAKU
    if(m_timer<CScen::getDurationScen())
    {
        setWhenAtack();
        m_creator_enemies.reset();
    }
}

///------
void CBowser::charge()
{
    if((m_right_dir_reversal&&m_current_position.x>m_second_zero_place)||
    (!m_right_dir_reversal&&m_current_position.x<m_second_zero_place))
    {
        setWhenAtack();
        m_value_acceleration=m_basic_speed;
    }
}

///---
void CBowser::basicFireBallAtack()
{
    if(m_attack_interrupted)
    {
        setWhenAtack();
        return;
    }

    /// UZYWAM TIMERA DO PRZECHOWANIA POPRZEDNIEGO INDEKSU KLATKI
    /// BY MOGL WYKONAC SIE STRZAL
    if(m_animations->getIndexFrame()!=m_timer)
        m_shoot=false;

    m_timer=m_animations->getIndexFrame();

    shoot({1,2,3},1,KindBullet::BASIC);

    if (m_animations->islastFrame())
        setWhenAtack();
}

///---
void CBowser::specialCircleFireBallAtack()
{
    if(m_attack_interrupted)
    {
        setWhenAtack();
        return;
    }

    shoot({1,3,5},1,KindBullet::CIRCLE_MOVEMENT);

    /// KONIEC TEGO ATAKU
    if(m_animations->islastFrame())
        setWhenAtack();
}

///------
void CBowser::leapOnMario()
{
    /// CZYLI GDY JUZ JESTEM PO SKOKU
    if(!m_jump)
    {
        setWhenAtack();
        m_value_acceleration=m_basic_speed;
    }
}

///----------
void CBowser::specialSinFireBallAtack()
{
    if(m_attack_interrupted)
        if(m_changing_reversal)
            return;
        else
        {
            m_attack_interrupted=false;

            /// KONTYNULUJE DALEJ TEN ATAK
            const float time=m_timer;

            setAtack(KindsAtacks::SPECIAL_SIN_FIREBALL_ATACK);

            m_timer=time;

            if(m_timer<CScen::getDurationScen())m_timer+3.0f;
        }

    shoot({1,3},3,KindBullet::SIN_MOVEMENT);

    /// KONIEC TEGO ATAKU
    if(m_animations->islastFrame()&&m_timer<CScen::getDurationScen())
        setWhenAtack();
}

///-----
inline void CBowser::shoot(const vector<int>for_which_frame_shoot,int small_frame,KindBullet kind_bullet)
{
    /// MECHANIZM STRZELANIA KULAMI (DZIALA DLA KLATEK NIE WYSTEPUJACYCH PO SOBIE)
    // DLA KLATEK WYSTEPUJACYCH PO SOBIE METODA WYWOLUJACA TA METODE SAMA MUSI ZMIENIC M_SHOOT

    bool is_it_frame_to_shoot=false;
    int frame_to_shoot;

    for(const auto &for_which_frame:for_which_frame_shoot)
        if(m_animations->getIndexFrame()==for_which_frame)
        {
            is_it_frame_to_shoot=true;
            frame_to_shoot=for_which_frame;
            break;
        }

    if(is_it_frame_to_shoot)
    {
        /// SPRAWIA ZE WYKONA SIE JEDEN STRZAL DLA JEDNEJ KLATKI
        if(!m_shoot)
        {
            m_shoot=true;
            sf::Vector2f bullet_pos=m_current_position;

            if(frame_to_shoot!=small_frame)
                bullet_pos.y-=getSize().y*0.7f;
            else
                bullet_pos.y-=getSize().y*0.2f;

            if(m_right_dir_reversal)
                bullet_pos.x+=getSize().x/2.0f;
            else
                bullet_pos.x-=getSize().x/2.0f;


            switch(kind_bullet)
            {

            case KindBullet::BASIC:
                addNewObject(new CBasicBullet(bullet_pos,m_right_dir_reversal));
                break;

            case KindBullet::SIN_MOVEMENT:
                addNewObject(new CSinBullet(bullet_pos,m_right_dir_reversal));
                break;

            case KindBullet::CIRCLE_MOVEMENT:
                addNewObject(new CCircleBullet(bullet_pos,m_right_dir_reversal));
                break;

            }
        }

    }else
        m_shoot=false;
}

///--------
inline void CBowser::changeReversal()
{
    if(m_block_changing_reversal)
        return;

    if(m_changing_reversal)
    {
        if(m_animations->islastFrame())
        {
            if(m_right_dir_reversal)
                m_animations->play(CAnimations::R_MOVE,m_current_position);
            else
                m_animations->play(CAnimations::L_MOVE,m_current_position);

            m_changing_reversal=false;
        }

        return;
    }

    if(!m_right_dir_reversal&&CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x>m_current_position.x)
    {
        m_animations->play(MyKindsAnimations::R_CHANGING_TO_REVERSAL,m_current_position);
        m_changing_reversal=true;
        m_right_dir_reversal=true;
        m_kind_movement=KindMovement::RIGHT_RUN;

        if(m_atack)
            m_attack_interrupted=true;
    }
    else
    if(m_right_dir_reversal&&CMarioGame::instance().getScen<CGameScen>().getMarioPosition().x<m_current_position.x)
    {
        m_animations->play(MyKindsAnimations::L_CHANGING_TO_REVERSAL,m_current_position);
        m_changing_reversal=true;
        m_right_dir_reversal=false;
        m_kind_movement=KindMovement::LEFT_RUN;

        if(m_atack)
            m_attack_interrupted=true;
    }
}

///------
inline void CBowser::setTrapForMario()
{
    m_enable=true;

    CMarioGame::instance().getScen<CGameScen>().setGamePlayState(CGameScen::GamePlayStates::FIGHT_WITH_BOWSER);

    /// USTAWIAM KIEDY ATAK
    this->setWhenAtack();

    /// TWORZE LEWA KRATE
    addNewObject(new CBarse({ m_left_barse_pos,(float)CMarioGame::s_size_window.y }, true, true));

    /// TWORZE PRAWA KRATE( JEST KLUCZOWA)
    m_barse_to_princes=new CBarse({m_right_barse_pos,0},false,false);
    addNewObject(m_barse_to_princes);

    /// TWORZE WSKAZNIK ZYCIA
    createShieldPointer(CGUI::createRectangleShape({0,0,500,30},sf::Color::Black,false,true)
                ,sf::Color::Red,CShieldPointer::KindsOrigin::RIGHT,CGUI::createSprite("Bowser_right",{49,2,84,80},{0,0},0.5f,true));
}

///----
inline void CBowser::setWhenAtack()
{
    if(!m_changing_reversal)
    if(m_right_dir_reversal)
    {
        m_animations->play(CAnimations::R_MOVE,m_current_position);
        m_kind_movement=KindMovement::RIGHT_RUN;
    }
    else
    {
        m_animations->play(CAnimations::L_MOVE,m_current_position);
        m_kind_movement=KindMovement::LEFT_RUN;
    }

    if(!CMarioGame::instance().getScen<CGameScen>().isMarioDead())
        m_when_atack=CScen::getDurationScen()+rand()%2+1;
    else
        m_when_atack=CScen::getDurationScen()+20;

    (this->m_atack)=nullptr;

    m_attack_interrupted=false;
    m_block_changing_reversal=false;
    m_shoot=false;
}

///-----
void CBowser::makeJump()
{
    if(m_atack==&CBowser::leapOnMario)
    {
        /// SKOK NA PODSTAWIE FUNKCJI KWADRATOWEJ
        if(m_right_dir_reversal)
            m_X_coordinate+=m_value_acceleration;
        else
            m_X_coordinate-=m_value_acceleration;

        m_current_position.y=m_my_pos_before_jump.y+(-(m_A_coefficient*(m_X_coordinate-0)*(m_X_coordinate-m_second_zero_place)));

    }else
        CPhysicaltObject::makeJump();
}

///-------------
void CBowser::actOnMe(KindAction how_action)
{
    if(how_action== KindAction::HIT)
        reduceShield();
}

///-------------
void CBowser::actOnObject(CGameObject* obj,KindCollision how_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
    {
        CMario *mario=dynamic_cast<CMario* >(obj);
        mario->setDamage(m_damage_value);

        obj->actOnMe(KindAction::HIT);

        if(m_jump&&!mario->isJumping())
            obj->actOnMe(KindAction::HOP);
    }
}

///------
void CBowser::draw(const unique_ptr<sf::RenderWindow>&window)
{
    m_animator->draw(window);

    if(m_shield_pointer)
        m_shield_pointer->draw(window,{CMarioGame::instance().getViewPosition().x+CMarioGame::s_size_window.x/2.0f-40,20});
}

///---------
inline void CBowser::createAnimations()
{
    vector<sf::IntRect> m_frame_animation;
    m_frame_animation.push_back({421,4,82,76});
    m_frame_animation.push_back({337,2,83,78});
    m_frame_animation.push_back({253,1,83,79});
    m_frame_animation.push_back({168,4,83,76});
    m_frame_animation.push_back({84,6,84,74});
    m_frame_animation.push_back({1,3,82,77});

    m_animations->create(CAnimations::L_MOVE,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,1.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({50,4,82,76});
    m_frame_animation.push_back({133,2,83,78});
    m_frame_animation.push_back({217,1,83,79});
    m_frame_animation.push_back({302,4,83,76});
    m_frame_animation.push_back({385,6,84,74});
    m_frame_animation.push_back({470,3,82,77});

    m_animations->create(CAnimations::R_MOVE,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,1.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({381,125,74,82});
    m_frame_animation.push_back({458,122,71,85});

    m_animations->create(MyKindsAnimations::R_CHANGING_TO_REVERSAL,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,3.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({98,125,74,82});
    m_frame_animation.push_back({24,122,71,85});


    m_animations->create(MyKindsAnimations::L_CHANGING_TO_REVERSAL,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,3.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({266,85,84,82});
    m_frame_animation.push_back({184,80,80,87});
    m_frame_animation.push_back({95,82,84,78});
    m_frame_animation.push_back({0,90,91,69});

    m_animations->create(MyKindsAnimations::L_SPECIAL_SIN_FIRE_BALL_ATACK,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,2.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({203,85,84,82});
    m_frame_animation.push_back({289,80,80,87});
    m_frame_animation.push_back({374,82,84,78});
    m_frame_animation.push_back({462,90,91,69});

    m_animations->create(MyKindsAnimations::R_SPECIAL_SIN_FIRE_BALL_ATACK,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,2.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({467,286,84,78});
    m_frame_animation.push_back({368,295,91,69});
    m_frame_animation.push_back({276,288,92,76});
    m_frame_animation.push_back({191,277,81,87});
    m_frame_animation.push_back({112,267,67,97});
    m_frame_animation.push_back({6,277,81,86});

    m_animations->create(MyKindsAnimations::L_SPECIAL_CIRCLE_FIRE_BALL_ATACK,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,2.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({2,286,84,78});
    m_frame_animation.push_back({94,295,91,69});
    m_frame_animation.push_back({185,288,92,76});
    m_frame_animation.push_back({281,277,81,87});
    m_frame_animation.push_back({374,267,67,97});
    m_frame_animation.push_back({466,277,81,86});

    m_animations->create(MyKindsAnimations::R_SPECIAL_CIRCLE_FIRE_BALL_ATACK,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,2.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({277,185,83,82});
    m_frame_animation.push_back({182,186,91,81});
    m_frame_animation.push_back({97,175,80,92});
    m_frame_animation.push_back({19,167,63,100});

    m_animations->create(MyKindsAnimations::L_BASIC_FIRE_BALL_ATACK,CMarioGame::s_texture_manager["Bowser_left"],m_frame_animation,2.0f,m_my_scal);

    ///-----
    m_frame_animation.clear();
    m_frame_animation.push_back({193,185,83,82});
    m_frame_animation.push_back({280,186,91,81});
    m_frame_animation.push_back({376,175,80,92});
    m_frame_animation.push_back({471,167,63,100});

    m_animations->create(MyKindsAnimations::R_BASIC_FIRE_BALL_ATACK,CMarioGame::s_texture_manager["Bowser_right"],m_frame_animation,2.0f,m_my_scal);
}


