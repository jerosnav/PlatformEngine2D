#include "IGameStateMachine.h"

IGameStateMachine::IGameStateMachine( IGameManager* gameMgr ) 
	: IStateMachine()
	, m_gameMgr( gameMgr )
{}

void IGameStateMachine::SendMsg( s32 messageType_ )
{
	IStateMachine::SendMsg( messageType_, m_gameMgr, sizeof( IGameManager* ) );
}