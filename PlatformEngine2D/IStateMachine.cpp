#include "IStateMachine.h"


bool IStateMachine::SwitchState( const std::string& stateName )
{
	if ( m_dicStateFunctions.find( stateName ) == m_dicStateFunctions.end())
	{
		ASSERT( false, "State " << stateName.c_str() << " does not exist" );
//return false> key does not exits
		return false;
	}
	else
	{
		m_nextState = stateName;
	}
	return true;
}

void IStateMachine::SendMsg( s32 messageType_ )
{
	SendMsg( messageType_, 0, 0 );
}

void IStateMachine::SendMsg( s32 messageType_, void *pData_, s32 dataSize_ )
{
	// check if state is valid
	if ( m_dicStateFunctions.find( m_currentState ) != m_dicStateFunctions.end() )
	{
		// call state function
		(*m_dicStateFunctions[ m_currentState ])( messageType_, pData_, dataSize_ );

		if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
		{
			m_isFirstUpdate = false;
		}
	}
}

void IStateMachine::Update()
{
	if(
		( m_nextState.length() > 0 ) 
		//&& ( m_nextState.compare( m_currentState ) != 0 )
	)
	{
		m_prevState = m_currentState;
		SendMsg( eCommonMessageTypes::STATE_MESSAGE_EXIT);
		m_currentState = m_nextState;
		SendMsg( eCommonMessageTypes::STATE_MESSAGE_ENTER);
		m_isFirstUpdate = true;
		//SendMsg( eCommonMessageTypes::STATE_MESSAGE_UPDATE); // fix: in attack state, set sword collision flipx is done in update, so is not properly set unless this is call same frame you call enter
	}
	m_nextState = "";
}