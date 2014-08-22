#pragma once

#include <irrlicht.h>
#include <string>

class FModule
{
public:
	std::string Id;
	irr::core::vector2di Offset;
	irr::u32 Flags;
};
