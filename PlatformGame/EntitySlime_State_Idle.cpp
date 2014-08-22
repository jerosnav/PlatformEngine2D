#include "EntitySlime.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntitySlime::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		if( !entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation() )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("PreJumpingUp", 1);		
		}
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
		
		// destroy if outside camera boundingbox
		if( false == entityNode_->GetGameManager()->GetCamera()->GetBoundingBox().isPointInside( phyObj->Position )  )
		{
			entityNode_->DestroyEntity();
		}

		// Face player
		if( player->GetPhysicObject()->Position.X < phyObj->Position.X )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
		}
		else
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_NONE;
		}

		if( phyObj->IsOnPlatform ) 
		{
			if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation()->Tag.compare("PreJumpingUp") )
			{
				entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("PreJumpingUp", 1);
			}
			else if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
			{
				// jumping!
				phyObj->Velocity.Y = -entityNode_->GetIntParam("jumpVeloc");

				entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("JumpingUp", -1);
			}
		}
		else if(
			( phyObj->Velocity.Y > 0 ) &&
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation()->Tag.compare("FallingDown")
		)
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("FallingDown", 1);
		}
	}
}