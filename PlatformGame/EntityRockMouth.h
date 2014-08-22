#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionMgr.h"
#include "MathUtil.h"

class EntityRockMouth
{
public:

	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Attack(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	static void BuildEntity( EntityNode* entityNode )
	{

		entityNode->SetSpriteComponent("sprites\\rock_mouth.sprite");
		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->IsApplyGravity = false;
		entityNode->GetPhysicObject()->IsClipping = true;

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
		stateMachine->SetStateFunction("Attack", (TStateFP)State_Attack);

		stateMachine->SwitchState("Idle");

	}
};