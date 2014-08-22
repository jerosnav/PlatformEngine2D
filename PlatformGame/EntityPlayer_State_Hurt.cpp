#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Hurt(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity.X = (entityNode_->GetIntParam("DamageDir") == eDirFlag::RIGHT )? 40 : -40;
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Hurt", 3);
		
		entityNode_->GetEntityCollisionBody("Shield")->IsEnabled = false;
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity.X = 0;

		entityNode_->GetEntityCollisionBody("Shield")->IsEnabled = true;
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		// This divide velX by 4 each second
		phyObj->Velocity.X /= pow( 40., entityNode_->GetGameManager()->GetFrameDt() / 1000.);

		if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
		{
			if( entityNode_->GetIntParam("HP") <= 0 )
			{
				entityNode_->GetStateMachine()->SwitchState("Dying");
			}
			else
			{
				entityNode_->GetStateMachine()->SwitchState("Walking");
			}
		}
		
	}
}