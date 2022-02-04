#include "Coin.h"
#include "../../Scens/GameScen.h"
#include "../../GUIClasses/FlowText.h"

CAnimations CCoin::s_static_animator;
int CCoin::s_count_gathered_coins=0;

CCoin::CCoin(sf::Vector2f pos)
:CGameObject(new CSprite(CGUI::createSprite("Items",{0,0,14,30},pos,m_scale_to_tile,true)),Parentage::ITEM,pos)
{
}

///-----
void CCoin::actOnObject(CGameObject* obj,KindCollision kind_collision)
{
    if(obj->getParentage()==Parentage::MARIO)
        actOnMe(KindAction::HIT);
}

///----
void CCoin::actOnMe(KindAction which_action)
{
    if(which_action==KindAction::HIT)
    {
        CAnimations * falling_apart_animation=creatFallingApartCoinAnimation();
        m_current_position.y-=CScen::s_tile_size/2.0f;
        falling_apart_animation->setPosition(m_current_position);

        CGuiObject* obj=new CSpecialEffects(falling_apart_animation,CSpecialEffects::KindUpdate::ONE_LOOP_ANIMATION,0);

        CGUI::addGuiObject(obj);

        CMarioGame::instance().getScen<CGameScen>().addPoints(m_my_value_points);

        CMarioGame::s_sound_manager.play("coin");

        s_count_gathered_coins++;
        CGUI::setCoinCounter(s_count_gathered_coins);

        removeObject(this);
    }
}

///------
void CCoin::draw(const unique_ptr<sf::RenderWindow>& window)
{
    s_static_animator.setPosition(m_current_position);
    s_static_animator.draw(window);
}

///-----
void CCoin::setStaticAnimation()
{
    CAnimations *animation=creatFlashingCoinAnimation();
    s_static_animator=*animation;
    s_static_animator.play(CAnimations::STANDARD,{0,0});

    delete animation;
}

///------
CAnimations* CCoin::creatFallingApartCoinAnimation()
{
    CAnimations* animation = new CAnimations();
    vector<sf::IntRect> m_frames_animation;

    m_frames_animation.push_back(sf::IntRect(12,123,15,15));
    m_frames_animation.push_back(sf::IntRect(44,115,34,33));
    m_frames_animation.push_back(sf::IntRect(81,115,40,32));
    m_frames_animation.push_back(sf::IntRect(120,115,42,32));
    m_frames_animation.push_back(sf::IntRect(161,121,38,24));

    animation->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Items"],m_frames_animation,2.0f,m_scale_to_tile,true);
    return animation;
}

///-----
CAnimations* CCoin::creatFlashingCoinAnimation()
{
    CAnimations *animation=new CAnimations();
    vector<sf::IntRect> m_frames_animation;

    m_frames_animation.push_back(sf::IntRect(9,85,14,30));
    m_frames_animation.push_back(sf::IntRect(39,85,20,30));
    m_frames_animation.push_back(sf::IntRect(74,85,14,30));
    m_frames_animation.push_back(sf::IntRect(110,85,10,30));

    animation->create(CAnimations::STANDARD,CMarioGame::s_texture_manager["Items"],m_frames_animation,3.0f,m_scale_to_tile,true);
    return animation;
}

