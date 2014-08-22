#pragma once

#include <vector>
#include <string>

#include "ISpriteObj.h"
#include "FModule.h"
#include "FMkRect.h"
#include "FMkPoint.h"

class Frame : public ISpriteObj
{

public:
    
	virtual eSpriteObjType GetType() { return eSpriteObjType::SPR_TYPE_FRAME; } 

	std::vector <FModule*> Modules;
    std::vector <FMkRect*> MkRects;
    std::vector <FMkPoint*> MkPoints;

    // ! Future feature allow cache frame to avoid artifacts when applying alpha effect and two modules are intersecting, for example
    //public Texture2D CachedFrameTex2D { get; set; }

	Frame( const std::string& Tag, const std::string& Id ) : ISpriteObj(Id, Tag),
		Modules(),
        MkRects(),
		MkPoints()
	{}

	~Frame()
	{
		for( std::vector<FModule*>::iterator iter( Modules.begin() ); iter != Modules.end(); ++iter )			delete (*iter);
		for( std::vector<FMkRect*>::iterator iter( MkRects.begin() ); iter != MkRects.end(); ++iter )			delete (*iter);
		for( std::vector<FMkPoint*>::iterator iter( MkPoints.begin() ); iter != MkPoints.end(); ++iter )			delete (*iter);
	}
};
