#include "Menu.h"
#include "../MarioGame.h"

const sf::Vector2f CMenu::CPointerToLabel::s_sprite_offset_poz(-50,-7);

CMenu::CPointerToLabel::CPointerToLabel(REF_TO_LABELS ptr,const string &texture_name)
:m_pointer_to_labels(ptr)
{
    if(texture_name!="none")
      m_sprite.reset(new sf::Sprite(CMarioGame::s_texture_manager[texture_name]));

    update();
}

///------
void CMenu::CPointerToLabel::switchPointer(bool up_or_left)
{
    if(!m_sprite)
        getCurrentLabel()->setOnBasicColor();

    if(up_or_left)
    {
        if(m_current_label>0)
           m_current_label--;
    }else
        if(m_current_label<m_pointer_to_labels.size()-1)
           m_current_label++;

    update();
}

///------
const unique_ptr<CLabel>& CMenu::CPointerToLabel::getCurrentLabel()
{
    assert(m_pointer_to_labels.size()!=0);
    return m_pointer_to_labels[m_current_label];
}

///------
void CMenu::CPointerToLabel::update()
{
    if(m_sprite)
    {
        m_sprite->setPosition(getCurrentLabel()->getPosition());
        m_sprite->move(s_sprite_offset_poz);

    }else
        getCurrentLabel()->setColor(sf::Color::White);
}

///-----
void CMenu::CPointerToLabel::draw(const unique_ptr<sf::RenderWindow>&window)
{
  if(m_sprite)
    window->draw(*m_sprite);
}
