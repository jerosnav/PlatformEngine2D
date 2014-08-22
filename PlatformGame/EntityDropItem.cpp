	
#include "EntityDropItem.h"

#include "EntityCollisionFlags.h"
#include "EntityPlayer.h"

void EntityDropItem::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity.Y = -200;

		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( entityNode_->GetStringParam("AnimUp"), -1);
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
		if(
			( phyObj->Velocity.Y < 0 ) &&
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation()->Tag.compare( entityNode_->GetStringParam("AnimDown") )
		)
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( entityNode_->GetStringParam("AnimDown"), -1);
		}
		else if(
			( phyObj->IsOnPlatform ) &&
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation()->Tag.compare( entityNode_->GetStringParam("AnimFloor") )
		)
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( entityNode_->GetStringParam("AnimFloor"), -1);
		}
	}
}

void EntityDropItem::State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		;// do nothing
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
		entityNode_->DestroyEntity();

		// Set params
		EntityNode* player = entityNode_->GetGameManager()->GetEntityManager()->GetEntityByTag("player");
		int itemType = entityNode_->GetIntParam("ItemType");
		int itemSubtype = entityNode_->GetIntParam("ItemSubtype");
		switch( itemType )
		{
			case ITEM_TYPE_WEAPON:			
				EntityPlayer::ChangeEquipment( player, (eWeapon)itemSubtype );
				break;
			case ITEM_TYPE_SHIELD:
				EntityPlayer::ChangeEquipment( player, (eShield)itemSubtype );
				break;

			case ITEM_TYPE_MONEY:
				
				// Play sound
				entityNode_->GetGameManager()->GetSoundMgr()->PlaySound("sounds/Coin.ogg");

				break;
		}
	}
}

void EntityDropItem::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	if(
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY ) &&
		( otherCollBody->Owner == otherCollBody->Owner->GetGameManager()->GetEntityManager()->GetEntityByTag("player") ) && // collides with player
		( thisCollBody->Owner->GetPhysicObject()->Velocity.Y >= 0 )
	)
	{
		thisCollBody->Owner->GetStateMachine()->SwitchState("Dying");
	}
}