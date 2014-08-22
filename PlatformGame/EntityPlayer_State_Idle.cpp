#include "EntityPlayer.h"

//---------------------------------------------------------------------------------------------------------
// State_Idle
//---------------------------------------------------------------------------------------------------------
void EntityPlayer::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		DBG(" Enter Idle ");
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Idle", -1);
		PhyObject* phyObj = entityNode_->GetPhysicObject();

		if( 
			InputKeyboard::IsKeyDown( EKEY_CODE::KEY_RIGHT ) ||
			InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT )
		)
		{
			entityNode_->GetStateMachine()->SwitchState("Walking");
		}
		else
		{
			phyObj->MaxVelocity = core::vector2df(-1, -1);
			DBG(" I Set Max Velocity -1 " );
		}

		// Set spawn point
		entityNode_->SetIntParam("SpawnX", entityNode_->GetPhysicObject()->Position.X);
		entityNode_->SetIntParam("SpawnY", entityNode_->GetPhysicObject()->Position.Y);
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

		// This divide velX by 4 each second
		phyObj->Velocity.X /= pow( 40000., entityNode_->GetGameManager()->GetFrameDt() / 1000.);

		//!!!debugRRR
		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_KEY_N ) )
		{
			int iWeapon = entityNode_->GetIntParam("Weapon");
			int iShield = entityNode_->GetIntParam("Shield");
			iWeapon++; iWeapon %= WEAPON_SIZE;
			EntityPlayer::ChangeEquipment( entityNode_, (eWeapon)iWeapon, (eShield)iShield );
		}
		if( InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_KEY_M ) )
		{
			int iWeapon = entityNode_->GetIntParam("Weapon");
			int iShield = entityNode_->GetIntParam("Shield");
			iShield++; iShield %= SHIELD_SIZE;
			EntityPlayer::ChangeEquipment( entityNode_, (eWeapon)iWeapon, (eShield)iShield );
		}
		//---

		if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_UP ) )
		{
			entityNode_->GetStateMachine()->SwitchState("LookingUp");
		}

		if( 
			InputKeyboard::IsKeyDown( EKEY_CODE::KEY_RIGHT ) ||
			InputKeyboard::IsKeyDown( EKEY_CODE::KEY_LEFT )
		)
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

		if( InputKeyboard::IsKeyDown( EKEY_CODE::KEY_DOWN ) )
		{
			entityNode_->GetStateMachine()->SwitchState("Crouch");
		}
	}
}