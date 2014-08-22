#include "EntityMushSmall.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityMushSmall::State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity.X = 0;
		phyObj->Velocity.Y = -100;
		phyObj->IsClipping = true;
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Hurt", -1);

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

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		// destroy if outside camera boundingbox
		if( false == entityNode_->GetGameManager()->GetCamera()->GetBoundingBox().isPointInside( phyObj->Position )  )
		{
			entityNode_->DestroyEntity();
		}
		
	}
}