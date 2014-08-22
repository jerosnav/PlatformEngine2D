#include "EntityRock.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityRock::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Idle", -1); // set when build
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

		entityNode_->GetSpriteComponent()->GetAnimPlayer()->TintColor = video::SColor(255, 220 + rand()%35, 80 + rand()%80, 0);

		s32 frameDt = entityNode_->GetGameManager()->GetFrameDt();
		f32 rotation = entityNode_->getRotation().Z;
		rotation += (( 1.79 * phyObj->Velocity.X * frameDt) / 1000);
		if( rotation > 360 )
		{
			rotation -= 360;
		}
		else if( rotation < -360 )
		{
			rotation += 360;
		}
		entityNode_->setRotation( core::vector3df(0, 0, rotation) );

		if( phyObj->IsOnPlatform )
		{
			//phyObj->Velocity.X *= 1 + (2*( phyObj->Position.Y - phyObj->PrevPosition.Y ))/100;
			//core::vector2df acc = core::vector2df( 400 * ( phyObj->Position.Y - phyObj->PrevPosition.Y ), 0 );
			int iSlopeDir = (int)( phyObj->PrevPosition.Y - phyObj->Position.Y );
			
			core::vector2df acc = core::vector2df( 50, 0 );
			if( iSlopeDir == 0 )
			{
				acc.X = 0;
			}
			else if( 
				( iSlopeDir > 0 ) && ( phyObj->Velocity.X >= 0 ) ||
				( iSlopeDir < 0 ) && ( phyObj->Velocity.X <= 0 )
			)
			{
				acc.X = -acc.X;
			}

			f32 threshold = 4;
			if( ( phyObj->Velocity.X >= 0) && ( phyObj->Velocity.X < (threshold/2)  ) )
			{
				phyObj->Velocity.X = -threshold;
			}
			else if( ( phyObj->Velocity.X <= 0) && ( phyObj->Velocity.X > -(threshold/2) ) )
			{
				phyObj->Velocity.X = threshold;
			}

			phyObj->ApplyAcceleration( acc );
		}

		entityNode_->SetIntParam( "lastPosY", phyObj->Position.Y );

		if( phyObj->Velocity.X > 0 )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_NONE;
		}
		else if( phyObj->Velocity.X < 0 )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = eSpriteEffects::SPRITE_EFFECT_FLIP_X;
		}

		if( phyObj->PhyCollisionFlags & ( eDirFlag::RIGHT | eDirFlag::LEFT ) )
		{
			f32 lastVx = entityNode_->GetFloatParam("lastVelocX");
			entityNode_->SetIntParamOffset("HP", -10);
			phyObj->Velocity.Y -= lastVx;
			phyObj->Velocity.X = -0.7*lastVx;

			if( entityNode_->GetIntParam("HP") <= 0 )
			{
				EntityRock::DoDestroyRock( entityNode_ );
			}
		}
		entityNode_->SetFloatParam("lastVelocX", phyObj->Velocity.X);

		// destroy if outside camera boundingbox
		if( false == entityNode_->GetGameManager()->GetCamera()->GetBoundingBox().isPointInside( phyObj->Position )  )
		{
			entityNode_->DestroyEntity();
		}
	}
}