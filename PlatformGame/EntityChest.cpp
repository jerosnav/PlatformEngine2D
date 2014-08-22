	
#include "EntityChest.h"

#include "EntityCollisionFlags.h"
#include "EntityFactory.h"

void EntityChest::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
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

void EntityChest::State_Opening(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		// Play sound
		entityNode_->GetGameManager()->GetSoundMgr()->PlaySound("sounds/Chest.ogg");

		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( "Opening", 1);
		entityNode_->SetIntParam("itemDropped", 0);
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
		if( 
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() &&
			!entityNode_->GetIntParam("itemDropped")
		)
		{
			entityNode_->SetIntParam("itemDropped", 1);
			// generate drop item
			PhyObject* phyObj = entityNode_->GetPhysicObject();
			int itemType = entityNode_->GetIntParam("itemType");
			int itemSubtype = entityNode_->GetIntParam("itemSubtype");
			EntityNode* dropItem = ((EntityFactory *)entityNode_->GetGameManager()->GetEntityFactory())->CreateDropItem(entityNode_, (eItemType)itemType, itemSubtype);
			dropItem->GetPhysicObject()->Position = phyObj->Position + core::vector2df(0, -16);
		}
	}
}

void EntityChest::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
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
			thisCollBody->Owner->GetStateMachine()->SwitchState("Opening");
		}
	}
}