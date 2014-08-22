#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>
#include "EntityNode.h"
#include "IObjectManager.h"
#include "PhyObject.h"

class EntityNode;

enum eEntityCollisionBodyType
{
	COLLISION_TYPE_RECT,
	COLLISION_TYPE_LINE,
	COLLISION_TYPE_DOT
};

struct EntityCollisionBody;
typedef void (*TEntCollCB)( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

struct EntityCollisionBody
{

	friend class EntityCollisionMgr;

	TEntCollCB collCBFunc;
	EntityNode* Owner;
	eEntityCollisionBodyType Type;
	core::rectf CollData;
	bool IsEnabled;

	u32 Flags; // used to allow different kind of collisions like collisions group

	EntityCollisionBody() :
	collCBFunc(0),
	IsEnabled( true ),
	Flags( 0 )
	{}

private:

	u32 lastCollisionUpdateCounter; // value of updateCounter last time there was a collision, for debug purpose
};

class EntityCollisionMgr : protected IObjectManager<EntityCollisionBody>
{
private:

	u32 m_groupCounter;
	bool **m_isGroupManageCollWith;
	vObject_t *m_vEntCollBody;

	u32 updateCounter;

public:

	EntityCollisionMgr( u32 collGroupNb );
	~EntityCollisionMgr();

	void SetCollisionGroup( u32 collGroup0, u32 collGroup1, bool isCollides ){ m_isGroupManageCollWith[collGroup0][collGroup1] = m_isGroupManageCollWith[collGroup1][collGroup0] = isCollides; }
	bool IsGroupCollides( u32 collGroup0, u32 collGroup1 ){ return m_isGroupManageCollWith[collGroup0][collGroup1]; }

	unsigned int AddCollisionBody( EntityNode* entityNode, eEntityCollisionBodyType type, u32 collGroup, TEntCollCB collCBFunc = 0, const std::string& name = "" );

	void RemoveCollisionBody( u16 id );

	void Update();

	bool IsCollision( u16 id ){ return updateCounter == IObjectManager::GetById( id )->GetData()->lastCollisionUpdateCounter; }

	u32 GetCollisionBodyCounter() { int ret = 0; for( int i = 0; i < m_groupCounter; i++){ ret += m_vEntCollBody[i].size(); } return ret; }

	EntityCollisionBody* GetById( unsigned int id );

private:

	void _DoCollision( object_t* thisCollBodyContainer, const vObject_t& vEntCollBody );

};