#include "EntityCollisionMgr.h"

EntityCollisionMgr::EntityCollisionMgr( u32 collGroupNb ) :
	updateCounter( 0 ),
	m_groupCounter( collGroupNb )
{
	m_vEntCollBody = new vObject_t[collGroupNb];
	m_isGroupManageCollWith = new bool*[collGroupNb];
	for( int i = 0; i < collGroupNb; i++)
	{
		m_vEntCollBody[i].clear();
		m_isGroupManageCollWith[i] = new bool[collGroupNb];
		memset(m_isGroupManageCollWith[i], false, sizeof(bool) );
	}
}

EntityCollisionMgr::~EntityCollisionMgr()
{
	// remove deleted entities
	delete [] m_vEntCollBody;

	for( int i = 0; i < m_groupCounter; i++)
	{
		delete [] m_isGroupManageCollWith[i];
	}
	delete [] m_isGroupManageCollWith;
}

EntityCollisionBody* EntityCollisionMgr::GetById( unsigned int id )
{
	object_t* pObjContainer = IObjectManager::GetById(id);

	return pObjContainer? pObjContainer->GetData() : 0;
}

unsigned int EntityCollisionMgr::AddCollisionBody( EntityNode* entityNode, eEntityCollisionBodyType type, u32 collGroup,   TEntCollCB collCBFunc, const std::string& name )
{
	ASSERT(collGroup < m_groupCounter, " collGroup is greather than number of groups " << collGroup);

	EntityCollisionBody* pObj = new EntityCollisionBody();

	object_t* dataContainer = AddObjectData( pObj, name );

	m_vEntCollBody[collGroup].push_back( dataContainer );

	dataContainer->GetData()->collCBFunc = collCBFunc;
	dataContainer->GetData()->Owner = entityNode;
	dataContainer->GetData()->Type = type;

	return dataContainer->GetId();
}

void EntityCollisionMgr::RemoveCollisionBody( u16 id )
{
	Destroy( id, true);	
}

void EntityCollisionMgr::Update()
{
	// increase update counter
	++updateCounter;

	// remove deleted entities
	for( int groupIdx = 0; groupIdx < m_groupCounter; ++groupIdx )
	{
		vObject_t::iterator iter( m_vEntCollBody[groupIdx].begin() );
		vObject_t::iterator iter_end( m_vEntCollBody[groupIdx].end() );
		while( iter != iter_end )
		{
			object_t* entCollBodyContainer = (*iter);

			// Remove marked for deletion
			if( entCollBodyContainer->IsMarkedForDeletion() )
			{
				iter = m_vEntCollBody[groupIdx].erase( iter );		
				iter_end = m_vEntCollBody[groupIdx].end();
				
				FreeObjectData( entCollBodyContainer );	
			}
			else
			{
				// next element
				++iter;
			}
		}
	}

	// check collision for each group with the others
	for( int groupIdx = 0; groupIdx < m_groupCounter; ++groupIdx )
	{
		for( int groupIdx2 = groupIdx+1; groupIdx2 < m_groupCounter; ++groupIdx2 )
		{
			if( IsGroupCollides( groupIdx, groupIdx2 ) )
			{
				vObject_t::iterator iter( m_vEntCollBody[groupIdx].begin() );
				vObject_t::iterator iter_end( m_vEntCollBody[groupIdx].end() );
				while( iter != iter_end )
				{
					object_t* entCollBodyContainer = (*iter);

					if( entCollBodyContainer->GetData()->IsEnabled )
					{
						_DoCollision( entCollBodyContainer, m_vEntCollBody[groupIdx2] );
					}

					// next element
					++iter;
				}
			}
		}
	}
}

void EntityCollisionMgr::_DoCollision( object_t* thisCollBodyContainer, const vObject_t& vEntCollBody )
{

	core::rectf thisRect = thisCollBodyContainer->GetData()->CollData + thisCollBodyContainer->GetData()->Owner->GetPhysicObject()->Position;

	// check always with entities after this one, previous were already checked
	vObject_t::const_iterator iter( vEntCollBody.begin() );
	vObject_t::const_iterator iter_end( vEntCollBody.end() );
	for( ; iter != iter_end; ++iter )
	{
		object_t* otherCollBodyContainer = (*iter);
		if(
			( otherCollBodyContainer->GetData()->IsEnabled ) &&
			( false == otherCollBodyContainer->IsMarkedForDeletion() ) &&
			( otherCollBodyContainer->GetData()->collCBFunc || thisCollBodyContainer->GetData()->collCBFunc ) && // at least one is not passive ( receive collision events )
			( otherCollBodyContainer->GetData()->Owner != thisCollBodyContainer->GetData()->Owner )
		)
		{
			core::rectf otherRect = otherCollBodyContainer->GetData()->CollData + otherCollBodyContainer->GetData()->Owner->GetPhysicObject()->Position;

			if( thisRect.isRectCollided( otherRect ) )
			{
				thisCollBodyContainer->GetData()->lastCollisionUpdateCounter = updateCounter;
				otherCollBodyContainer->GetData()->lastCollisionUpdateCounter = updateCounter;
				if( thisCollBodyContainer->GetData()->collCBFunc )		{ thisCollBodyContainer->GetData()->collCBFunc( thisCollBodyContainer->GetData(), otherCollBodyContainer->GetData() ); }
				if( otherCollBodyContainer->GetData()->collCBFunc )	{ otherCollBodyContainer->GetData()->collCBFunc( otherCollBodyContainer->GetData(), thisCollBodyContainer->GetData() ); }				
			}
		}
	}
}