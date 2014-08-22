#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State_Idle
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Crouch(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Crouch", -1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->MaxVelocity = core::vector2df(-1, -1);

		// set new physic body
		entityNode_->GetPhysicObject()->BoundingBox = entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody_Crouch");

		entityNode_->SetIntParam("Timer", 800);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		//Restore CollBody
		entityNode_->GetEntityCollisionBody("CollBody")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "CollBody", eSpriteEffects::SPRITE_EFFECT_NONE ) );
		entityNode_->GetPhysicObject()->BoundingBox = entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");

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
		entityNode_->GetEntityCollisionBody("Shield")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "Shield_Crouch" ) );
		//Set CollBody to crouch
		entityNode_->GetEntityCollisionBody("CollBody")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "CollBody_Crouch", eSpriteEffects::SPRITE_EFFECT_NONE ) );

		// After a while, lookDown
		if( entityNode_->GetIntParam("Timer") > 0 )
		{
			entityNode_->SetIntParamOffset("Timer", -entityNode_->GetGameManager()->GetFrameDt() );
		}
		else
		{
			// Move camera up
			int camDist = 75;
			int camYf = entityNode_->GetIntParam("CameraOffY");
			if( camYf < camDist )
			{
				camYf += (camDist - camYf) / 8;
				entityNode_->SetIntParam("CameraOffY", camYf);
			}
			else
			{
				camYf = camDist;
			}
		}

		PhyObject* phyObj = entityNode_->GetPhysicObject();

		// This divide velX by 4 each second
		phyObj->Velocity.X /= pow( 40000., entityNode_->GetGameManager()->GetFrameDt() / 1000.);

		if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_RIGHT ) )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_NONE;
		}
		else if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT ) )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
		}

		if( false == phyObj->IsOnPlatform )
		{
			entityNode_->GetStateMachine()->SwitchState("Falling");
		}

		if( false == InputKeyboard::IsKeyDown( EKEY_CODE::KEY_DOWN ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Idle");
		}

		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_SPACE ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Attacking_Crouch");
		}

		if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_KEY_Z ) )
		{
			phyObj->SkipFloatingPlatformCheck = true;
		}
	}
}