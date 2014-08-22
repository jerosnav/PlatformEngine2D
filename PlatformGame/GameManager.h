#pragma once

#include "IGameManager.h"

class GuiManager;

enum eCollGroups
{
	COLLISION_GROUP_PLAYER,		// collides with enemies and blocks
	COLLISION_GROUP_ENEMIES,	// collides with player
	COLLISION_GROUP_BLOCKS,		// collides with player
//Size of enum
	COLLISION_GROUP_SIZE
};

enum eItemType
{
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_SHIELD,
	ITEM_TYPE_MONEY
};

enum eWeapon
{
	WEAPON_SWORD_NONE,
	WEAPON_SWORD_BONE,
	WEAPON_SWORD_BRONZE,
	WEAPON_SWORD_IRON,
	WEAPON_SWORD_MAGIC,
	WEAPON_SIZE
};

enum eShield
{
	SHIELD_NONE,
	SHIELD_WOOD,
	SHIELD_IRON,
	SHIELD_MAGIC,
	SHIELD_SIZE
};

class GameManager : public IGameManager
{
public:

	GameManager(void);
	~GameManager(void);


	s32 Initialize();
	s32 Run();
	s32 LoadRoom( const char *sRoomFile );
	void UnloadRoom(  );

	GuiManager* GetGuiManager() { return m_guiManager; }

	void GoToRoom( const char* roomFile, const char* doorTarget );


// Damage Manager
	void ApplyDamage(EntityNode *srcEntity, EntityNode *dstEntity, s32 damage, s32 damageType, s32 damageDir)
	{
		if(
			( dstEntity->GetStateMachine()->GetCurrentState().compare("Hurt") != 0 ) &&
			( dstEntity->GetStateMachine()->GetCurrentState().compare("Dying") != 0 ) &&
			( !dstEntity->IsIntParamSet("Invencible") || (dstEntity->GetIntParam("Invencible") > 0) )
		)
		{
			dstEntity->SetIntParamOffset( "HP", -damage );
			dstEntity->SetIntParam( "DamageType", damageType );
			dstEntity->SetIntParam( "DamageDir", damageDir );

			if( dstEntity->GetStateMachine()->IsValidState("Hurt") )
			{
				dstEntity->GetStateMachine()->SwitchState("Hurt");
			}
		}
	}
//---

private:

	GuiManager* m_guiManager;

	//+++ Game States
	static void State_TitleScreen(s32 messageType_, GameManager* gameMgr_, s32 dataSize_);
	static void State_LoadingRoom(s32 messageType_, GameManager* gameMgr_, s32 dataSize_);
	static void State_GamePlay(s32 messageType_, GameManager* gameMgr_, s32 dataSize_);
	
	//+++ moving room
	std::string m_loadRoom_roomToBeLoaded;
	std::string m_loadRoom_targetDoor;
};

