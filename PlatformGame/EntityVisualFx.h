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

class EntityVisualFx
{
public:
	EntityVisualFx(void);
	~EntityVisualFx(void);


	// States
	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
		

	static void BuildEntity( EntityNode* entityNode, io::IrrXMLReader* xmlParams )
	{
		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->Position = core::vector2df( 0, 0 );
		entityNode->GetPhysicObject()->IsApplyGravity = false;
		entityNode->GetPhysicObject()->IsClipping = true;

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		
		// States
		stateMachine->SetStateFunction("Idle", (TStateFP)State_Idle);
				

		stateMachine->SwitchState("Idle");
	}
};

