#include "IGameManager.h"

void IGameManager::SetLevel ( Level* level )
{ 
	if( m_level ) delete m_level; 
	m_level = level; 
}

void IGameManager::SetPhysicEngine( PhyEngine* phyEngine )
{ 
	if( m_phyEngine ) delete m_phyEngine; 
	m_phyEngine = phyEngine; 
}