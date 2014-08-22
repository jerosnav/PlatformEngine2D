#include "EntityRockMouth.h"
#include "EntityUtils.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityRockMouth::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Idle", -1);
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
		EntityNode* player = entityNode_->GetGameManager()->GetEntityManager()->GetEntityByTag("player");
		PhyObject* phyPlayerObj = player->GetPhysicObject();

		if( 
			EntityUtils::IsEntityFacingOther( entityNode_, player ) &&
			( abs(phyObj->Position.X - phyPlayerObj->Position.X) <= 200 ) &&
			( abs(phyObj->Position.Y - phyPlayerObj->Position.Y) <= 50 )
		)
		{
			entityNode_->GetStateMachine()->SwitchState("Attack");
		}
	}
}