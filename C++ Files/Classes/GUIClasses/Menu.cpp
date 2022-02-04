#include "Menu.h"
#include "../EventHandler.h"
#include "../Scens/Scen.h"
#include "GUI.h"
#include "../MarioGame.h"

using namespace std;

const float CMenu::s_value_margin=50.0f;

/// POZWALA NA WPROWADZENIE JAKIEGOS OPISU DO MENU W FORMIE TYTULU
CMenu::CMenu(bool vertical_control,vector<CLabel*>& labels,sf::RectangleShape * background,sf::Text * title,string texture_name)
:m_vertical_control(vertical_control)
{
    ///------
    if(background)
        m_background.reset(background);

    ///-----
    for(auto label:labels)
        m_my_labels.push_back(unique_ptr<CLabel>(label));

    ///-----
    m_pointer_to_label.reset(new CPointerToLabel(m_my_labels,texture_name));

    ///-----
    if(title)
        m_title.reset(title);
}

///------
CMenu::CMenu(bool vertical_control,vector<CLabel*>& labels,sf::RectangleShape * background,string name_title,bool double_size,string texture_name)
:m_vertical_control(vertical_control)
{
    ///------
    if(background)
        m_background.reset(background);

    ///-----
    for(auto label:labels)
        m_my_labels.push_back(unique_ptr<CLabel>(label));

    ///-----
    m_pointer_to_label.reset(new CPointerToLabel(m_my_labels,texture_name));

    ///-----
    setTitle(name_title,double_size);
}

