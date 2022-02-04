#include "Animator.h"

///-----------------------------
void CAnimations::create(int name, const sf::Texture& texture, const sf::IntRect& rect,float m_scale,bool bottom_origin)
{
    assert(m_animations.end()==m_animations.find(name));

    sf::Sprite m_sprite_to_load(texture,rect);

    if(bottom_origin)
        m_sprite_to_load.setOrigin(rect.width/2.0f,rect.height);

    m_sprite_to_load.setScale(m_scale,m_scale);

    m_animations[name]={{m_sprite_to_load},0.0f};

    ///DOMYSLNIE
    play(name,{0,0});
}

///-----------------------
void CAnimations::create(int name, const sf::Texture& texture, const std::vector<sf::IntRect>& rects, float speed,float m_scale,bool bottom_origin)
{
    assert(m_animations.end()==m_animations.find(name));
    assert(speed>0);

    vector<sf::Sprite> m_sprites;

    for(const auto &frames:rects)
    {
        sf::Sprite m_sprite_to_load(texture,frames);

        if(bottom_origin)
            m_sprite_to_load.setOrigin(frames.width/2.0f,frames.height);

        m_sprite_to_load.setScale(m_scale,m_scale);
        m_sprites.push_back(m_sprite_to_load);
    }

    m_animations[name]={m_sprites,speed};

    ///DOMYSLNIE
    play(name,{0,0});
}

///-------------------
void CAnimations::update(const sf::Vector2f& pos)
{
    this->update();
    m_current_frame->setPosition(pos);
}

///-------------------
void CAnimations::update()
{
    /// MECHANIZM ZMIENIANIA KLATEK CO OKRESLONY CZAS

    if(m_count_frames>1)
    {
        m_timer+=0.1f;
        if(m_timer>=m_current_speed)
        {
            m_index_frame++;
            if(m_index_frame>=m_count_frames)
            {
                m_index_frame=0;
                m_last_frame=true;
            }
            else
                m_last_frame=false;

            setCurrentFrame();
            m_timer=0.0f;
        }
    }
}

///---------------------
void CAnimations::setSpeed(float new_speed)
{
    assert(new_speed>0);
    assert(m_animations.end()!=m_animations.find(m_current_animation));

    m_current_speed=m_animations[m_current_animation].second=new_speed;
}

///---------
void CAnimations::play(int name,const sf::Vector2f& pos)
{
    assert(m_animations.end()!=m_animations.find(name));

    m_current_animation=name;
    m_index_frame=0;
    m_current_speed=m_animations[m_current_animation].second;
    m_timer=0.0f;
    m_count_frames=(m_animations[m_current_animation].first).size();

    /// GDY ILOSC KLATEK DANEJ ANIMACJI WYNOSI 1 TO OKRESLAM ZE JEST TO
    /// OSTATNIA KLATKA
    m_last_frame=m_count_frames==1;

    setCurrentFrame();

    m_current_frame->setPosition(pos);
}

///-----------------------
inline void CAnimations::setCurrentFrame()
{
     m_current_frame=&(m_animations[m_current_animation].first)[m_index_frame];
}

///-----------
void CAnimations::setIndexFrame(int new_index,const sf::Vector2f &pos)
{
    assert(new_index>=0&&new_index<m_count_frames);

    m_index_frame=new_index;
    setCurrentFrame();

    m_current_frame->setPosition(pos);
}

