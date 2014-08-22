#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>
#include "IGameManager.h"
#include "IObjectManager.h"

// TODO: use this class for entities instead EntityNode and include all components inside this class
class Entity
{

	friend class EntityManager;

public:

	Entity();
	~Entity();

	EntityNode* GetEntityNode(){ return m_entityNodeRef; }

private:
	EntityNode* m_entityNodeRef;
};

class EntityManager : public IObjectManager<Entity>
{
private:

	IGameManager* m_gameMgr;

public:

	EntityManager( IGameManager* gameMgr ) : IObjectManager( irr::u16(-1) ),
		m_gameMgr( gameMgr )
	{}

	~EntityManager();

	EntityNode* CreateEntity( const std::string& dstLayerName, const std::string& entityName );

	void DestroyEntity( EntityNode* entityNode );

	EntityNode* GetEntityByTag( const std::string& entityName );

	size_t GetNbOfEntityWithTag( const std::string& entityName );
};