#include "EntityFactory.h"

#include "GameManager.h"

// Add all entity headers here
#include "EntityPlayer.h"
#include "EntityFireSnake.h"
#include "EntityMushSmall.h"
#include "EntityRock.h"
#include "EntityBlock.h"
#include "EntityCollectible.h"
#include "EntityDropItem.h"
#include "EntitySlime.h"
#include "EntityRockMouth.h"
#include "EntityChest.h"
#include "EntityDoor.h"
#include "EntityVisualFx.h"
//---


EntityFactory::~EntityFactory(void)
{
}

EntityNode* EntityFactory::CreateEntityFromTemplate( const std::string& sTemplate, const std::string& dstLayerName, const std::string& entityName, io::IrrXMLReader* xmlParams )
{
	EntityNode* entityNode = CreateEntity( dstLayerName, entityName );
	BuildEntityFromTemplate( entityNode , sTemplate, xmlParams);
	return entityNode;
}

//NOTE: these two methods below were made to be able to create first an entity, the apply params from Tiled, and finally call BuildEntity after thouse params were set
EntityNode* EntityFactory::CreateEntity( const std::string& dstLayerName, const std::string& entityName )
{
	return m_gameMgr->GetEntityManager()->CreateEntity( dstLayerName, entityName);
}

bool EntityFactory::BuildEntityFromTemplate( EntityNode* entity, const std::string& sTemplate, io::IrrXMLReader* xmlParams )
{

	//TODO: change this for a hash table with template to pointer to function (BuildEntity)
	// Entity template list
	if( !sTemplate.compare("player") )
	{
		if( xmlParams )
		{
			entity->SetIntParam("offsetX", xmlParams->getAttributeValueAsInt("offsetX"));
			entity->SetIntParam("offsetY", xmlParams->getAttributeValueAsInt("offsetY"));
			entity->SetFloatParam("jumpVeloc", xmlParams->getAttributeValueAsFloat("jumpVeloc") );
			entity->SetFloatParam("movingVeloc", xmlParams->getAttributeValueAsFloat("movingVeloc") );
			entity->SetFloatParam("movingAcc", xmlParams->getAttributeValueAsFloat("movingAcc") );		
			entity->SetFloatParam("jumpAcc", xmlParams->getAttributeValueAsFloat("jumpAcc") );
			entity->SetFloatParam("jumpMoveAcc", xmlParams->getAttributeValueAsFloat("jumpMoveAcc") );
			entity->SetStringParam("Sprite", xmlParams->getAttributeValue("sprite") );
		}
		//!!!TODO: load from file
		else
		{
			entity->SetFloatParam("jumpVeloc", 250 );
			entity->SetFloatParam("movingVeloc", 75 );
			entity->SetFloatParam("movingAcc", 1000 );
			entity->SetFloatParam("jumpAcc", 200 );
			entity->SetFloatParam("jumpMoveAcc", 600 );
			entity->SetStringParam("Sprite", "sprites\\playerSprite.sprite" );
		}

		EntityPlayer::BuildEntity( entity, xmlParams );
	}
	else if( !sTemplate.compare("placeable") )
	{
		entity->CreatePhysicObject();
		entity->GetPhysicObject()->IsApplyGravity = false;
		entity->SetSpriteComponent("sprites\\deco.sprite");

		/*
		entity_ret->setPosition( core::vector3df( xmlParams->getAttributeValueAsFloat("offsetX"), xmlParams->getAttributeValueAsFloat("offsetY"), 0) );
		entity_ret->SetSpriteComponent(xmlParams->getAttributeValueSafe("spriteSource"));
		entity_ret->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag(xmlParams->getAttributeValueSafe("animName"), -1);
		*/

	}
	else if( !sTemplate.compare("visualFx") )
	{
		EntityVisualFx::BuildEntity( entity, xmlParams);
	}
	else if( !sTemplate.compare("fireSnake") )
	{
		EntityFireSnake::BuildEntity( entity, xmlParams );
	}
	else if( !sTemplate.compare("mushroomSmall") )
	{
		EntityMushSmall::BuildEntity( entity, xmlParams );
	}
	else if( !sTemplate.compare("slime") )
	{
		EntitySlime::BuildEntity( entity, xmlParams );
	}
	else if( !sTemplate.compare("rock") )
	{
		EntityRock::BuildEntity( entity );
	}
	else if( !sTemplate.compare("block") )
	{
		EntityBlock::BuildEntity( entity );
	}
	else if( !sTemplate.compare("collectible") )
	{
		EntityCollectible::BuildEntity( entity );
	}
	else if( !sTemplate.compare("dropItem") )
	{
		EntityDropItem::BuildEntity( entity );
	}
	else if( !sTemplate.compare("rock_mouth") )
	{
		EntityRockMouth::BuildEntity( entity );
	}
	else if( !sTemplate.compare("chest") )
	{
		EntityChest::BuildEntity( entity );
	}
	else if( !sTemplate.compare("door") )
	{
		EntityDoor::BuildEntity( entity );

		entity->SetIntParam("offsetX", xmlParams->getAttributeValueAsInt("offsetX"));
		entity->SetIntParam("offsetY", xmlParams->getAttributeValueAsInt("offsetY"));
		entity->SetStringParam("targetRoom", xmlParams->getAttributeValue("targetRoom"));
		entity->SetStringParam("targetDoor", xmlParams->getAttributeValue("targetDoor"));
	}
	else
	{
		return false;
	}

	if( entity->IsIntParamSet("offsetX") )
	{
		entity->GetPhysicObject()->Position.X += entity->GetIntParam("offsetX");
	}
	if( entity->IsIntParamSet("offsetY") )
	{
		entity->GetPhysicObject()->Position.Y += entity->GetIntParam("offsetY");
	}

	return true;
}

EntityNode* EntityFactory::CreateProjectile( 
	EntityNode* entityParent, 
	const std::string& sSprite, 
	const std::string& sFlyingAnim,
	const std::string& sCollBody,
	const core::vector2df& position, 
	const core::vector2df& velocity, 
	bool isGravityOn 
)
{
		EntityNode* projectile = CreateEntity( entityParent->getParent()->getName(), "" );

		projectile->CreatePhysicObject();

		projectile->GetPhysicObject()->Position = position;
		projectile->GetPhysicObject()->Velocity = velocity;
		projectile->GetPhysicObject()->IsApplyGravity = isGravityOn;

		projectile->SetSpriteComponent( sSprite );
		projectile->GetPhysicObject()->BoundingBox = projectile->GetSpriteComponent()->GetSprite()->GetMkRectDataByTag( sCollBody );
		projectile->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag(sFlyingAnim, -1);

		BuildEntityFromTemplate( projectile, "projectile", 0);

		return projectile;
}

EntityNode* EntityFactory::CreateDropItem( 
	EntityNode* entityParent, 
	eItemType itemType,
	int itemSubtype
)
{
	EntityNode* entity = CreateEntity( entityParent->getParent()->getName(), "" );
	entity->SetIntParam("ItemType", itemType);
	entity->SetIntParam("ItemSubtype", itemSubtype);
	BuildEntityFromTemplate( entity, "dropItem", 0);
	return entity;
}