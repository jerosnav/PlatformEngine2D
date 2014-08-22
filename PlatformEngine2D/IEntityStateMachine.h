#pragma once

#include "IStateMachine.h"
#include "EntityNode.h"

class EntityNode;

class IEntityStateMachine : public IStateMachine
{
public:

	IEntityStateMachine( EntityNode* entityNode );

	void SendMsg( s32 messageType_ );

private:

	EntityNode* m_entityNode;
};