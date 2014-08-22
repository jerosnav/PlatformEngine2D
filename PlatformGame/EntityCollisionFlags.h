#pragma once

// Collision flags used for managing entity collisions
enum eEntityCollFlag
{
	ENTITY_COLLISION_FLAG_NONE		= 0,
	ENTITY_COLLISION_FLAG_COLLBODY	= ( 1 << 1 ),
	ENTITY_COLLISION_FLAG_SHIELD	= ( 1 << 2 ),
	ENTITY_COLLISION_FLAG_SWORD		= ( 1 << 3 )
};