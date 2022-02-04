#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <assert.h>

using namespace std;

template<typename T>
class ResourceManager
{
public:
    ResourceManager()=default;
    ResourceManager(const ResourceManager&)=delete;
    ResourceManager operator =(const ResourceManager&)=delete;
    virtual ~ResourceManager(){}

    virtual void loadFromFile(string ,const string &);
    const T& operator[](const string &);
    const T& get(const string&);

protected:
    map<string ,unique_ptr<T>> m_resources;

};


///------
template<typename T>
void ResourceManager<T>::loadFromFile(string name,const string &way_to_file)
{
    assert(m_resources.find(name)==m_resources.end());
    m_resources[name]=unique_ptr<T>(new T);

    if(!m_resources[name]->loadFromFile(way_to_file))
        throw runtime_error(("NIE MOGE ZALADOWAC PLIKU "+way_to_file).c_str());
}

///------
template<>
inline void ResourceManager<sf::Music>::loadFromFile(string name,const string &way_to_file)
{
    assert(m_resources.find(name)==m_resources.end());
    m_resources[name]=unique_ptr<sf::Music>(new sf::Music);

    if(!m_resources[name]->openFromFile(way_to_file))
        throw runtime_error(("NIE MOGE ZALADOWAC PLIKU  "+way_to_file).c_str());

    m_resources[name]->setLoop(true);
}

///------
template<typename T>
const T& ResourceManager<T>:: operator[](const string &name)
{
    assert(m_resources.find(name)!=m_resources.end());
    return (*m_resources[name]);
}

///------
template<typename T>
const T& ResourceManager<T>::get(const string &name)
{
    assert(m_resources.find(name)!=m_resources.end());
    return (*m_resources[name]);
}

///-------------------///
using CTextureManager=ResourceManager<sf::Texture>;
using CFontManager=ResourceManager<sf::Font>;

class CMusicVolumeControler
{
protected:

    float m_volume=60;

public:

    float getVolume()const {return m_volume;}
    void updateVolume(bool is_louder)
    {
        if(is_louder)
        {
            if(m_volume<100)
                m_volume+=10;
        }else
        {
            if(m_volume>0)
                m_volume-=10;
        }
    }
};

///----------
class CMusicManager:public ResourceManager<sf::Music>,public CMusicVolumeControler
{
    vector<string> m_played_musics;

    inline bool isPlayingMusic()const{return m_played_musics.size()!=0;}
    sf::Music& getCurrentMusic();

public:
    ~CMusicManager(){}

    void play(string);
    void pause();
    void resume();
    void stop();
    void updateMusicVolume(bool);
    void playToDefineVolume();
    void resetAllMusic();
};

///-----------
class CSoundManager:public ResourceManager<sf::SoundBuffer>,public CMusicVolumeControler
{
    std::vector<sf::Sound> m_sounds;

public:

    CSoundManager():m_sounds(25){}
    ~CSoundManager(){}

    void play(string);
    void stop();
    void pause();
    void resume();
};

