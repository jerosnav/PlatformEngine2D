#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"

class EntityFireSnake
{
public:
	EntityFireSnake(void);
	~EntityFireSnake(void);


	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Attacking(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);


	static void BuildEntity( EntityNode* entityNode, io::IrrXMLReader* xmlParams )
	{

		entityNode->SetSpriteComponent("sprites\\fireSnake.sprite");
		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->Position = core::vector2df( xmlParams->getAttributeValueAsFloat("offsetX"), xmlParams->getAttributeValueAsFloat("offsetY") );
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
		stateMachine->SetStateFunction("Attacking", (TStateFP)State_Attacking);

		stateMachine->SwitchState("Idle");
	}
};

