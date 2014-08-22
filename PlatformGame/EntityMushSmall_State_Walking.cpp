#include "EntityMushSmall.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityMushSmall::State_Walking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Walking", -1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();
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

		if( phyObj->Velocity.X == 0 )
		{
			if(
				!entityNode_->IsIntParamSet("StartX") || // set only first time
				( phyObj->PhyCollisionFlags != 0 ) // or when colliding with wall, to avoid weird behaviour if player push this entity too far from origin
			)
			{
				entityNode_->SetIntParam("StartX", entityNode_->GetPhysicObject()->Position.X);
			}
			int iWalkSizeX = rand() % 50; iWalkSizeX += abs(iWalkSizeX) + 50;
			entityNode_->SetIntParam("LeftX", entityNode_->GetIntParam("StartX") - iWalkSizeX);
			entityNode_->SetIntParam("RightX", entityNode_->GetIntParam("StartX") + iWalkSizeX);

			phyObj->Velocity.X = ( rand()%2 )? -30 : 30;
		}
		else
		{
			if(
				( phyObj->Velocity.X < 0 && phyObj->Position.X < entityNode_->GetIntParam("LeftX") ) ||
				( phyObj->Velocity.X > 0 && phyObj->Position.X > entityNode_->GetIntParam("RightX") )
			)
			{
				phyObj->Velocity.X = -phyObj->Velocity.X;
			}
			
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = phyObj->Velocity.X > 0 ? eSpriteEffects::SPRITE_EFFECT_NONE : eSpriteEffects::SPRITE_EFFECT_FLIP_X;
		}
	}
}