#pragma once

#include <irrlicht.h>
#include <string>

class AFrame
{
public:
    std::string Id;
    irr::core::vector2di Offset;
    irr::u32 TimeMs;

	AFrame() {}
	AFrame( const std::string& Id, irr::core::vector2di Offset, int TimeMs ) :
	Id(Id), Offset(Offset), TimeMs(TimeMs){}
};
