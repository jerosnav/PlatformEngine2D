#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionMgr.h"
#include "EntityCollisionFlags.h"
#include "MathUtil.h"


class EntityChest
{
public:
	EntityChest(void);
	~EntityChest(void);

	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Opening(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	static void Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

	static void BuildEntity( EntityNode* entityNode )
	{

		entityNode->SetSpriteComponent("sprites\\chest.sprite");

		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");
		entityNode->GetPhysicObject()->IsApplyGravity = true;

		// Set entity collisions
		entityNode->AddEntityCollisionBody("CollBody", 
			MathUtil::RectI2F( entityNode->GetPhysicObject()->BoundingBox ), 
			eCollGroups::COLLISION_GROUP_BLOCKS, eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY, Collision_Body );

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
		stateMachine->SetStateFunction("Opening", (TStateFP)State_Opening);

		stateMachine->SwitchState("Idle");
	}
};