#include "EntityRock.h"

#include "EntityCollisionFlags.h"


void EntityRock::Collision_Body( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody )
{
	//DBG(" <EntityFireBall::Collision_Body> Collision with entity ");
	if( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_SHIELD )
	{
		//DBG(" <EntityFireBall::Collision_Body> Destroy by shield ");
		
		// Play sound
		thisCollBody->Owner->GetGameManager()->GetSoundMgr()->PlaySound("sounds/ShieldBlock00.wav");

		EntityRock::DoDestroyRock( thisCollBody->Owner );
		//thisCollBody->Owner->GetPhysicObject()->Velocity.X = 0;
	}
	else if(
		( otherCollBody->Flags & eEntityCollFlag::ENTITY_COLLISION_FLAG_COLLBODY ) &&
		( otherCollBody->Owner == otherCollBody->Owner->GetGameManager()->GetEntityManager()->GetEntityByTag("player") ) // collides with player
	)
	{
		bool isPlayerLeft = thisCollBody->Owner->GetPhysicObject()->Position.X > otherCollBody->Owner->GetPhysicObject()->Position.X;
		((GameManager *)thisCollBody->Owner->GetGameManager())->ApplyDamage(0, otherCollBody->Owner, 5, 0, isPlayerLeft? eDirFlag::LEFT : eDirFlag::RIGHT );
	}
}

void EntityRock::DoDestroyRock( EntityNode* entityNode )
{
	EntityNode* visualFx = entityNode->GetGameManager()->GetEntityFactory()->CreateEntityFromTemplate("visualFx", entityNode->getParent()->getName(), "", 0);
	visualFx->GetPhysicObject()->Position = entityNode->GetPhysicObject()->Position;
	visualFx->GetPhysicObject()->Velocity.Y = -50;
	visualFx->GetPhysicObject()->IsApplyGravity = true;
	visualFx->SetSpriteComponent("sprites\\effect01.sprite");
	visualFx->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("rock_explosion", 1);
	visualFx->setScale( core::vector3df(0.4));

	entityNode->DestroyEntity();
}