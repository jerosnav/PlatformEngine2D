#include "GameManager.h"


//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void GameManager::State_LoadingRoom(s32 messageType_, GameManager* gameMgr_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		;
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		;
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{		
		gameMgr_->LoadRoom( gameMgr_->m_loadRoom_roomToBeLoaded.c_str() );

		// Move player to door		
		if( gameMgr_->m_loadRoom_targetDoor.length() > 0 )
		{
			EntityNode* doorNode = gameMgr_->GetEntityManager()->GetEntityByTag(gameMgr_->m_loadRoom_targetDoor);
			EntityNode* playerNode = gameMgr_->GetEntityManager()->GetEntityByTag("player");		
			playerNode->GetPhysicObject()->Position = doorNode->GetPhysicObject()->Position;
		}

		//Reset data
		gameMgr_->m_loadRoom_roomToBeLoaded = "";
		gameMgr_->m_loadRoom_targetDoor = "";

		gameMgr_->GetGameStateMachine()->SwitchState("Gameplay");
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