#pragma once

#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionMgr.h"
#include "MathUtil.h"
#include "EntityCollisionFlags.h"

using namespace irr;

class EntityPlayer
{
public:
	EntityPlayer(void);
	~EntityPlayer(void);


	// States
	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Hurt(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Attacking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Walking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Running(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Jumping(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Falling(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Crouch(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Attacking_Crouch(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_LookingUp(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	// Collision ev
	static void Collision_Sword_Side( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

	// Support Methods
	static void DoJumpAgainstWallCheck( EntityNode* entityNode_ );

	static void BuildEntity( EntityNode* entityNode, io::IrrXMLReader* xmlParams )
	{
		entityNode->SetSpriteComponent( entityNode->GetStringParam("Sprite") );
		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->Position = core::vector2df( 0, 0 );
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");

		// Setup entity collision
		Sprite* sprite = entityNode->GetSpriteComponent()->GetSprite();
		entityNode->AddEntityCollisionBody("CollBody", MathUtil::RectI2F( entityNode->GetPhysicObject()->BoundingBox ), eCollGroups::COLLISION_GROUP_PLAYER, ENTITY_COLLISION_FLAG_COLLBODY );
		entityNode->AddEntityCollisionBody("Shield", MathUtil::RectI2F( sprite->GetMkRectDataByTag( "Shield_Side" ) ), eCollGroups::COLLISION_GROUP_PLAYER, eEntityCollFlag::ENTITY_COLLISION_FLAG_SHIELD );		
		entityNode->AddEntityCollisionBody("Sword", MathUtil::RectI2F( sprite->GetMkRectDataByTag( "Sword_Side" ) ), eCollGroups::COLLISION_GROUP_PLAYER, eEntityCollFlag::ENTITY_COLLISION_FLAG_SHIELD, Collision_Sword_Side );	
		entityNode->GetEntityCollisionBody("Sword")->IsEnabled = false;

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
		stateMachine->SetStateFunction("Hurt", (TStateFP)State_Hurt);
		stateMachine->SetStateFunction("Dying", (TStateFP)State_Dying);
		stateMachine->SetStateFunction("Attacking", (TStateFP)State_Attacking);
		stateMachine->SetStateFunction("Walking", (TStateFP)State_Walking);
		stateMachine->SetStateFunction("Running", (TStateFP)State_Running);
		stateMachine->SetStateFunction("Jumping", (TStateFP)State_Jumping);
		stateMachine->SetStateFunction("Falling", (TStateFP)State_Falling);
		stateMachine->SetStateFunction("Crouch", (TStateFP)State_Crouch);
		stateMachine->SetStateFunction("Attacking_Crouch", (TStateFP)State_Attacking_Crouch);
		stateMachine->SetStateFunction("LookingUp", (TStateFP)State_LookingUp);

		stateMachine->SwitchState("Idle");

		// Set params
		entityNode->SetIntParam("HP", 200);
		entityNode->SetIntParam("HP_Max", entityNode->GetIntParam("HP"));
		entityNode->SetIntParam("CameraOffX", 0); // used for lookingUp state
		entityNode->SetIntParam("CameraOffY", 0);
		entityNode->SetIntParam("SpawnX", entityNode->GetPhysicObject()->Position.X);
		entityNode->SetIntParam("SpawnY", entityNode->GetPhysicObject()->Position.Y);

		// Initial equipment set
		EntityPlayer::ChangeEquipment( entityNode, WEAPON_SWORD_IRON, SHIELD_IRON );
	}

	static void ChangeEquipment( EntityNode* entityNode_, eWeapon weaponType ) { ChangeEquipment( entityNode_, weaponType, (eShield)entityNode_->GetIntParam("Shield") ); }
	static void ChangeEquipment( EntityNode* entityNode_, eShield shieldType ) { ChangeEquipment( entityNode_, (eWeapon)entityNode_->GetIntParam("Weapon"), shieldType ); };
	static void ChangeEquipment( EntityNode* entityNode_, eWeapon weaponType, eShield shieldType );
};

