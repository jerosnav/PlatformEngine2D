#pragma once

#include <irrlicht.h>
#include <string>
#include <map>

using namespace irr;

enum eCommonMessageTypes
{
	STATE_MESSAGE_ENTER = 0,
	STATE_MESSAGE_EXIT,
	STATE_MESSAGE_UPDATE,
	STATE_MESSAGE_PAINT
};

typedef void (*TStateFP)(s32 messageType_, void *pData_, s32 dataSize_);

class IStateMachine
{
public:

	IStateMachine()
	: m_isFirstUpdate(false)
	{}

	bool SwitchState( const std::string& stateName );

	virtual void SendMsg( s32 messageType_ );	
	virtual void SendMsg( s32 messageType_, void *pData_, s32 dataSize_ );	

	void Update();

	void SetStateFunction( const std::string& stateName, TStateFP stateFunc )
	{
		m_dicStateFunctions[ stateName ] = stateFunc;
	}

	std::string GetCurrentState() { return m_currentState; }
	std::string GetPrevState() { return m_prevState; }

	bool IsValidState( const std::string& stateName ) { return m_dicStateFunctions.find( stateName ) != m_dicStateFunctions.end(); }
	bool IsFirstUpdate() { return m_isFirstUpdate; }

protected:	
	
	std::map<std::string, TStateFP>	m_dicStateFunctions;

	bool		m_isFirstUpdate; // set to true when entering and state until first update is done
	std::string m_prevState;
	std::string m_nextState;
	std::string m_currentState;
};