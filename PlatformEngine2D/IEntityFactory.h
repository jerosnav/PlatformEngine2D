#pragma once

#include "IGameManager.h"
#include "EntityNode.h"

class IEntityFactory
{
public:

	IEntityFactory( IGameManager* gameMgr ) :
	  m_gameMgr( gameMgr ){}

	virtual EntityNode* CreateEntityFromTemplate( const std::string& sTemplate, const std::string& dstLayerName, const std::string& entityName, io::IrrXMLReader* xmlParams ) = 0;
	virtual EntityNode* CreateEntity( const std::string& dstLayerName, const std::string& entityName ) = 0;
	virtual bool BuildEntityFromTemplate( EntityNode* entity, const std::string& sTemplate, io::IrrXMLReader* xmlParams ) = 0;


protected:

	IGameManager* m_gameMgr;

};