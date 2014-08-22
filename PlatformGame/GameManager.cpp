#include "GameManager.h"

#include "EventReceiverMgr.h"
#include "ICamera.h"
#include "DebugCamera.h"
#include "Tilemap.h"
#include "PhyTileEngine.h"
#include "TileLayerNode.h"
#include "TextUtil.h"
#include "Level.h"
#include "EntityFactory.h"
#include "GuiManager.h"

GameManager::GameManager(void) 
	: IGameManager() 
{};

s32 GameManager::Initialize()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType= video::EDT_OPENGL;//driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device
	m_device = createDevice(driverType,
			core::dimension2d<u32>(1024, 768), 32, false, false, false, &m_receiver);
		
	if (m_device == 0)
		return 1; // could not create selected driver.

	// create engine and camera
	m_device->setWindowCaption(L"Tiled Platform Engine - Demo");

	// create gui manager
	m_guiManager = new GuiManager( this );

	// Create Game State Machine
	m_gameStateMachine = new IGameStateMachine( this );
	m_gameStateMachine->SetStateFunction("TitleScreen", (TStateFP)State_TitleScreen);
	m_gameStateMachine->SetStateFunction("LoadingRoom", (TStateFP)State_LoadingRoom);
	m_gameStateMachine->SetStateFunction("Gameplay", (TStateFP)State_GamePlay);

	m_isInitialized = true;

	return 0;
}

void _UpdateDebugEntity( IGameManager* gameMgr, EntityNode* debugEntity )
{
	PhyObject* phyObj = debugEntity->GetPhysicObject();
	if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_KEY_Z ) )
	{
		phyObj->Velocity.Y = -debugEntity->GetFloatParam("jumpVeloc");
	}

	if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_RIGHT ) )
	{
		debugEntity->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_NONE;
		phyObj->Velocity.X = debugEntity->GetFloatParam("movingVeloc");
	}
	else if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT ) )
	{
		debugEntity->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
		phyObj->Velocity.X = -debugEntity->GetFloatParam("movingVeloc");
	}
	else
	{
		phyObj->Velocity.X = 0;
	}

	// Set animations
	if( phyObj->Velocity.getLengthSQ() == 0 )
	{
		debugEntity->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Standing", -1);
	}
	else if( phyObj->Velocity.Y < 0 )
	{
		debugEntity->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Jumping", -1);
	}
	else if( phyObj->Velocity.Y > 0 )
	{
		debugEntity->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Falling", -1);
	}
	else if( phyObj->Velocity.X != 0 )
	{
		debugEntity->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Walking", -1);
	}

	// Target camera
	gameMgr->GetCamera()->MoveTo( phyObj->Position );
}

s32 GameManager::LoadRoom( const char *sRoomFile )
{
	//!!!pass them by parameter
	m_entityFactory = new EntityFactory(this);
	m_entityMgr = new EntityManager( this );
	m_entityCollMgr = new EntityCollisionMgr( eCollGroups::COLLISION_GROUP_SIZE );
	m_entityCollMgr->SetCollisionGroup( eCollGroups::COLLISION_GROUP_PLAYER, eCollGroups::COLLISION_GROUP_ENEMIES, true);
	m_entityCollMgr->SetCollisionGroup( eCollGroups::COLLISION_GROUP_PLAYER, eCollGroups::COLLISION_GROUP_BLOCKS, true);
	//---
	m_level = Level::LoadFromFile( this, sRoomFile);//!!!debugRRR

	ASSERT( m_level, "Could not load file " << sRoomFile );

	Tilemap* tilemap = m_level->GetTilemap();

	// Set bounding box to map area
	GetCamera()->SetBoundingBox( core::rectf(core::vector2df(0, 0), core::dimension2df( tilemap->GetTileWidth() * tilemap->GetMapWidth(), tilemap->GetTileHeight() * tilemap->GetMapHeight()) ) );

	// create player
	EntityNode* playerNode = GetEntityManager()->GetEntityByTag("player");

	//!!!TODO: remove this parameter set and load player data from file
	if( !playerNode )
	{
		playerNode = GetEntityFactory()->CreateEntity( "Objects", "player" );
		GetEntityFactory()->BuildEntityFromTemplate( playerNode, "player", 0 );
	}
//---

	return 0;
}

