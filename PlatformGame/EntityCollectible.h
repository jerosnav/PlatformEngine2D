#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionMgr.h"
#include "EntityCollisionFlags.h"
#include "MathUtil.h"


class EntityCollectible
{
public:
	EntityCollectible(void);
	~EntityCollectible(void);

	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	static void Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

	static void BuildEntity( EntityNode* entityNode )
	{

		entityNode->SetSpriteComponent("sprites\\collectible.sprite");

		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");
		entityNode->GetPhysicObject()->IsApplyGravity = false;
		entityNode->GetPhysicObject()->IsClipping = true;

		// Set entity collisions
		entityNode->AddEntityCollisionBody("CollBody", 
			MathUtil::RectI2F( entityNode->GetPhysicObject()->BoundingBox ), 
			eCollGroups::COLLISION_GROUP_BLOCKS, eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY, Collision_Body );

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
		stateMachine->SetStateFunction("Dying", (TStateFP)State_Dying);

		stateMachine->SwitchState("Idle");
	}
};