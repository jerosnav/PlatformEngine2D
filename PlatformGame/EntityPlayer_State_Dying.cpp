#include "EntityPlayer.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
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
			//entityNode_->DestroyEntity();
		}
		
	}
}