#include "EntityPlayer.h"
#include "GameManager.h"

EntityPlayer::EntityPlayer(void)
{
}


EntityPlayer::~EntityPlayer(void)
{
}

void EntityPlayer::Collision_Sword_Side( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	if(
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY )
	)
	{
		DBG(" <EntityPlayer::Collision_Sword_Side> Collided Enemy ");

		bool isFlipX = ( thisCollBody->Owner->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects == eSpriteEffects::SPRITE_EFFECT_FLIP_X );
		((GameManager *)thisCollBody->Owner->GetGameManager())->ApplyDamage(0, otherCollBody->Owner, 5, 0, isFlipX? eDirFlag::LEFT : eDirFlag::RIGHT );
	}
}

void EntityPlayer::DoJumpAgainstWallCheck( EntityNode* entityNode_ )
{
	PhyObject* phyObj = entityNode_->GetPhysicObject();

	if( 
		InputKeyboard::IsKeyPressed( EKEY_CODE::KEY_KEY_Z ) &&
		( false == phyObj->IsOnPlatform )
	)
	{
		// Check if has a block tile in the bottom side to jump according to player direction
		irr::f32 xf;

		// check if collides at left side, if not, check right side
		xf = ( phyObj->BoundingBox.UpperLeftCorner.X - 1 );
		LayerTile* layerTile = entityNode_->GetGameManager()->GetLevel()->GetPhyLayerTileAtPos( phyObj->Position.X + xf, phyObj->Position.Y - 8 );	

		if( 
			( layerTile->GetTilesetId() < 0 ) ||
			( layerTile->GetTileId() != 0x00000000 )  // check if tile has type full red block
		)
		{
			xf = ( phyObj->BoundingBox.LowerRightCorner.X + 1 );
			layerTile = entityNode_->GetGameManager()->GetLevel()->GetPhyLayerTileAtPos( phyObj->Position.X + xf, phyObj->Position.Y - 8 );	
		}
			
		if( 
			( layerTile->GetTilesetId() >= 0 ) && 
			( layerTile->GetTileId() == 0x00000000 )  // check if tile has type full red block
		) 
		{
			phyObj->Velocity.X = ( xf > 0 )? -100: 100; // apply side velocity
			entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects ^= eSpriteEffects::SPRITE_EFFECT_FLIP_X;
			entityNode_->GetStateMachine()->SwitchState("Jumping");			
		}
	}
}

void EntityPlayer::ChangeEquipment( EntityNode* entityNode_, eWeapon weaponType, eShield shieldType )
{
	entityNode_->SetIntParam("Weapon", weaponType);
	entityNode_->SetIntParam("Shield", shieldType);

	//save Sprite effects
	irr::u32 savedSpriteEffects = entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects;

	entityNode_->GetSpriteComponent()->SetSprite( entityNode_->GetStringParam("Sprite") );
	
	//load Sprite effects
	entityNode_->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects = savedSpriteEffects;

	Sprite* sprite = entityNode_->GetSpriteComponent()->GetSprite();
	for( std::vector<Frame*>::iterator iter( sprite->Frames.begin() ); iter != sprite->Frames.end(); ++iter)
	{
		Frame* frame = *iter;

		for( std::vector<FModule*>::iterator iter2( frame->Modules.begin() ); iter2 != frame->Modules.end(); ++iter2 )
		{
			FModule* fmodule = *iter2;

			if( !sprite->GetModule( fmodule->Id )->Tag.compare( "Sword_Iron0" ) )
			{
				switch( weaponType )
				{
					case WEAPON_SWORD_NONE: fmodule->Id = sprite->GetModuleByTag("Empty")->Id; break;
					case WEAPON_SWORD_BONE: fmodule->Id = sprite->GetModuleByTag("Sword_Bone0")->Id; break;
					case WEAPON_SWORD_BRONZE: fmodule->Id = sprite->GetModuleByTag("Sword_Bronze0")->Id; break;
					case WEAPON_SWORD_MAGIC: fmodule->Id = sprite->GetModuleByTag("Sword_Magic0")->Id; break;
				}
			}
			else
			if( !sprite->GetModule( fmodule->Id )->Tag.compare( "Sword_Iron1" ) )
			{
				switch( weaponType )
				{
					case WEAPON_SWORD_NONE: fmodule->Id = sprite->GetModuleByTag("Empty")->Id; break;
					case WEAPON_SWORD_BONE: fmodule->Id = sprite->GetModuleByTag("Sword_Bone1")->Id; break;
					case WEAPON_SWORD_BRONZE: fmodule->Id = sprite->GetModuleByTag("Sword_Bronze1")->Id; break;
					case WEAPON_SWORD_MAGIC: fmodule->Id = sprite->GetModuleByTag("Sword_Magic1")->Id; break;
				}
			}
			else
			if( !sprite->GetModule( fmodule->Id )->Tag.compare( "Sword_Iron2" ) )
			{
				switch( weaponType )
				{
					case WEAPON_SWORD_NONE: fmodule->Id = sprite->GetModuleByTag("Empty")->Id; break;
					case WEAPON_SWORD_BONE: fmodule->Id = sprite->GetModuleByTag("Sword_Bone2")->Id; break;
					case WEAPON_SWORD_BRONZE: fmodule->Id = sprite->GetModuleByTag("Sword_Bronze2")->Id; break;
					case WEAPON_SWORD_MAGIC: fmodule->Id = sprite->GetModuleByTag("Sword_Magic2")->Id; break;
				}
			}
			else
			if( !sprite->GetModule( fmodule->Id )->Tag.compare( "Shield_Iron0" ) )
			{
				switch( shieldType )
				{
					case SHIELD_NONE: fmodule->Id = sprite->GetModuleByTag("Empty")->Id; break;
					case SHIELD_WOOD: fmodule->Id = sprite->GetModuleByTag("Shield_Wood0")->Id; break;
					case SHIELD_MAGIC: fmodule->Id = sprite->GetModuleByTag("Shield_Magic0")->Id; break;
				}
			}
			else
			if( !sprite->GetModule( fmodule->Id )->Tag.compare( "Shield_Iron1" ) )
			{
				switch( shieldType )
				{
					case SHIELD_NONE: fmodule->Id = sprite->GetModuleByTag("Empty")->Id; break;
					case SHIELD_WOOD: fmodule->Id = sprite->GetModuleByTag("Shield_Wood1")->Id; break;
					case SHIELD_MAGIC: fmodule->Id = sprite->GetModuleByTag("Shield_Magic1")->Id; break;
				}
			}
		}
	}
}