void GameManager::UnloadRoom()
{
	if( m_level )
	{
		GetSceneManager()->clear();

		// Set camera again due GetSceneManager()->clear() remove the camera
		DebugCamera* camera = new DebugCamera( GetSceneManager(), GetDriver()->getScreenSize().Width, GetDriver()->getScreenSize().Height);
		SetCamera( camera );

		delete m_entityMgr; m_entityMgr = 0;
		delete m_level; m_level = 0;		
		delete m_entityCollMgr; m_entityCollMgr = 0;
		delete m_entityFactory; m_entityFactory = 0;
	}
}

void GameManager::GoToRoom( const char* roomFile, const char* doorTarget )
{
	m_loadRoom_roomToBeLoaded = roomFile;
	m_loadRoom_targetDoor = doorTarget;
	GetGameStateMachine()->SwitchState("LoadingRoom");
}

s32 GameManager::Run()
{

	// Create and configure driver
	video::IVideoDriver* driver = m_device->getVideoDriver();
	driver->setTextureCreationFlag(video::E_TEXTURE_CREATION_FLAG::ETCF_CREATE_MIP_MAPS,false);

//--- Camera
	DebugCamera* camera = new DebugCamera( GetSceneManager(), driver->getScreenSize().Width, driver->getScreenSize().Height);
	SetCamera( camera );
//---

//--- Start State
	GetGameStateMachine()->SwitchState("TitleScreen");
//---

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = m_device->getTimer()->getTime();

	/*
	Now draw everything and finish.
	*/
	u32 frames=0;
	while(m_device->run())
	{

		// Work out a frame delta time.
		const u32 now = m_device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		const u32 fps = 17; //!!!TODO> find this hardcoded value and get it by calling a method or by define
		u32 frameDT = (now - then); // Time in ms

//+++ FPS Control
		//if( frameDT > 1000 ){ frameDT = 1000; then = now - 1000; }
		if( frameDT < fps )
		{
			continue;
		}
		
		//then = now - (frameDT - fps); //without frame skip
		then = now - (frameDT % fps); //with frame skip
//---


		// apply time factor
		int frameNb = (frameDT/fps);
		frameDT = core::round_( fps * frameNb * m_timeFactor ); // remove frameNb to avoid update with a high frameDt TODO: update some systems

		// Set frameDt
		m_frameDt = frameDT;

		// Update sound manager
		GetSoundMgr()->Update();
		GetSoundMgr()->SetGlobalPitch( 1 * GetTimeFactor() );

		// Update Game State Machine
		GetGameStateMachine()->Update();

		// Update Inputs
		InputKeyboard::Update();

		// Update camera
		GetCamera()->Update( frameDT );

		// Send Update event
		GetGameStateMachine()->SendMsg( eCommonMessageTypes::STATE_MESSAGE_UPDATE );

	
		driver->beginScene(true, true, video::SColor(0,0,0,0));

		
		// Send Update event
		GetGameStateMachine()->SendMsg( eCommonMessageTypes::STATE_MESSAGE_PAINT );

//+++ Show memory leaks
		//_CrtDumpMemoryLeaks();
//---

		driver->endScene();
		if (++frames==100)
		{
			core::vector3df cameraPosition = GetSceneManager()->getActiveCamera()->getPosition();
			core::vector3df cameraScale = GetSceneManager()->getActiveCamera()->getScale();

			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			str += L" CAM: ";
			str += cameraPosition.X;
			str += L"; ";
			str += cameraPosition.Y;
			str += L"; ";
			str += cameraPosition.Z;
			str += L" SCALE: ";
			str += cameraScale.X;
			str += L"; ";
			str += cameraScale.Y;
			str += L"; ";
			str += cameraScale.Z;

			m_device->setWindowCaption(str.c_str());
			frames=0;
		}
	}

	m_device->drop();
	m_device = 0;
	
	return 0;
}
