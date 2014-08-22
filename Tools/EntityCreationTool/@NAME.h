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

class @NAME
{
public:
	@NAME(void);
	~@NAME(void);


	// States
	<STATES1>	

	static void BuildEntity( EntityNode* entityNode, io::IrrXMLReader* xmlParams )
	{
		entityNode->SetSpriteComponent( "Sprite.sprite" );
		entityNode->CreatePhysicObject();
		entityNode->GetPhysicObject()->Position = core::vector2df( 0, 0 );
		entityNode->GetPhysicObject()->BoundingBox = entityNode->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag("CollBody");

		// Setup entity collision
		Sprite* sprite = entityNode->GetSpriteComponent()->GetSprite();
		entityNode->AddEntityCollisionBody("CollBody", MathUtil::RectI2F( entityNode->GetPhysicObject()->BoundingBox ), eCollGroups::COLLISION_GROUP_PLAYER, ENTITY_COLLISION_FLAG_COLLBODY );

		// Setup state machine
		IEntityStateMachine* stateMachine = new IEntityStateMachine( entityNode );
		entityNode->SetStateMachine( stateMachine );
		
		// States
		<STATES2>		

		stateMachine->SwitchState("Idle");

		// Set params
		entityNode->SetIntParam("HP", 200);
		entityNode->SetIntParam("HP_Max", entityNode->GetIntParam("HP"));

	}
};

