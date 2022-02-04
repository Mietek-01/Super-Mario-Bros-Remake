#include "MysterBox.h"
#include "../../GUIClasses/FlowText.h"
#include "../../Scens/GameScen.h"
#include "../Items/Coin.h"

CAnimations CMysterBox::s_private_animator;

CMysterBox::CMysterBox(sf::Vector2f pos,MyItem my_item)
:CDynamicBlock({0,32,32,32},pos)
,m_rand_item(my_item)
{
}

///---------
void CMysterBox::update()
{
    if(m_hit)
    {
        this->makeJump();

        if(m_force>0)
            m_draw_question_mark=false;
    }

    if(m_createing_item)
    {
        if(m_rand_item==MyItem::COIN)
        {
            m_to_rand=m_createing_item=false;

            CAnimations * coin_animation=CCoin::creatFlashingCoinAnimation();

            coin_animation->setPosition({m_current_position.x,m_current_position.y-getSize().y});

            CSpecialEffects * obj=new CSpecialEffects(coin_animation,CSpecialEffects::KindUpdate::JUMP,m_current_position.y-getSize().y,{0,-600});

            ///-----------------------

            CAnimations * falling_apart_coin_animation=CCoin::creatFallingApartCoinAnimation();

            falling_apart_coin_animation->setPosition({m_current_position.x,m_current_position.y-10.0f-getSize().y});

            obj->setDeathAnimation(new CSpecialEffects(falling_apart_coin_animation,CSpecialEffects::KindUpdate::ONE_LOOP_ANIMATION,0));

            CGUI::addGuiObject(obj);

            ///------------------------------

            sf::Text *text=CGUI::createText(CGUI::toString(CCoin({0,0}).getValuePoints()), {m_current_position.x,m_current_position.y-getSize().y-CScen::s_tile_size},sf::Color::Red);
            CGUI::addGuiObject(new CFlowText(text,m_current_position.y-getSize().y-CScen::s_tile_size*3));

            CMarioGame::instance().getScen<CGameScen>().addPoints(CCoin({0,0}).getValuePoints());

            CMarioGame::s_sound_manager.play("coin");

        }
        else
        {
            if(!m_hit)
            {
                if(!m_my_obj)
                {
                    switch(m_rand_item)
                    {
                    case MyItem::MUSHROOM:
                        m_my_obj=new CMushroom(m_current_position);
                        break;
                    case MyItem::FLOWER:
                        m_my_obj=new CSpecialFlower(m_current_position);
                        break;
                    }

                    CMarioGame::s_sound_manager.play("powerup_appears");
                }
                else
                {
                    if(m_my_obj->getCurrentPosition().y>this->m_current_position.y-getSize().y)
                         m_my_obj->movePosition({0,m_leaving_speed});
                    else
                    {
                        m_my_obj->setPosition({m_my_obj->getCurrentPosition().x,this->m_current_position.y-getSize().y+0.1f});
                        m_my_obj->setPreviousPosition({m_my_obj->getCurrentPosition().x,this->m_current_position.y-getSize().y-0.2f});

                        addNewObject(m_my_obj);

                        m_to_rand=m_createing_item=false;
                        m_my_obj=nullptr;
                    }
                }
            }
        }
    }
}

///----------
void CMysterBox::actOnMe(KindAction which_action)
{
    /// ZAPOBIEGA UDERZANIU GDY GRZYB WYCHODZI
    if(m_my_obj)
        return;

    if(m_to_rand)
        m_createing_item=true;

    m_hit=true;
}

///----------
void CMysterBox::draw(const unique_ptr<sf::RenderWindow>& window)
{
    if(m_draw_question_mark)
    {
        s_private_animator.setPosition(m_current_position);
        s_private_animator.draw(window);

    }else
    {
        if(m_my_obj)
            m_my_obj->draw(window);

        m_animator->setPosition(m_current_position);
        m_animator->draw(window);
    }
}

///----------
void CMysterBox::setStaticAnimation()
{
    vector<sf::IntRect> m_frames_animation;

    for(int i=0;i<4;i++)
    m_frames_animation.push_back({32*i,0,32,32});

    s_private_animator.create(CAnimations::STANDARD,CMarioGame::s_texture_manager["AnimTiles"],m_frames_animation,3.0f,m_scale_to_tile,true);
}
