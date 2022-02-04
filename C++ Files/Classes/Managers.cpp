#include "Managers.h"
#include <iostream>
#include "GUIClasses/GUI.h"

void CMusicManager::play(string name)
{
    assert(m_resources.find(name)!=m_resources.end());

    if(isPlayingMusic())
        getCurrentMusic().pause();

    m_played_musics.push_back(name);

    getCurrentMusic().play();
    getCurrentMusic().setVolume(m_volume);
}

///-------
void CMusicManager::playToDefineVolume()
{
    bool findig=true;

    while(findig)
    {

        const int which_music=CGUI::rand(m_resources.size(),0);
        int i=0;

        for(auto it=m_resources.begin();;it++,i++)
            if(i==which_music)
            {
                if(m_played_musics.end()==find(m_played_musics.begin(),m_played_musics.end(),(*it).first))
                {
                    m_played_musics.push_back((*it).first);
                    findig=false;
                }

                break;
            }
    }

    getCurrentMusic().play();
    getCurrentMusic().setVolume(m_volume);
}

///--------
sf::Music& CMusicManager::getCurrentMusic()
{
    assert(isPlayingMusic());

    return *m_resources[m_played_musics.back()];
}

///-----------
void CMusicManager::stop()
{
    if(isPlayingMusic())
    {
        getCurrentMusic().stop();
        m_played_musics.pop_back();
    }
}

///------
void CMusicManager::pause()
{
    if(isPlayingMusic())
        getCurrentMusic().pause();
}

///---
void CMusicManager::resume()
{
    if(isPlayingMusic())
    {
        getCurrentMusic().play();
        getCurrentMusic().setVolume(m_volume);
    }
}

///----------
void CMusicManager::updateMusicVolume(bool is_louder)
{
    if(!isPlayingMusic())
        return;

    updateVolume(is_louder);

    getCurrentMusic().setVolume(m_volume);
}

///----------
void CMusicManager::resetAllMusic()
{
    for(auto music_name:m_played_musics)
        m_resources[music_name]->stop();

    m_played_musics.clear();
}

///------------------------CSoundManager-------------///

void CSoundManager::play(string name)
{
  assert(m_resources.find(name)!=m_resources.end());

  for(auto &sound:m_sounds)
    if(sound.getStatus()==sf::Sound::Stopped)
    {
        sound.setBuffer(*m_resources[name]);
        sound.setVolume(m_volume);
        sound.play();
        return;
    }

    cout<<"----------------ZA DUZO DZWIEKOW--------------"<<endl;
    assert(0);
}

///-----------
void CSoundManager::stop()
{
   for(auto &sound:m_sounds)
    sound.stop();
}

///------
void CSoundManager::pause()
{
   for(auto &sound:m_sounds)
    if(sound.getStatus()==sf::Sound::Playing)
        sound.pause();
}

///---
void CSoundManager::resume()
{
  for(auto &sound:m_sounds)
    if(sound.getStatus()==sf::Sound::Paused)
        sound.play();
}



