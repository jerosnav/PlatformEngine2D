#include "EntityFireSnake.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityFireSnake::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Idle", -1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity.X = 0;
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

		if( player->GetPhysicObject()->Position.X < phyObj->Position.X )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
		}
		else
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_NONE;
		}
		
		if( 
			( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsFrameHasChanged() ) &&
			( entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation()->CurrentAFrameIdx() == 1 )
		)
		{
			EntityNode* fireBall = entityNode_->GetGameManager()->GetEntityFactory()->CreateEntityFromTemplate("fireBall", entityNode_->getParent()->getName(), "", 0);

			if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects & eSpriteEffects::SPRITE_EFFECT_FLIP_X )
			{
				fireBall->GetPhysicObject()->Position = phyObj->Position + core::vector2df( -20, -30 );
				fireBall->GetPhysicObject()->Velocity =  core::vector2df( -200, 0 );
			}
			else
			{
				fireBall->GetPhysicObject()->Position = phyObj->Position + core::vector2df( +20, -30 );
				fireBall->GetPhysicObject()->Velocity =  core::vector2df( +200, 0 );
			}
		}
	}
}