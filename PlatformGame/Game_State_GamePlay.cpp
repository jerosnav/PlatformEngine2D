#include "GameManager.h"
#include "GuiManager.h"

#include "TextUtil.h"

static gui::IGUIFont* _debug_font;

static int s_mainMusicId;

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void GameManager::State_GamePlay(s32 messageType_, GameManager* gameMgr_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
	//!!!debugRRR
		_debug_font = gameMgr_->GetDevice()->getGUIEnvironment()->getBuiltInFont();
		gameMgr_->GetCamera()->SetPosition(core::vector2df(250, 192));
		gameMgr_->GetCamera()->SetScale( 3.f);
		gameMgr_->GetLevel()->GetTilemap()->GetTileLayer("Physic")->AddProperty("Visible", "false");
	//---

		// Play Music
		s_mainMusicId = gameMgr_->GetSoundMgr()->CreateSound("music/Town.ogg");
		sf::Sound* sndMusic = gameMgr_->GetSoundMgr()->GetSound( s_mainMusicId );
		sndMusic->SetLoop(true);
		sndMusic->Play();
		sndMusic->SetRelativeToListener(true);
		sndMusic->SetVolume( 50 );
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		gameMgr_->GetSoundMgr()->DestroySound( s_mainMusicId );
		gameMgr_->UnloadRoom();
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{

		u32 frameDT = gameMgr_->GetFrameDt();

		// Update Gui
		((GameManager*)gameMgr_)->GetGuiManager()->Update();

		// Update Level
		gameMgr_->GetLevel()->Update( frameDT );

		// Update Physics
		static u32 updateTime = 0;
		updateTime += frameDT;
		while( updateTime >= 17 )
		{
			gameMgr_->GetPhyEngine()->Update( 17 );
			updateTime -= 17;
		}

		// Keep Player inside screen
		EntityNode* playerNode = gameMgr_->GetEntityManager()->GetEntityByTag("player");
		int iBorderWidth = 10;
		if( playerNode->GetPhysicObject()->Position.X < iBorderWidth )
		{
			playerNode->GetPhysicObject()->Velocity.X = 0;
			playerNode->GetPhysicObject()->Position.X = iBorderWidth;
		}
		else if( playerNode->GetPhysicObject()->Position.X > (gameMgr_->GetCamera()->GetBoundingBox().getWidth() - iBorderWidth) )
		{
			playerNode->GetPhysicObject()->Velocity.X = 0;
			playerNode->GetPhysicObject()->Position.X = (gameMgr_->GetCamera()->GetBoundingBox().getWidth() - iBorderWidth);
		}

		if( InputKeyboard::IsKeyPressed( KEY_ESCAPE ) )
		{
			gameMgr_->GetGameStateMachine()->SwitchState("TitleScreen");
		}
	} 
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_PAINT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_PAINT )
	{

		// Get Player Node
		// !!!TODO> improve by taking this node by reference
		EntityNode* playerNode = gameMgr_->GetEntityManager()->GetEntityByTag("player");

		// Draw Entities but also: UpdateStateMachine
		gameMgr_->GetSceneManager()->drawAll();

		// Draw Shadows
		u32 screenWidth = gameMgr_->GetDriver()->getScreenSize().Width;
		u32 screenHeight = gameMgr_->GetDriver()->getScreenSize().Height;
		f32 PlayerX = playerNode->GetPhysicObject()->Position.X;
		f32 PlayerY = playerNode->GetPhysicObject()->Position.Y;
		f32 PlayerScrX = (screenWidth/2) + PlayerX - gameMgr_->GetCamera()->GetPosition().X;
		f32 PlayerScrY = (screenHeight/2) + PlayerY - gameMgr_->GetCamera()->GetPosition().Y;
		f32 shadowFactor = ( (PlayerY-800) / 512 );
		shadowFactor = core::clamp(shadowFactor, 0.f, 1.f);
/*
		gameMgr_->GetDriver()->draw2DRectangle( 
			video::SColor( static_cast<u32>(shadowFactor * 255), 0, 0, 0), 
			core::recti(0, 0, screenWidth, screenHeight),
			0//&core::recti(PlayerScrX - 64, PlayerScrY - 64, PlayerScrX + 64, PlayerScrY + 64 )
		);
*/
		// Update Entity Collisions
		gameMgr_->GetEntityCollisionMgr()->Update();

		// Rener GUI
		((GameManager*)gameMgr_)->GetGuiManager()->Render();

		// Draw center
		gameMgr_->GetDriver()->draw2DRectangleOutline(core::recti( core::vector2di((screenWidth/2)-1, (screenHeight/2)-1), core::dimension2di(2, 2)), video::SColor(255, 255, 255, 255)); 

		//!!!debugRRR
		static bool phyLayerOn = false;
		
		if( InputKeyboard::IsKeyPressed( irr::KEY_F1 ) )
		{
			phyLayerOn = !phyLayerOn;			
			gameMgr_->GetLevel()->GetTilemap()->GetTileLayer("Physic")->AddProperty("Visible", phyLayerOn?"true":"false");
		}

		if( InputKeyboard::IsKeyPressed( irr::KEY_F2 ) )
		{
			playerNode->SetDebugPhyCollRect( !playerNode->IsDebugPhyCollRect() );
		}

		if( InputKeyboard::IsKeyPressed( irr::KEY_F5 ) )
		{
			if( gameMgr_->GetTimeFactor() > 0 ) gameMgr_->SetTimeFactorOffset( -0.1 );						
		}

		if( InputKeyboard::IsKeyPressed( irr::KEY_F6 ) )
		{
			gameMgr_->SetTimeFactorOffset( 0.1 );
		}					

		//_UpdateDebugEntity( this, playerNode );
		PhyObject* phyObj = playerNode->GetPhysicObject();

		// Target camera
		playerNode->GetGameManager()->GetCamera()->MoveTo( phyObj->Position + core::vector2df( playerNode->GetIntParam("CameraOffX"), playerNode->GetIntParam("CameraOffY") ) );

		std::string sDebug;

		// Engine
		sDebug =   
		"TimeFactor:   "+TextUtil::to_string<f32>(gameMgr_->GetTimeFactor(), std::dec)+"\n"
		"CollisionBodies:   "+TextUtil::to_string<f32>( gameMgr_->GetEntityCollisionMgr()->GetCollisionBodyCounter() , std::dec)+"\n";
		_debug_font->draw(sDebug.c_str(),
					core::rect<s32>(512,110,400,150),
					video::SColor(255,255,255,255));

		// Player
		sDebug =   
		"Position:   "+TextUtil::to_string<f32>(phyObj->Position.X, std::dec)+"; "+TextUtil::to_string<f32>(phyObj->Position.Y, std::dec)+"\n"
		"Velocity:   "+TextUtil::to_string<f32>(phyObj->Velocity.X, std::dec)+"; "+TextUtil::to_string<f32>(phyObj->Velocity.Y, std::dec)+"\n"
		"MaxVelocity:   "+TextUtil::to_string<f32>(phyObj->MaxVelocity.X, std::dec)+"; "+TextUtil::to_string<f32>(phyObj->MaxVelocity.Y, std::dec)+"\n"
		"OnPlatform: "+TextUtil::to_string<bool>(phyObj->IsOnPlatform, std::dec)+"\n"
		"PrevState:  "+playerNode->GetStateMachine()->GetPrevState()+"\n"
		"State:      "+playerNode->GetStateMachine()->GetCurrentState()+"\n"
		"Hp: "+TextUtil::to_string<s32>(playerNode->GetIntParam("HP"), std::dec)+" / "+TextUtil::to_string<s32>(playerNode->GetIntParam("HP_Max"), std::dec);
		_debug_font->draw(sDebug.c_str(),
					core::rect<s32>(230,110,400,150),
					video::SColor(255,255,255,255));

		//Help info
		sDebug =
			" Controls: \n"
			"    - Use arrows for moving around. \n"
			"    - Press Q for zoom In\n"
			"    - Press E for zoom Out\n"
			"    - Press 1 for zoom at 100%\n"
			"    - Press 2 for zoom at 200%\n"
			"    - Press 3 for zoom at 300%\n"
			"    - Press F1 to enable/disable physic layer drawing\n"
			"    - Press F2 to enable/disable player physic rectangle drawing\n"
			"    - Press F5/F6 to increase/decreate time factor\n";
		_debug_font->draw(sDebug.c_str(),
					core::rect<s32>(0,100,200,200),
					video::SColor(255,255,255,255));
		//---
	}
}