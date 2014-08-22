#include "EntityBlock.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityBlock::State_Hurt(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		int iAnimNb = entityNode_->GetSpriteComponent()->GetSprite()->Animations.size() - 1; //-1 due last anim is for dying
		int iAnimIdx = (iAnimNb-1) - ((iAnimNb-1) * entityNode_->GetIntParam("HP")) / entityNode_->GetIntParam("HP_Max");
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimation(iAnimIdx, -1);

		// Set Hurt timer( to avoid extra damage )
		entityNode_->SetIntParam("Timer", 250);
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

		//Update timer
		entityNode_->SetIntParamOffset("Timer", -entityNode_->GetGameManager()->GetFrameDt() );
		if(
			( entityNode_->GetIntParam("Timer") <= 0 ) ||
			( entityNode_->GetIntParam("HP") <= 0 )
		)
		{
			if( entityNode_->GetIntParam("HP") <= 0 )
			{
				entityNode_->GetStateMachine()->SwitchState("Dying");
			}
			else
			{
				entityNode_->GetStateMachine()->SwitchState("Idle");
			}
		}
	}
}