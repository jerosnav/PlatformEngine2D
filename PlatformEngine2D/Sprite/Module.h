#pragma once

#include <irrlicht.h>
#include <string>

#include "ISpriteObj.h"

class Module : public ISpriteObj
{
public:

	virtual eSpriteObjType GetType() { return eSpriteObjType::SPR_TYPE_MODULE; } 

    irr::core::recti Rect;
    irr::u32 ImgIdx;

	Module( irr::core::recti Rect, int ImgIdx, const std::string& Tag, const std::string& Id ) : ISpriteObj(Id, Tag),
	Rect(Rect), ImgIdx(ImgIdx)
	{}

};