#pragma once

#include <irrlicht.h>

using namespace irr;

class MathUtil
{
public:
	
	static core::rectf RectI2F( core::recti rect )
	{
		return core::rectf( rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y, rect.LowerRightCorner.X, rect.LowerRightCorner.Y );
	}

	static core::recti RectF2I( core::rectf rect )
	{
		return core::recti( rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y, rect.LowerRightCorner.X, rect.LowerRightCorner.Y );
	}

	static core::position2di PosF2I( core::position2df position )
	{
		return core::position2di( position.X, position.Y );
	}

	static core::position2df PosI2F( core::position2di position )
	{
		return core::position2df( position.X, position.Y );
	}
};