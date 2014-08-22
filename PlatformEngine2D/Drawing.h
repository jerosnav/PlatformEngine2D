#pragma once

#include <irrlicht.h>

using namespace irr;

// Bits on the far end of the 32-bit global tile ID are used for tile flags
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

class Drawing
{

public:

	static void draw2DImage(
		irr::video::IVideoDriver *driver, 
		irr::video::ITexture* texture , 
		irr::core::rect<irr::s32> sourceRect,
		irr::core::CMatrix4<irr::f32> transformationMatrix,
		bool useAlphaChannel, 
		irr::video::SColor color,
		irr::u32	flags = 0		// FLIPPED_*
		);
};