#pragma once
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <map>
#include <memory>

using namespace std;

class CAnimator
{
public:

    virtual ~CAnimator(){}

    void draw(const unique_ptr<sf::RenderWindow>&window){window->draw(*m_current_frame);}
    virtual void update(const sf::Vector2f& )=0;

    void setPosition(const sf::Vector2f& pos){m_current_frame->setPosition(pos);}

    sf::FloatRect getGlobalBounds()const {return m_current_frame->getGlobalBounds();}
    sf::Vector2f getPosition()const {return m_current_frame->getPosition();}
    sf::Sprite& getSprite()const {assert(m_current_frame);return *m_current_frame;}

protected:

     sf::Sprite * m_current_frame=nullptr;
};

///-------------------------------------------------------------///
class CSprite: public CAnimator
{
public:

    CSprite(sf::Sprite* sprite){m_current_frame=sprite;}
    ~CSprite(){delete m_current_frame;}

    void update(const sf::Vector2f& pos)override {m_current_frame->setPosition(pos);}
};

///-------------------------------------------------------------///
class CAnimations: public CAnimator
{
    int m_current_animation=-1;
    map<int,pair< vector<sf::Sprite>,float> > m_animations;

    int m_index_frame=0;
    int m_count_frames=0;
    float m_current_speed=0;
    float m_timer=0.0f;
    bool m_last_frame=false;

    inline void setCurrentFrame();

public:

    enum BasicKindsAnimations
    {
        L_MOVE=1000, /// BARDZO WAZNE BO W ENUMACH KLAS POCHODNYCH NIE MOGA ZACZYNAC SIE OD TEGO SAMEGO 0
        R_MOVE,
        L_STANDING,
        R_STANDING,
        L_JUMP,
        R_JUMP,
        STANDARD
    };

    ~CAnimations(){}

	void create(int,const sf::Texture&,const sf::IntRect&,float,bool=true);
	void create(int,const sf::Texture&,const std::vector<sf::IntRect>&,float,float,bool=true);

    void update(const sf::Vector2f&)override;
    void update();

    void play(int,const sf::Vector2f&);
    void reset(){m_animations.clear();m_current_frame=nullptr;}

    void setSpeed(float);
    void setIndexFrame(int,const sf::Vector2f&);

    int getIndexFrame()const{return m_index_frame;}
    int getAnimationName()const {return m_current_animation;}
    bool islastFrame()const {return m_last_frame;}
};
