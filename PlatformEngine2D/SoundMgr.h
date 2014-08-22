#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>
#include "IObjectManager.h"
#include <SFML/Audio.hpp>


class SoundContainer
{
	friend class SoundMgr;

public:

	SoundContainer( sf::SoundBuffer* sndBuffer ) 
	:m_isDestroyWhenFinished(false)
	{
		m_sound = new sf::Sound(*sndBuffer);
	}
	~SoundContainer(){ delete m_sound; }

	sf::Sound* GetSound(){ return  m_sound; };

protected:

	sf::Sound*	m_sound;
	bool		m_isDestroyWhenFinished;
};

class SoundMgr : protected IObjectManager<SoundContainer>
{
public:

	SoundMgr() 
	: IObjectManager( ((unsigned int)-1) - 1 ) // reserve 1 bit for invalid id -1
	{}

	void Update();

	int CreateSound( const std::string& sSoundPath );
	void DestroySound( int id );

	void PlaySound( const std::string& sSoundPath, float volume = 100 );

	sf::Sound* GetSound( int id );

	void SetGlobalPitch( float pitch);

private:

	IObjectManager::vObject_t m_vSounds; // sounds destroyed once are finished
};