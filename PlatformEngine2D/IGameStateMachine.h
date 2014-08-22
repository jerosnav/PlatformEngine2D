#pragma once

#include "IStateMachine.h"
#include "IGameManager.h"

class IGameStateMachine : public IStateMachine
{
public:

	IGameStateMachine( IGameManager* gameMgr );

	void SendMsg( s32 messageType_ );

private:

	IGameManager* m_gameMgr;
};