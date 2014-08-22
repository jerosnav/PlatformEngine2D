#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Walking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Walking", -1);

		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->MaxVelocity = core::vector2df( entityNode_->GetFloatParam("movingVeloc"), -1);
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
			phyObj->ApplyAcceleration( core::vector2df( entityNode_->GetFloatParam("movingAcc"), 0) );
		}
		else if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT ) )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
			phyObj->ApplyAcceleration( core::vector2df( -entityNode_->GetFloatParam("movingAcc"), 0) );
		}
		else
		{
			entityNode_->GetStateMachine()->SwitchState("Idle");
		}
		
		if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_KEY_A ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Running");
		}

		if( false == phyObj->IsOnPlatform )
		{
			entityNode_->GetStateMachine()->SwitchState("Falling");
		}

		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_SPACE ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Attacking");
		}

		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_KEY_Z ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Jumping");
		}

		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_DOWN ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Crouch");
		}
	}
}