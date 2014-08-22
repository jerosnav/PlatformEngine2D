#include "EntityMushSmall.h"
#include "GameManager.h"
#include "EntityCollisionFlags.h"

EntityMushSmall::EntityMushSmall(void)
{
}


EntityMushSmall::~EntityMushSmall(void)
{
}

void EntityMushSmall::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	if(
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY ) &&
		( otherCollBody->Owner == otherCollBody->Owner->GetGameManager()->GetEntityManager()->GetEntityByTag("player") ) && // collides with player
		( thisCollBody->Owner->GetStateMachine()->GetCurrentState().compare("Walking") == 0 )
	)
	{
		bool isPlayerLeft = thisCollBody->Owner->GetPhysicObject()->Position.X > otherCollBody->Owner->GetPhysicObject()->Position.X;
		((GameManager *)thisCollBody->Owner->GetGameManager())->ApplyDamage(0, otherCollBody->Owner, 5, 0, isPlayerLeft? eDirFlag::LEFT : eDirFlag::RIGHT );
	}
}