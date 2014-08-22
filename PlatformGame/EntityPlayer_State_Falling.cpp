#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Falling(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Jumping", 1);

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		phyObj->MaxVelocity = core::vector2df( entityNode_->GetFloatParam("movingVeloc"), -1);

		phyObj->Velocity.X *= 0.8;

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

		if( phyObj->IsOnPlatform )
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

		// Spawn in pawn position if outside screen
		if( 
			(phyObj->Position.Y > 0) && // avoid spawn if player is outside screen but above
			( false == entityNode_->GetGameManager()->GetCamera()->GetBoundingBox().isPointInside( phyObj->Position )  )
		)
		{
			// Set spawn point
			phyObj->Position.X = entityNode_->GetIntParam("SpawnX");
			phyObj->Position.Y = entityNode_->GetIntParam("SpawnY");
		}
	}
}