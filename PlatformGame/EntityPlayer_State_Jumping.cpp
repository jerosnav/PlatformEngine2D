#include "EntityPlayer.h"
#include "MathUtil.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Jumping(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		DBG(" Enter Jumping ");
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Jumping", 1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();

		phyObj->MaxVelocity = core::vector2df( entityNode_->GetFloatParam("movingVeloc"), -1);
		
		phyObj->Velocity.X *= 0.8;
		phyObj->Velocity.Y = -entityNode_->GetFloatParam("jumpVeloc");		

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

		//Update shield coll body
		entityNode_->GetEntityCollisionBody("Shield")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "Shield_Side" ) );

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_RIGHT ) )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_NONE;
			//phyObj->Velocity.X = entityNode_->GetFloatParam("movingVeloc");
			phyObj->ApplyAcceleration( core::vector2df(entityNode_->GetFloatParam("jumpMoveAcc"), 0));
		}
		else if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT ) )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
			//phyObj->Velocity.X = -entityNode_->GetFloatParam("movingVeloc");
			phyObj->ApplyAcceleration( core::vector2df(-entityNode_->GetFloatParam("jumpMoveAcc"), 0));
		}

		if( false == InputKeyboard::IsKeyDown( EKEY_CODE::KEY_KEY_Z ) )
		{
			// This divide vely by 4 each second
			phyObj->Velocity.Y/= pow( 40., entityNode_->GetGameManager()->GetFrameDt() / 1000.);
		}

		if( phyObj->Velocity.Y > 0 )
		{
			entityNode_->GetStateMachine()->SwitchState("Falling");
		}
		else if( phyObj->Velocity.Y == 0 && phyObj->IsOnPlatform ) // fix weird bug when Vy = 0 and is on platform
		{
			entityNode_->GetStateMachine()->SwitchState("Idle");
		}

		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_SPACE ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Attacking");
		}

		//+++Check jump against wall
		DoJumpAgainstWallCheck(entityNode_);
		//---
	}
}