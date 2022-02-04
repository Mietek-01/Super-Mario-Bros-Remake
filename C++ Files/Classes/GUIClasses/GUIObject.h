#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

using namespace std;


class CGuiObject
{
protected:
    bool m_remove=false;
    const float m_when_remove;/// ALBO PO JAKIM CZASIE ALBO GDZIE

public:

    virtual ~CGuiObject(){}
    CGuiObject():m_when_remove(0.0f){}
    CGuiObject(float when_remove):m_when_remove(when_remove){}

    virtual void draw(unique_ptr<sf::RenderWindow>&)=0;
    virtual void update()=0;

    bool isRemove()const{return m_remove;}

};
