#pragma once

#include <irrlicht.h>
#include <string>

#include "Sprite.h"
#include "AnimationPlayer.h"

class SpriteComponent
{
private:

	irr::video::IVideoDriver *m_driver;

	Sprite*				m_sprite;
	AnimationPlayer*	m_animPlayer;	

public:

	core::vector2df		Offset;

	SpriteComponent( irr::video::IVideoDriver* driver) :
	  m_driver(driver),
		  m_sprite(0),
		  m_animPlayer(0),
		  Offset(0)

	{}

	~SpriteComponent()
	{
		if ( m_sprite ) delete m_sprite;
		if ( m_animPlayer ) delete m_animPlayer;
	}

	void SetSprite( const std::string& spritePath )
	{
		if( m_sprite ) delete m_sprite;

		m_sprite = Sprite::LoadFromFile( spritePath, m_driver);
		
		if( !m_animPlayer ) m_animPlayer = new AnimationPlayer( m_sprite );
		else m_animPlayer->SetSprite( m_sprite );
	}

	AnimationPlayer *GetAnimPlayer()
	{
		return m_animPlayer;
	}

	Sprite *GetSprite()
	{
		return m_sprite;
	}

	void Update( irr::u32 frameDT )
	{
		m_animPlayer->Update( frameDT );
	}

	void Draw( irr::core::vector2df position )
	{
		m_animPlayer->Draw( position + Offset );
	}
};