#include "EntitySlime.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntitySlime::State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity = core::vector2df( 0, 0 );
		phyObj->IsApplyGravity = false;
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Dying", 1);

		// generate drop item
		EntityNode* dropItem = ((EntityFactory *)entityNode_->GetGameManager()->GetEntityFactory())->CreateDropItem(entityNode_, ITEM_TYPE_MONEY, 0);
		dropItem->GetPhysicObject()->Position = phyObj->Position;
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
			entityNode_->DestroyEntity();
		}		
	}
}