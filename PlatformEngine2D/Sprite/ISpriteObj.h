#pragma once

#include <string>


enum eSpriteObjType
{
    SPR_TYPE_MODULE,
    SPR_TYPE_MKRECT,
    SPR_TYPE_MKPOINT,
    SPR_TYPE_FRAME,
    SPR_TYPE_ANIMATION
};

class ISpriteObj
{
public:

    std::string Id;
    std::string Tag;

    virtual eSpriteObjType GetType() = 0;

	ISpriteObj( const std::string& Id, const std::string& Tag ) : Id(Id), Tag(Tag){}
};
