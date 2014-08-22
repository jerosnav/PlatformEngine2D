#pragma once


#include "PhyEngine.h"
#include "EventReceiverMgr.h"
#include "EntityNode.h"
#include "ICamera.h"
#include "Level.h"
#include "EntityManager.h"
#include "IEntityFactory.h"
#include "EntityCollisionMgr.h"
#include "IGameStateMachine.h"
#include "SoundMgr.h"

class Level;
class IEntityFactory;
class EntityManager;
class EntityCollisionMgr;
class IGameStateMachine;

class IGameManager
{
public:

	IGameManager() 
	: m_timeFactor( 1 )
	, m_phyEngine(0)
	, m_level(0)
	, m_soundMgr( new SoundMgr() )
	{}

	virtual s32 Initialize() = 0;

	virtual s32 Run() = 0;

	virtual s32 LoadRoom( const char* sRoomFile ) = 0;
	virtual void UnloadRoom( ) = 0;

	f32						GetTimeFactor() { return m_timeFactor; }
	s32						GetFrameDt() { return m_frameDt; }
	IrrlichtDevice*			GetDevice(){ return m_device; }
	video::IVideoDriver*	GetDriver(){ return m_device->getVideoDriver(); }
	Level*					GetLevel(){ return m_level; }

	//+++ Componentes
	PhyEngine*				GetPhyEngine()			{ return m_phyEngine; }
	scene::ISceneManager*	GetSceneManager()		{ return m_device->getSceneManager(); } // Irrlicht: Scene Graph
	gui::IGUIEnvironment*	GetGuiEnvironment()		{ return m_device->getGUIEnvironment(); }; // Irrlicht:
	EntityManager*			GetEntityManager()		{ return m_entityMgr; }
	IEntityFactory*			GetEntityFactory()		{ return m_entityFactory; }
	EntityCollisionMgr*		GetEntityCollisionMgr()	{ return m_entityCollMgr; }
	IGameStateMachine*		GetGameStateMachine()	{ return m_gameStateMachine; }
	SoundMgr*				GetSoundMgr()			{ return m_soundMgr; }
	//---

	ICamera*				GetCamera( ){ return m_camera; }
	void SetCamera( ICamera* camera ){ m_camera = camera; }
	
	void SetTimeFactor( f32 value ) { m_timeFactor = value; }
	void SetTimeFactorOffset( f32 value ) { m_timeFactor += value; }
	void SetLevel ( Level* level );
	void SetPhysicEngine( PhyEngine* phyEngine );

protected:
	
	bool m_isInitialized;

	s32 m_frameDt;
	f32 m_timeFactor;

	EventReceiverMgr m_receiver;
	
	PhyEngine* m_phyEngine;

	IrrlichtDevice* m_device;

	ICamera* m_camera;

	Level* m_level;
	EntityManager*		m_entityMgr;
	IEntityFactory*		m_entityFactory;
	EntityCollisionMgr* m_entityCollMgr;
	IGameStateMachine*	m_gameStateMachine;
	SoundMgr*			m_soundMgr;
};