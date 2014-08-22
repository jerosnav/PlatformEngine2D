#pragma once

#include <irrlicht.h>

#include"ICamera.h"

#include "InputKeyboard.h"

using namespace irr;

class DebugCamera : public ICamera
{
public:	
	DebugCamera( scene::ISceneManager* smgr, f32 width, f32 height ) : ICamera( smgr, width, height )
	{}

	virtual void Update( u32 gameTime )
	{

		// Call base
		ICamera::Update( gameTime );

		// This is the movemen speed in units per second.
		const f32 MOVEMENT_SPEED = 1000.f;

		const f32 frameDeltaTime = (f32)(gameTime) / 1000.f; // Time in seconds

		core::vector2df camPos = GetPosition();

		/*
		if(InputKeyboard::IsKeyDown(irr::KEY_KEY_W))
		{
			camPos.Y -= MOVEMENT_SPEED * frameDeltaTime;
		}
		else if(InputKeyboard::IsKeyDown(irr::KEY_KEY_S))
		{
			camPos.Y += MOVEMENT_SPEED * frameDeltaTime;
		}

		if(InputKeyboard::IsKeyDown(irr::KEY_KEY_A))
		{
			camPos.X -= MOVEMENT_SPEED * frameDeltaTime;
		}
		else if(InputKeyboard::IsKeyDown(irr::KEY_KEY_D))
		{
			camPos.X += MOVEMENT_SPEED * frameDeltaTime;
		}
		*/

		SetPosition( camPos );

		core::vector3df vZoom = m_cameraNode->getScale();
		f32 scale_off = 0.01;
		if(InputKeyboard::IsKeyDown(irr::KEY_KEY_Q))
		{
			//nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
			vZoom.X+=scale_off;
			vZoom.Y+=scale_off;
			//vZoom.Z+=scale_off; // fix bug of sprites dissapearing when in the bottom of screen
		}
		else if(InputKeyboard::IsKeyDown(irr::KEY_KEY_E))
		{
			//nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
			vZoom.X-=scale_off;
			vZoom.Y-=scale_off;
			//vZoom.Z-=scale_off;
			vZoom.X = std::max<f32>( vZoom.X, scale_off);
			vZoom.Y = std::max<f32>( vZoom.X, scale_off);
			//vZoom.Z = std::max<f32>( vZoom.X, scale_off);
		}
		
		if( InputKeyboard::IsKeyPressed( irr::KEY_KEY_1 ) )
		{
			vZoom = core::vector3df( 1, 1, 1 );
		}
		if( InputKeyboard::IsKeyPressed( irr::KEY_KEY_2 ) )
		{
			vZoom = core::vector3df( 2, 2, 1 );
		}
		if( InputKeyboard::IsKeyPressed( irr::KEY_KEY_3 ) )
		{
			vZoom = core::vector3df( 3, 3, 1 );
		}


		m_cameraNode->setScale( vZoom );
	}
};