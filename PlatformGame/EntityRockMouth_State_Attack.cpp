#include "EntityRockMouth.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityRockMouth::State_Attack(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("OpeningMouth", 1);
		entityNode_->SetIntParamOffset("attackNb", 10);
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

		if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
		{
			if( !entityNode_->GetSpriteComponent()->GetAnimPlayer()->CurrentAnimation()->Tag.compare("OpeningMouth") )
			{
				entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("ClosingMouth", 1);

				core::vector2di iShootPos = entityNode_->GetSpriteComponent()->GetSprite()->GetMkPointDataByTag("Shoot");
				core::vector2df shootPos = phyObj->Position + core::vector2df( iShootPos.X, iShootPos.Y );
				core::vector2df shootVeloc = core::vector2df( 75, -10 );

				EntityNode* rock = entityNode_->GetGameManager()->GetEntityFactory()->CreateEntityFromTemplate("rock", entityNode_->getParent()->getName(), "", 0);
				rock->GetPhysicObject()->Position = shootPos;
				rock->GetPhysicObject()->Velocity = shootVeloc;
				rock->SetIntParam("HP", 50);
			}
			else
			{
				entityNode_->SetIntParamOffset("attackNb", -1);

				if( entityNode_->GetIntParam("attackNb") > 0 )
				{
					entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("OpeningMouth", 1);
				}
				else
				{
					entityNode_->GetStateMachine()->SwitchState("Idle");
				}
			}
		}
	}
}