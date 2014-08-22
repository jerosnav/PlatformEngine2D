#include "EntityPlayer.h"

static void _UpdateCollisions( EntityNode* entityNode_ )
{
		//Update sword coll body
		entityNode_->GetEntityCollisionBody("Sword")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "Sword_Crouch" ) );
		//Set CollBody to crouch
		entityNode_->GetEntityCollisionBody("CollBody")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "CollBody_Crouch", eSpriteEffects::SPRITE_EFFECT_NONE ) );
		//Change shield to crouch one
		entityNode_->GetEntityCollisionBody("Shield")->CollData = MathUtil::RectI2F( entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "Shield_Crouch", eSpriteEffects::SPRITE_EFFECT_NONE ) );

}

//---------------------------------------------------------------------------------------------------------
// State_Attacking
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Attacking_Crouch(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{

		// Play sound
		entityNode_->GetGameManager()->GetSoundMgr()->PlaySound("sounds/SwordSlash00.wav");	

		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Crouch_Attack", 1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();

		if( phyObj->IsOnPlatform )
		{
			phyObj->Velocity.X = 0;
		}

		entityNode_->GetEntityCollisionBody("Sword")->IsEnabled = true;
		entityNode_->GetEntityCollisionBody("Shield")->IsEnabled = false;

		// set new physic body
		entityNode_->GetPhysicObject()->BoundingBox = entityNode_->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody_Crouch");

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
				entityNode_->GetStateMachine()->SwitchState("Crouch");			
			}
		}
	}
}