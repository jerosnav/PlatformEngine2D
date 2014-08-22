#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionFlags.h"

#include "MathUtil.h"

class EntityMushSmall
{
public:
	EntityMushSmall(void);
	~EntityMushSmall(void);


	static void State_Walking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Hurt(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	static void Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );


	static void BuildEntity( EntityNode* entityNode, io::IrrXMLReader* xmlParams )
	{

		entityNode->SetSpriteComponent("sprites\\mushroom_small.sprite");
		entityNode->CreatePhysicObject();
		//entityNode->GetPhysicObject()->Position = core::vector2df( xmlParams->getAttributeValueAsFloat("offsetX"), xmlParams->getAttributeValueAsFloat("offsetY") );
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");		

		// Set entity collisions
		entityNode->AddEntityCollisionBody("CollBody", MathUtil::RectI2F( entityNode->GetPhysicObject()->BoundingBox ), eCollGroups::COLLISION_GROUP_ENEMIES, eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY, Collision_Body );

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );

		//stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
		stateMachine->SetStateFunction("Walking", (TStateFP)State_Walking);
		stateMachine->SetStateFunction("Hurt", (TStateFP)State_Hurt);
		stateMachine->SetStateFunction("Dying", (TStateFP)State_Dying);

		stateMachine->SwitchState("Walking");

		// Set params
		entityNode->SetIntParam("HP", 15);
	}
};

