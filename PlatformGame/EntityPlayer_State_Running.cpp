#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Running(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Running", -1);
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
			phyObj->Velocity.X = entityNode_->GetFloatParam("movingVeloc") * 2.5f;
		}
		else if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT ) )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
			phyObj->Velocity.X = -entityNode_->GetFloatParam("movingVeloc") * 2.5f;
		}
		else
		{
			entityNode_->GetStateMachine()->SwitchState("Idle");
		}

		if( false == InputKeyboard::IsKeyDown( EKEY_CODE::KEY_KEY_A ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Walking");
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
	}
}