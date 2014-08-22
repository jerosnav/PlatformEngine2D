#include "EntityPlayer.h"

static void _UpdateCollisions( EntityNode* entityNode_ )
{
	//Update sword coll body
	entityNode_->GetEntityCollisionBody("Sword")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "Sword_Side" ) );
}

//---------------------------------------------------------------------------------------------------------
// State_Attacking
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Attacking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{

		// Play sound
		entityNode_->GetGameManager()->GetSoundMgr()->PlaySound("sounds/SwordSlash00.wav");		

		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Stand_Attack", 1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();

		if( phyObj->IsOnPlatform )
		{
			phyObj->Velocity.X = 0;
		}

		entityNode_->GetEntityCollisionBody("Sword")->IsEnabled = true;
		entityNode_->GetEntityCollisionBody("Shield")->IsEnabled = false;
		
		//Update sword coll body> has to be called to avoid on first frame with previous configuration
		_UpdateCollisions( entityNode_ );

	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		entityNode_->GetEntityCollisionBody("Sword")->IsEnabled = false;
		entityNode_->GetEntityCollisionBody("Shield")->IsEnabled = true;
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{

		_UpdateCollisions( entityNode_ );

		// only allow sword collision first frame
		//entityNode_->GetEntityCollisionBody("Sword")->IsEnabled = entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsFrameHasChanged();

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		if( phyObj->IsOnPlatform )
		{
			phyObj->Velocity.X = 0;
		}

		if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
		{
			if( phyObj->IsOnPlatform )
			{
				entityNode_->GetStateMachine()->SwitchState("Idle");
			}
			else
			{
				entityNode_->GetStateMachine()->SwitchState("Falling");
			}
		}
	}
}