#include "FlowText.h"

///------
CFlowText::CFlowText(sf::Text* text,float when_remove,float speed_changing)
:m_text(text)
,CGuiObject(when_remove)
,m_speed_changing_transparency(speed_changing)
{
    m_color=m_text->getFillColor();

    /// LOWER - PRZEZROCZYSTOSC MA MALEC
    /// TALLER - MA ROSNAC

    /// ZNAK JEST MI POTRZEBNY TYLKO BY OKRESLIC ODPOWIEDNIA OPCJE
    if(m_speed_changing_transparency<0)
        m_kind_changing_transparency=KindChangingTransparency::LOWER;
    else
        if(m_speed_changing_transparency>0)
       m_kind_changing_transparency=KindChangingTransparency::TALLER;
    else
        m_kind_changing_transparency=KindChangingTransparency::NONE;
}

///----
CFlowText::CFlowText(sf::Text* text,std::function<bool(unique_ptr<sf::Text>&)> special_update,float speed_changing)
:m_text(text)
,m_speed_changing_transparency(speed_changing)
{
    m_special_update=special_update;
    m_color=m_text->getFillColor();

    if(m_speed_changing_transparency<0)
        m_kind_changing_transparency=KindChangingTransparency::LOWER;
    else
        if(m_speed_changing_transparency>0)
       m_kind_changing_transparency=KindChangingTransparency::TALLER;
    else
        m_kind_changing_transparency=KindChangingTransparency::NONE;
}

///--------------
void CFlowText::update()
{
    if(m_special_update)
        m_remove=m_special_update(m_text);
    else
    {
        m_text->move(0,-1);
        if(m_text->getPosition().y<=m_when_remove)
            m_remove=true;
    }

    /// WYKONUJE WYBRANY RODZAJ ZMIANY PRZEZROCZYSTOSCI

    switch(m_kind_changing_transparency)
    {
    case KindChangingTransparency::LOWER:
        {
            m_color.a=255+m_value_transparency;
            m_text->setFillColor(m_color);

            m_value_transparency+=m_speed_changing_transparency;

            if(m_value_transparency<-255)
            {
                m_color.a=0;
                m_text->setFillColor(m_color);
                m_kind_changing_transparency=KindChangingTransparency::NONE;
            }

            break;
        }

    case KindChangingTransparency::TALLER:
        {
            m_color.a=0+m_value_transparency;
            m_text->setFillColor(m_color);

            m_value_transparency+=m_speed_changing_transparency;

            if(m_value_transparency>255)
            {
                m_color.a=255;
                m_text->setFillColor(m_color);
                m_kind_changing_transparency=KindChangingTransparency::NONE;
            }

            break;
        }

    }
}
