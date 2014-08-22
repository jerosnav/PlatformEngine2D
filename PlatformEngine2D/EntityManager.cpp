#include "EntityManager.h"

Entity::Entity() :
	m_entityNodeRef( 0 )
{}

Entity::~Entity()
{
	/*
	if( m_entityNodeRef )
	{
		m_entityNodeRef->getSceneManager()->addToDeletionQueue( m_entityNodeRef );
	}
	*/
}

EntityManager::~EntityManager()
{
	DBG("~EntityManager");
}

EntityNode* EntityManager::CreateEntity( const std::string& dstLayerName, const std::string& entityName )
{
	Entity* entity = new Entity();

	EntityNode* entityNode = new EntityNode(m_gameMgr->GetLevel()->GetLayerNode( dstLayerName ), m_gameMgr->GetSceneManager(), -1, m_gameMgr);
	entityNode->drop(); // release this reference so node is removed before free container data

	entity->m_entityNodeRef = entityNode;

	object_t* dataContainer = AddObjectData( entity, entityName );

	// set name
	entityNode->setName( entityName.c_str() );

	// get final key [16bits|16bits] -> [layerId][entityId]
	s32 layerId = m_gameMgr->GetLevel()->GetLayerNode( dstLayerName )->getID();
	s32 entityId = (layerId << 16) | ( dataContainer->GetId() & 0xFFFF );
	entityNode->setID(entityId);
	
	return entity->GetEntityNode();
}

void EntityManager::DestroyEntity( EntityNode* entityNode )
{
	if( entityNode )
	{
		s32 entityId = ( entityNode->getID() & 0xFFFF );

		entityNode->getSceneManager()->addToDeletionQueue( entityNode );
		Destroy(entityId);
	}
}

EntityNode* EntityManager::GetEntityByTag( const std::string& entityName )
{ 
	object_t* objContainer = IObjectManager::GetFirstByTag( entityName ); 

	return objContainer? objContainer->GetData()->GetEntityNode() : 0;
}

size_t EntityManager::GetNbOfEntityWithTag( const std::string& entityName )
{
	return IObjectManager::m_mapObjectTag[ entityName ].size();
}