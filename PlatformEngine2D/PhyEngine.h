#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>

#include "PhyObject.h"


struct PhyObjectContainer
{
	PhyObject* phyObject;
	bool isMarkedForDeletion;

	PhyObjectContainer()
		: phyObject(0),
		isMarkedForDeletion(false)
	{}

	PhyObjectContainer( PhyObject* phyObject ) :
	phyObject( phyObject ),
	isMarkedForDeletion(false)
	{}

	~PhyObjectContainer()
	{
		delete phyObject;
	}
};

class PhyEngine
{

private:

	typedef std::vector<PhyObjectContainer*> TPhyObjContainerVector;
	typedef std::map<u32, PhyObjectContainer*> TPhyObjContainerMap;

	TPhyObjContainerVector	m_vPhyObjects;
	TPhyObjContainerMap	m_dicPhyObjects;

	u32 m_nextId;

	core::vector2df m_gravity;

private:

	void _UpdatePosition( PhyObject* phyObject, irr::u32 frameDT );
	void _UpdateVelocity( PhyObject* phyObject, irr::u32 frameDT );

	virtual void _DoPhysics( PhyObject* phyObject ) = 0;
	virtual bool _GetIsOnPlatform( PhyObject* phyObject ) = 0;
	virtual void _ApplyMagneticDistance( PhyObject* phyObject ) = 0;

public:
	
	PhyEngine() :
		m_nextId(0),
		m_gravity(0, 400)
	{}

	~PhyEngine()
	{
		TPhyObjContainerVector::iterator iter( m_vPhyObjects.begin() );
		TPhyObjContainerVector::iterator iter_end( m_vPhyObjects.end() );
		while( iter != iter_end )
		{
			delete (*iter);
			++iter;
		}
	}

	void Update( irr::u32 frameDT, bool isResetCollisionFlags = true );

	u32 CreatePhyObject( ePhyObjectType type );
	void RemovePhyObject( u32 id );

	core::vector2df GetGravity(){ return m_gravity;}
	void SetGravity( core::vector2df value ){ m_gravity = value;}

	PhyObject* GetPhyObject( u32 id )
	{
		//!!!NOTE: this is too slow> return m_dicPhyObjects.find( id ) !=  m_dicPhyObjects.end()? m_dicPhyObjects[id]->phyObject : 0;
		return m_dicPhyObjects[id]->phyObject;
	}
};