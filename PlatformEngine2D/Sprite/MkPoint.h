#pragma once

#include <irrlicht.h>
#include <string>

#include "ISpriteObj.h"

class MkPoint : public ISpriteObj
{
public:

	virtual eSpriteObjType GetType() { return eSpriteObjType::SPR_TYPE_MKPOINT; } 

    irr::core::vector2di Position;

	MkPoint( irr::core::vector2di Position, const std::string& Tag, const std::string& Id ) : ISpriteObj( Id, Tag),
	Position(Position)
	{}
};