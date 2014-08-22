#include "EntitySlime.h"
#include "GameManager.h"
#include "EntityCollisionFlags.h"

void EntitySlime::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	if(
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY ) &&
		( otherCollBody->Owner == otherCollBody->Owner->GetGameManager()->GetEntityManager()->GetEntityByTag("player") ) && // collides with player
		( thisCollBody->Owner->GetStateMachine()->GetCurrentState().compare("Dying") )
	)
	{
		bool isPlayerLeft = thisCollBody->Owner->GetPhysicObject()->Position.X > otherCollBody->Owner->GetPhysicObject()->Position.X;
		((GameManager *)thisCollBody->Owner->GetGameManager())->ApplyDamage(0, otherCollBody->Owner, 5, 0, isPlayerLeft? eDirFlag::LEFT : eDirFlag::RIGHT );
	}
}