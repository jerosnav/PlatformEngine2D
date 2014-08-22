#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionMgr.h"
#include "MathUtil.h"

class EntityRock
{
public:

	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	static void Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

	static void BuildEntity( EntityNode* entityNode )
	{
		// Setup entity collision
		entityNode->CreatePhysicObject();
		entityNode->SetSpriteComponent( "sprites\\rock.sprite" );
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( "CollBody" );
		entityNode->AddEntityCollisionBody("CollBody", MathUtil::RectI2F( entityNode->GetPhysicObject()->BoundingBox ), eCollGroups::COLLISION_GROUP_ENEMIES, 0, Collision_Body );

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);

		stateMachine->SwitchState("Idle");
	}

	static void DoDestroyRock( EntityNode* entityNode );
};