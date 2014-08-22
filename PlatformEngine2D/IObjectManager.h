#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

template <class T>
class IObjectManager;

template <class T>
class ObjContainer
{

friend IObjectManager<T>;

private:
	T* pData;
	unsigned int id;
	std::string tag;
	bool markedForDeletion;

public:
	unsigned int GetId(){ return id; }
	T* GetData(){ return pData; }
	bool IsMarkedForDeletion() { return markedForDeletion; }
	std::string GetTag() { return tag; }
};

template <class T>
class IObjectManager
{

protected:
	typedef ObjContainer<T>										object_t;
	typedef std::vector<object_t*>								vObject_t;
	typedef tr1::unordered_map< unsigned int, object_t*>		mapObject_t;
	typedef tr1::unordered_map< std::string, vObject_t>			mapObjectTag_t;

private:

	unsigned int m_IdCnt; // id to be asigned in next object creation in case is available

	unsigned int m_IdMax; // maximun id value ( in case you want to reserve Most Significant Bits for something else )

	bool m_isMapTagIterValid;
	std::string m_iteratedObjTag;
	typename vObject_t::iterator m_mapTagIter;

protected:
	
	mapObject_t		m_mapObject;
	mapObjectTag_t	m_mapObjectTag;

private:

	unsigned int GetNextFreeId();

protected:

	bool FreeObjectData( object_t* pObjCont );

public:

	IObjectManager( unsigned int maxId = -1 ) :
		m_IdCnt( 0 ),
		m_IdMax( maxId ),
		m_isMapTagIterValid( false )
	{}
	
	virtual ~IObjectManager()
	{
		while( m_mapObject.size() > 0 )
		{			
			mapObject_t::iterator iter( m_mapObject.begin() );
			FreeObjectData(  iter->second );
		}
	}

	unsigned int Create( const std::string &tag )
	{
		return AddObjectData( new T, tag );
	}

	object_t* AddObjectData( T* pObj, const std::string &tag );

	bool Destroy( unsigned int id, bool markedForDeletion = false );

	ObjContainer<T>* GetById( unsigned int id );

	ObjContainer<T>* GetFirstByTag( const std::string &tag );

	//NOTE: Create or deleting invalidates iterator
	ObjContainer<T>* GetNext( );
};

template <class T>
unsigned int IObjectManager<T>::GetNextFreeId()
{
	unsigned int savedId = m_IdCnt;
		
	do
	{
		++m_IdCnt;
		m_IdCnt %= m_IdMax;

		if( m_mapObject.find(m_IdCnt) == m_mapObject.end() )
		{
			return m_IdCnt;
		}
	}
	while( m_IdCnt != savedId );

	//this should happen only in case you have more than 2^(size(int)) objects
	return 0;
}

template <class T>
bool IObjectManager<T>::FreeObjectData( object_t* pObjCont )
{

	delete pObjCont->pData;
	pObjCont->pData = 0;
	pObjCont->markedForDeletion = true;

	m_mapObject.erase( pObjCont->id );

	if( m_mapObjectTag[pObjCont->tag].size() > 1 )
	{
		ObjContainer<T>* pObjIter = GetFirstByTag( pObjCont->tag );
		while( pObjIter && (pObjIter != pObjCont) )
		{
			pObjIter = GetNext();
		}

		// fast erase
		std::swap( *m_mapTagIter, m_mapObjectTag[pObjCont->tag].back());
		m_mapObjectTag[pObjCont->tag].pop_back();

		//m_mapTagIter = m_mapObjectTag[pObjCont->tag].erase( m_mapTagIter ); // <<--- this is very slow, but elemet order remains ( not useful in this case )
	}
	else
	{
		m_mapObjectTag.erase( pObjCont->tag );
	}

	m_isMapTagIterValid = false;

	return true;
}

template <class T>
ObjContainer<T>* IObjectManager<T>::AddObjectData( T* pObj, const std::string &tag )
{
	object_t* pObjData = new ObjContainer<T>;
	pObjData->pData = pObj;				
	pObjData->id = GetNextFreeId();
	pObjData->tag = tag;
	pObjData->markedForDeletion = false;

	m_mapObject[m_IdCnt]	= pObjData;

	if( !tag.empty() )
	{
		m_mapObjectTag[tag].push_back( pObjData );
	}

	m_isMapTagIterValid = false;

	return pObjData;
}

template <class T>
bool IObjectManager<T>::Destroy( unsigned int id, bool markedForDeletion = false )
{
	ObjContainer<T>* pObjCont = GetById(id);
		
	if( pObjCont )
	{
		if( markedForDeletion ) 
		{
			// only marked for deletion, FreeObjectData will be called by user implementation
			pObjCont->markedForDeletion = true;
		}
		else
		{
			FreeObjectData( pObjCont );
		}
		return true;
	}
	return false;
}

template <class T>
ObjContainer<T>* IObjectManager<T>::GetById( unsigned int id )
{
	return m_mapObject.find(id) == m_mapObject.end()? 0 : m_mapObject[id];
}

template <class T>
ObjContainer<T>* IObjectManager<T>::GetFirstByTag( const std::string &tag )
{
	if( m_mapObjectTag.find(tag) != m_mapObjectTag.end() )
	{
		m_mapTagIter = m_mapObjectTag[tag].begin(); 
		m_isMapTagIterValid = true; 
		m_iteratedObjTag = tag;
		return (*m_mapTagIter); 
	}

	return 0;
}

//NOTE: Create or deleting invalidates iterator
template <class T>
ObjContainer<T>* IObjectManager<T>::GetNext( )
{
	if( m_isMapTagIterValid )
	{
		++m_mapTagIter;
		if( m_mapTagIter != m_mapObjectTag[m_iteratedObjTag].end() )
		{
			return (*m_mapTagIter);
		}

		m_isMapTagIterValid = false; 
		return 0;
	}

	return 0;
}