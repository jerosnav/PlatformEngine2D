
#include "SoundMgr.h"

void SoundMgr::Update()
{
	vObject_t::iterator iter = m_vSounds.begin();
	vObject_t::iterator iter_end = m_vSounds.end();
	while( iter != iter_end )
	{
		object_t* objContainer = (*iter);
		SoundContainer* soundContainer	= objContainer->GetData();
		sf::Sound* sound = soundContainer->GetSound();

		if( objContainer->IsMarkedForDeletion() )
		{
			IObjectManager::FreeObjectData( objContainer );

			//fast erase from temporal sounds
			if( m_vSounds.size() > 1 )
			{
				std::swap( *iter, m_vSounds.back());
				iter = m_vSounds.erase( m_vSounds.end()  - 1 );
			}	
			else
			{
				iter = m_vSounds.erase( iter );
			}
			iter_end = m_vSounds.end();
		}
		else
		{
			if( soundContainer->m_isDestroyWhenFinished && (sound->GetStatus() != sf::Sound::Playing) )
			{
				DestroySound( objContainer->GetId() );			
			}
			++iter;
		}
	}
}

int SoundMgr::CreateSound( const std::string& sSoundPath )
{
	// Load a sound buffer from a wav file
    sf::SoundBuffer* sndBuffer = new sf::SoundBuffer();
    
	//if (!Buffer.LoadFromFile("footsteps.wav"))
	if (!sndBuffer->LoadFromFile( sSoundPath ))
	{
        return -1;
	}

	SoundContainer* soundContainer = new SoundContainer( sndBuffer );

	object_t* objContainer = IObjectManager::AddObjectData( soundContainer, sSoundPath );
	
	m_vSounds.push_back( objContainer );

	return objContainer->GetId();
}

void SoundMgr::DestroySound( int id )
{
	IObjectManager::Destroy( id, true );
}

sf::Sound* SoundMgr::GetSound( int id )
{
	object_t* objContainer = IObjectManager::GetById( id );
	return objContainer? objContainer->GetData()->GetSound() : 0;
}

void SoundMgr::PlaySound( const std::string& sSoundPath, float volume /*= 100*/ )
{
	int id = CreateSound( sSoundPath );
	object_t* objContainer = IObjectManager::GetById( id );

	if( objContainer )
	{
		objContainer->GetData()->m_isDestroyWhenFinished = true;
		objContainer->GetData()->GetSound()->SetLoop( false );
		objContainer->GetData()->GetSound()->Play();
		objContainer->GetData()->GetSound()->SetVolume( volume );
		objContainer->GetData()->GetSound()->SetRelativeToListener(true);
	}
}

void SoundMgr::SetGlobalPitch( float pitch)
{
	vObject_t::iterator iter = m_vSounds.begin();
	vObject_t::iterator iter_end = m_vSounds.end();
	while( iter != iter_end )
	{
		sf::Sound* sound = (*iter)->GetData()->GetSound();
		sound->SetPitch( pitch );
		++iter;
	}
}