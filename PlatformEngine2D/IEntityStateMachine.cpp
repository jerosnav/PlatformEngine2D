#include "IEntityStateMachine.h"

IEntityStateMachine::IEntityStateMachine( EntityNode* entityNode ) 
	: IStateMachine()
	, m_entityNode( entityNode )
	{}

void IEntityStateMachine::SendMsg( s32 messageType_ )
{
	IStateMachine::SendMsg( messageType_, m_entityNode, sizeof( EntityNode* ) );
}