	
#include "EntityCollectible.h"

#include "EntityCollisionFlags.h"

void EntityCollectible::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		std::string sAnim = entityNode_->GetStringParam("SetAnim");
		if( sAnim.length() > 0 )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( sAnim, -1);
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
	}
}

void EntityCollectible::State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{

		// Play sound
		entityNode_->GetGameManager()->GetSoundMgr()->PlaySound("sounds/Coin.ogg");		

		std::string sAnim = entityNode_->GetStringParam("SetAnimDying");
		if( sAnim.length() > 0 )
		{
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( sAnim, 5);
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
		phyObj->ApplyAcceleration( core::vector2df(0, -400) );

		if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
		{
			entityNode_->DestroyEntity();
		}
	}
}

void EntityCollectible::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	if(
		( thisCollBody->Owner->GetStateMachine()->GetCurrentState().compare("Dying") ) &&
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY ) &&
		( otherCollBody->Owner == otherCollBody->Owner->GetGameManager()->GetEntityManager()->GetEntityByTag("player") ) // collides with player
	)
	{
		DBG(" <EntityPlayer::Collision_Sword_Side> Collided Enemy ");
		thisCollBody->Owner->GetStateMachine()->SwitchState("Dying");
	}
}