#pragma once


#include <irrlicht.h>
#include "EntityNode.h"
#include "IEntityStateMachine.h"
#include "InputKeyboard.h"
#include "GameManager.h"
#include "EntityCollisionMgr.h"
#include "EntityCollisionFlags.h"
#include "MathUtil.h"


class EntityDropItem
{
public:
	EntityDropItem(void);
	~EntityDropItem(void);

	static void State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);
	static void State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_);

	static void Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

	static void BuildEntity( EntityNode* entityNode )
	{

		entityNode->SetSpriteComponent("sprites\\dropItem.sprite");

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
		stateMachine->SetStateFunction("Dying", (TStateFP)State_Dying);

		stateMachine->SwitchState("Idle");

		// Set params
		int itemType = entityNode->GetIntParam("ItemType");
		int itemSubtype = entityNode->GetIntParam("ItemSubtype");
		switch( itemType )
		{
			case ITEM_TYPE_WEAPON:
				switch( itemSubtype )
				{
					case WEAPON_SWORD_BONE:
						entityNode->SetStringParam("AnimUp", "Sword_Bone");
						entityNode->SetStringParam("AnimDown", "Sword_Bone");
						entityNode->SetStringParam("AnimFloor", "Sword_Bone");
						break;
					case WEAPON_SWORD_BRONZE:
						entityNode->SetStringParam("AnimUp", "Sword_Bronze");
						entityNode->SetStringParam("AnimDown", "Sword_Bronze");
						entityNode->SetStringParam("AnimFloor", "Sword_Bronze");
						break;
					case WEAPON_SWORD_IRON:
						entityNode->SetStringParam("AnimUp", "Sword_Iron");
						entityNode->SetStringParam("AnimDown", "Sword_Iron");
						entityNode->SetStringParam("AnimFloor", "Sword_Iron");
						break;
					case WEAPON_SWORD_MAGIC:
						entityNode->SetStringParam("AnimUp", "Sword_Magic");
						entityNode->SetStringParam("AnimDown", "Sword_Magic");
						entityNode->SetStringParam("AnimFloor", "Sword_Magic");
						break;
				}
				break;
			case ITEM_TYPE_SHIELD:
				switch( itemSubtype )
				{
					case SHIELD_WOOD:
						entityNode->SetStringParam("AnimUp", "Shield_Wood");
						entityNode->SetStringParam("AnimDown", "Shield_Wood");
						entityNode->SetStringParam("AnimFloor", "Shield_Wood");
						break;
					case SHIELD_IRON:
						entityNode->SetStringParam("AnimUp", "Shield_Iron");
						entityNode->SetStringParam("AnimDown", "Shield_Iron");
						entityNode->SetStringParam("AnimFloor", "Shield_Iron");
						break;
					case SHIELD_MAGIC:
						entityNode->SetStringParam("AnimUp", "Shield_Magic");
						entityNode->SetStringParam("AnimDown", "Shield_Magic");
						entityNode->SetStringParam("AnimFloor", "Shield_Magic");
						break;
				}
				break;

			case ITEM_TYPE_MONEY:
				entityNode->SetStringParam("AnimUp", "Money_Up");
				entityNode->SetStringParam("AnimDown", "Money_Down");
				entityNode->SetStringParam("AnimFloor", "Money_Floor");
				break;
		}
	}
};