#include "GameManager.h"

#include <SFML/Audio.hpp>
#include <iomanip>
#include <iostream>

static EntityNode *entityTitleScreen;

static int s_mainMusicId;

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void GameManager::State_TitleScreen(s32 messageType_, GameManager* gameMgr_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{

		// Play Music
		s_mainMusicId = gameMgr_->GetSoundMgr()->CreateSound("music/Title.ogg");
		sf::Sound* sndMusic = gameMgr_->GetSoundMgr()->GetSound( s_mainMusicId );
		sndMusic->SetLoop(true);
		sndMusic->Play();
		sndMusic->SetRelativeToListener(true);
		//sndMusic->SetPosition( 5, 5, 0);
		//sndMusic->SetVolume(100);


		//Show Title Screen
		entityTitleScreen = new EntityNode( gameMgr_->GetSceneManager()->getRootSceneNode(), gameMgr_->GetSceneManager(), 0, gameMgr_);
		entityTitleScreen->SetSpriteComponent("sprites\\titleScreen.sprite");
		entityTitleScreen->GetSpriteComponent()->GetAnimPlayer()->SetAnimation(0, -1);
		entityTitleScreen->drop();

		//entityTitleScreen->setPosition( core::vector3df( -driver->getScreenSize().Width/2, -driver->getScreenSize().Height/2, 0 ) );
		//entityTitleScreen->setPosition( core::vector3df( 50, -50, 0 ) );
		//GetCamera()->SetPosition( core::vector2df( driver->getScreenSize().Width/2, driver->getScreenSize().Height/2 ) );
		gameMgr_->GetCamera()->Reset();
		gameMgr_->GetCamera()->SetPosition( core::vector2df( 256/2, 192/2 ) );
		gameMgr_->GetCamera()->SetScale(4);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		gameMgr_->GetSoundMgr()->DestroySound( s_mainMusicId );
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{		
		if( InputKeyboard::IsKeyPressed( KEY_SPACE ) || InputKeyboard::IsKeyPressed( KEY_RETURN ) )
		{			
			entityTitleScreen->getSceneManager()->addToDeletionQueue( entityTitleScreen );

			gameMgr_->m_loadRoom_roomToBeLoaded = ".\\levels\\tutorial.level";
			gameMgr_->GetGameStateMachine()->SwitchState("LoadingRoom");
		}
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_PAINT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_PAINT )
	{
		gameMgr_->GetSceneManager()->drawAll();
	}
}