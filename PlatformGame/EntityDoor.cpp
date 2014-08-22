	
#include "EntityDoor.h"

#include "EntityCollisionFlags.h"
#include "EntityFactory.h"

void EntityDoor::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();

		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( "Closed", -1);
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
		PhyObject* phyObj = entityNode_->GetPhysicObject();
	}
}

void EntityDoor::State_Opening(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( "Opening", 1);
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
		if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
		{
			std::string targetRoom = entityNode_->GetStringParam("targetRoom");
			std::string targetDoor = entityNode_->GetStringParam("targetDoor");
			printf(" Going to room %s door %s", targetRoom.c_str(), targetDoor.c_str());
			static_cast<GameManager*>( entityNode_->GetGameManager() )->GoToRoom( targetRoom.c_str(), targetDoor.c_str() );
		}
	}
}

void EntityDoor::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	if(
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY ) &&
		( otherCollBody->Owner == otherCollBody->Owner->GetGameManager()->GetEntityManager()->GetEntityByTag("player") ) // collides with player
	)
	{
		if( 
			InputKeyboard::IsKeyPressed(KEY_UP) &&
			!thisCollBody->Owner->GetStateMachine()->GetCurrentState().compare("Idle")
		)
		{
			printf(" Door activated!!!");
			thisCollBody->Owner->GetStateMachine()->SwitchState("Opening");
		}
	}
}