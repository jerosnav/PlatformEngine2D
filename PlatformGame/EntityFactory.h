#pragma once

#include "GameManager.h"
#include "IEntityFactory.h"
#include "EntityNode.h"

class EntityFactory : public IEntityFactory
{
public:
	EntityFactory( IGameManager* gameMgr ) : IEntityFactory( gameMgr )	{}
	~EntityFactory(void);

	virtual EntityNode* CreateEntityFromTemplate( const std::string& sTemplate, const std::string& dstLayerName, const std::string& entityName, io::IrrXMLReader* xmlParams );
	virtual EntityNode* CreateEntity( const std::string& dstLayerName, const std::string& entityName );
	virtual bool BuildEntityFromTemplate( EntityNode* entity, const std::string& sTemplate, io::IrrXMLReader* xmlParams );

	EntityNode* CreateProjectile( EntityNode* entityParent, const std::string& sSprite, const std::string& sFlyingAnim, const std::string& sCollBody, const core::vector2df& position, const core::vector2df& velocity, bool isGravityOn );
	EntityNode* EntityFactory::CreateDropItem( EntityNode* entityParent, eItemType itemType, int itemSubtype );
};

