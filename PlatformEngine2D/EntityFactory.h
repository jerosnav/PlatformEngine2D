#pragma once

#include <irrlicht.h>
#include <irrXML.h>
#include "GameManager.h"
#include "EntityNode.h"
#include "EntityManager.h"
#include "Level.h"

class EntityFactory
{
	GameManager* m_gameMgr;

public:

	EntityFactory( GameManager* gameMgr ) 
		: m_gameMgr( gameMgr )
	{}

	// xmlParams, used to get parameter, promise you want modify it
	EntityNode* CreateEntityFromTemplate( std::string sTemplate, std::string dstLayerName, std::string entityName, io::IrrXMLReader* xmlParams );
};