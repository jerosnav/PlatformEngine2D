#pragma once

#include <irrlicht.h>
#include <string>

#include "ISpriteObj.h"

class MkRect : public ISpriteObj
{
public:
	virtual eSpriteObjType GetType() { return eSpriteObjType::SPR_TYPE_MKRECT; } 

	irr::core::recti Rect;

	MkRect( irr::core::recti Rect, const std::string& Tag, const std::string& Id ) : ISpriteObj(Id, Tag),
		Rect(Rect)
	{}
};