///---
CLabel* CMenu::createOptionLabel(const sf::Vector2f pos_label,const sf::Vector2f pos_option_menu,const sf::Color color)
{
    std::function<void(int)> action_create_menu=[pos_option_menu](int ascii)
    {

    const sf::IntRect bounds_menu(pos_option_menu.x,pos_option_menu.y,550,500);

    const sf::Vector2f first_label_pos(bounds_menu.left-bounds_menu.width/2.0f+CMenu::s_value_margin,
    bounds_menu.top-bounds_menu.height/2.0f+CMenu::s_value_margin/2.0f);

    ///---- TWORZE ETYKIETKI I POZNIEJ BD DODAWAL DO NICH AKCJE

    std::vector<CLabel*> m_options_collection;

    const int number_labels=8;
    string names[number_labels]={"VOLUME SOUND","VOLUME MUSIC","LEFT RUN","RIGHT RUN","JUMP","CROUCH","SHOOT","DEFAULT STEERAGE"};

    for(int i=0;i<number_labels;i++)
    {
        sf::Text * name;

        if(i<number_labels-1)
            name=CGUI::createText(names[i],{first_label_pos.x,first_label_pos.y+CMenu::s_value_margin*i},sf::Color(250,250,255,100),"menu_font",false);
        else
            name=CGUI::createText(names[i],{first_label_pos.x,first_label_pos.y+CMenu::s_value_margin*i+15},sf::Color(250,250,255,100),"menu_font",false);

        m_options_collection.push_back(new CLabel(name));
    }

    ///----- SOUND VOLUME LABEL
    m_options_collection[0]->addRectangleShap(CGUI::createRectangleShape(sf::IntRect(first_label_pos.x+260,first_label_pos.y,10*20+2*5,25),sf::Color::Black,false,true,sf::Color(250,250,255,100)));
    m_options_collection[0]->addRectangleShap(CGUI::createRectangleShape(sf::IntRect(first_label_pos.x+265,first_label_pos.y+5,(CMarioGame::s_sound_manager.getVolume()/10)*20,15),sf::Color(250,250,255,100),false));

    /// AKCJA DLA SOUND VOLUME LABEL
    m_options_collection[0]->addAction([m_options_collection](int ascii){

    const unique_ptr<sf::RectangleShape>& rctn=m_options_collection[0]->getRectangleShape(1);

    if(CGUI::isLabelMaintenance()&&ascii==(int)(sf::Keyboard::Return))
    {
        m_options_collection[0]->getRectangleShape(0)->setOutlineColor(m_options_collection[0]->getBasicColor());
        rctn->setFillColor(m_options_collection[0]->getBasicColor());

        CGUI::setLabelMaintenance(false);
        return;
    }

    if(!CGUI::isLabelMaintenance())
    {
        m_options_collection[0]->getRectangleShape(0)->setOutlineColor(sf::Color::White);
        rctn->setFillColor(sf::Color::White);

        CMarioGame::s_sound_manager.play("coin");
        CGUI::setLabelMaintenance(true);
    }

    if(ascii==(int)(sf::Keyboard::Left)&&rctn->getSize().x>0)
    {
        rctn->setSize({rctn->getSize().x-20,rctn->getSize().y});
        CMarioGame::s_sound_manager.updateVolume(false);
        CMarioGame::s_sound_manager.play("coin");
    }
    if(ascii==(int)(sf::Keyboard::Right)&&rctn->getSize().x<200)
    {
        rctn->setSize({rctn->getSize().x+20,rctn->getSize().y});
        CMarioGame::s_sound_manager.updateVolume(true);
        CMarioGame::s_sound_manager.play("coin");
    }

    });

    ///----- MUSIC VOLUME LABEL
    m_options_collection[1]->addRectangleShap(CGUI::createRectangleShape(sf::IntRect(first_label_pos.x+260,first_label_pos.y+CMenu::s_value_margin,10*20+2*5,25),sf::Color::Black,false,true,sf::Color(250,250,255,100)));
    m_options_collection[1]->addRectangleShap(CGUI::createRectangleShape(sf::IntRect(first_label_pos.x+265,first_label_pos.y+5+CMenu::s_value_margin,(CMarioGame::s_music_manager.getVolume()/10)*20,15),sf::Color(250,250,255,100),false));

    /// AKCJA DLA MUSIC LABEL
    m_options_collection[1]->addAction([m_options_collection](int ascii){

    const unique_ptr<sf::RectangleShape>& rctn=m_options_collection[1]->getRectangleShape(1);

    if(CGUI::isLabelMaintenance()&&ascii==(int)(sf::Keyboard::Return))
    {
        m_options_collection[1]->getRectangleShape(0)->setOutlineColor(m_options_collection[1]->getBasicColor());
        rctn->setFillColor(m_options_collection[1]->getBasicColor());

        CGUI::setLabelMaintenance(false);
        CMarioGame::s_music_manager.stop();
        return;
    }

    if(!CGUI::isLabelMaintenance())
    {
        m_options_collection[1]->getRectangleShape(0)->setOutlineColor(sf::Color::White);
        rctn->setFillColor(sf::Color::White);

        CMarioGame::s_music_manager.playToDefineVolume();
        CGUI::setLabelMaintenance(true);
    }

    if(ascii==(int)(sf::Keyboard::Left)&&rctn->getSize().x>0)
    {
        rctn->setSize({rctn->getSize().x-20,rctn->getSize().y});
        CMarioGame::s_music_manager.updateMusicVolume(false);
    }
    if(ascii==(int)(sf::Keyboard::Right)&&rctn->getSize().x<200)
    {
        rctn->setSize({rctn->getSize().x+20,rctn->getSize().y});
        CMarioGame::s_music_manager.updateMusicVolume(true);
    }

    });

    ///----AKCJE DLA STEERAGE LABELS
    for(int i=2;i<number_labels-1;i++)
    {
       CEventHandler::DecisionsPlayer my_decision=static_cast<CEventHandler::DecisionsPlayer>(i-2);
       string inscription=CEventHandler::convertKeyToString(CEventHandler::getRegisteredKeys()[my_decision]);

       m_options_collection[i]->addText(CGUI::createText(inscription,{first_label_pos.x+260,first_label_pos.y+CMenu::s_value_margin*i},sf::Color(250,250,255,100),"menu_font",false));

       m_options_collection[i]->addAction([i,m_options_collection,my_decision](int ascii)
        {
            const unique_ptr<sf::Text>& m_text=m_options_collection[i]->getText();

            if(CGUI::isLabelMaintenance())
            {
                if(ascii!=(int)(sf::Keyboard::Return))
                {
                    string new_text=CEventHandler::convertKeyToString(ascii);
                    if(new_text==""||CEventHandler::isOccupyThisKeyCode(ascii))return;

                    CEventHandler::changeRegisteredKeys(my_decision,ascii);

                    m_text->setString(new_text);

                }else
                {
                    m_text->setFillColor(m_options_collection[i]->getBasicColor());
                    CGUI::setLabelMaintenance(false);
                }
                return;
            }
            else
            {
                m_text->setFillColor(sf::Color::White);
                CGUI::setLabelMaintenance(true);
            }

        });
    }

    /// AKCJA DLA DEFAULT STEERAGE
    m_options_collection[number_labels-1]->addAction([m_options_collection,number_labels](int ascii){

    if(ascii==(int)(sf::Keyboard::Return))
    {
        CEventHandler::setDefaultRegisteredKeys();

        for(int i=2;i<number_labels-1;i++)
        {
            const string inscription=CEventHandler::convertKeyToString(CEventHandler::getRegisteredKeys()[static_cast<CEventHandler::DecisionsPlayer>(i-2)]);

            m_options_collection[i]->getText()->setString(inscription);
        }
    }

    });

    /// DODAJE ETYKIETKI POWROTU
    m_options_collection.push_back(CMenu::createReturnLabel(CGUI::createText("MAIN MENU",{first_label_pos.x,first_label_pos.y+CMenu::s_value_margin*number_labels+20},sf::Color(250,250,255,100),"menu_font",false)));

    /// TWORZE MENI
    sf::RectangleShape * backgorund=CGUI::createRectangleShape(bounds_menu,sf::Color::Black,true,true);
    CGUI::addMenu(new CMenu(true,m_options_collection,backgorund,nullptr));

    };

    ///--------------ZWRACAM STWORZONA ETYKIETKE----------------///
    sf::Text *name=CGUI::createText("OPTIONS",pos_label,color,"menu_font");
    return new CLabel(name,action_create_menu);
}

///---
CLabel* CMenu::createReturnLabel(sf::Text*text)
{
    CLabel* m_return_label=new CLabel(text,[](int ascii)
    {
        CGUI::removeCurrentMenu();
    });

    return m_return_label;
}

///----
void CMenu::setTitle(string text,bool double_size)
{
    if(!m_background)
        return;

    const sf::FloatRect &background_bounds=m_background->getGlobalBounds();

    sf::Vector2f pos_title={background_bounds.left+background_bounds.width/2.0f,background_bounds.top+CMenu::s_value_margin};
    size_t size_text=CGUI::s_basic_size_text;

    if(double_size)
    {
        size_text*=1.6;
        pos_title.x+=5;
    }
    else
        size_text*=1.2;

    m_title.reset(CGUI::createText(text,pos_title,sf::Color::Red,"menu_font",true,size_text));
}

///----
void CMenu::draw(const std::unique_ptr<sf::RenderWindow>&window)
{
    if(m_background)
        window->draw(*m_background);

    if(m_title)
        window->draw(*m_title);

    m_pointer_to_label->draw(window);

    for(auto &obj:m_my_labels)
        obj->draw(window);
}

