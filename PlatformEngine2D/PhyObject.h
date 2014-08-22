#pragma once

#include <irrlicht.h>
#include <vector>

using namespace irr;

enum ePhyObjectType
{
	PHY_OBJECT_TYPE_RECT,
	PHY_OBJECT_TYPE_DOT,
	PHY_OBJECT_TYPE_LINE
};

enum eDirFlag
{
	NONE	= 0x0,
	TOP		= 0x1,
	RIGHT	= 0x2,
	BOTTOM	= 0x4,
	LEFT	= 0x8,
	ALL		= 0xf
};

class PhyObject
{
private:

	ePhyObjectType m_type;

public:

	core::recti BoundingBox;
	core::vector2df PrevPosition;
	core::vector2df Position;
	core::vector2df Velocity;
	core::vector2df Acceleration;
	core::vector2df MaxVelocity;
	bool IsApplyGravity;
	s32 GndMagneticOffset;
	u32 GndSlopeHigh;
	u32 PhyCollisionFlags;
	irr::f32 Mass;
	bool IsOnPlatform;
	bool IsClipping;
	bool SkipFloatingPlatformCheck; // skip during one update check floating block collision to allow entity go through

	PhyObject() :
		m_type(ePhyObjectType::PHY_OBJECT_TYPE_RECT),
		BoundingBox(),
		Position(0, 0),
		Velocity(0, 0),
		Acceleration(0, 0),
		MaxVelocity(-1, -1),
		IsApplyGravity(true),
		GndMagneticOffset(10),
		GndSlopeHigh(5),
		PhyCollisionFlags(0),
		Mass(1),
		IsOnPlatform(false),
		IsClipping(false),
		SkipFloatingPlatformCheck(false)
	{}

	virtual ~PhyObject()
	{
		//DBG(" PhyObject Deleted ");
	}

	void SetType( ePhyObjectType type ){ m_type = type; }
	ePhyObjectType GetType( ){ return m_type; }

	void ApplyAcceleration( core::vector2df acc )
	{
		Acceleration += acc;
	}
};