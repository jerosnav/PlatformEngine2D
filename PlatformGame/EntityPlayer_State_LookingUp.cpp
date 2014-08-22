#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State_Idle
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_LookingUp(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("LookingUp", -1);
		entityNode_->SetIntParam("Timer", 800);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		// reset camera offset
		entityNode_->SetIntParam("CameraOffY", 0);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{

		//Update shield coll body
		entityNode_->GetEntityCollisionBody("Shield")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "Shield_Side" ) );

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		// This divide velX by 4 each second
		phyObj->Velocity.X /= pow( 40000., entityNode_->GetGameManager()->GetFrameDt() / 1000.);

		// After a while, lookUp
		if( entityNode_->GetIntParam("Timer") > 0 )
		{
			entityNode_->SetIntParamOffset("Timer", -entityNode_->GetGameManager()->GetFrameDt() );
		}
		else
		{
			// Move camera up
			int camDist = 75;
			int camYf = -entityNode_->GetIntParam("CameraOffY");
			if( camYf < camDist )
			{
				camYf += (camDist - camYf) / 8;
				entityNode_->SetIntParam("CameraOffY", -camYf);
			}
			else
			{
				camYf = camDist;
			}
		}

		if( false == InputKeyboard::IsKeyDown( EKEY_CODE::KEY_UP ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Idle");
		}

		if( false == phyObj->IsOnPlatform )
		{
			entityNode_->GetStateMachine()->SwitchState("Falling");
		}

	}
}