#include "EntitySlime.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntitySlime::State_Hurt(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();

		entityNode_->SetIntParam("Timer", 300);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->TintColor = irr::video::SColor(0xff, 0xff, 0xff, 0xff);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{
		entityNode_->SetIntParamOffset("Timer", -entityNode_->GetGameManager()->GetFrameDt());

		if( entityNode_->GetIntParam("Timer") <= 0 )
		{
			entityNode_->GetStateMachine()->SwitchState("Idle");
		}
		else
		{
			if( entityNode_->GetIntParam("Timer") & 0x20 )
			{
				entityNode_->GetSpriteComponent()->GetAnimPlayer()->TintColor = irr::video::SColor(0x50, 0x50, 0x50, 0x50);
			}
			else
			{
				entityNode_->GetSpriteComponent()->GetAnimPlayer()->TintColor = irr::video::SColor(0xff, 0xff, 0xff, 0xff);
			}
		}

		if( entityNode_->GetIntParam("HP") <= 0 )
		{
			entityNode_->GetStateMachine()->SwitchState("Dying");
		}
	}
}