#include <irrlicht.h>
#include "GameManager.h"
#include "IEntityFactory.h"
#include "EntityNode.h"

using namespace irr;

class EntityUtils
{
public:

	// suppose all sprites look right when no flipX
	static bool IsEntityFacingOther( EntityNode* entitySrc, EntityNode* entityDst )
	{
		return( entitySrc->GetSpriteComponent()->GetAnimPlayer()->SpriteEffects & SPRITE_EFFECT_FLIP_X ) ? 
			( entitySrc->GetPhysicObject()->Position.X > entityDst->GetPhysicObject()->Position.X ) :
			( entitySrc->GetPhysicObject()->Position.X <= entityDst->GetPhysicObject()->Position.X );
	}

	static f32 GetDistanceBetweenEntities( EntityNode* entitySrc, EntityNode* entityDst )
	{
		return entitySrc->GetPhysicObject()->Position.getDistanceFrom( entityDst->GetPhysicObject()->Position );
	}
};