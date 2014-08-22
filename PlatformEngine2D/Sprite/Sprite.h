#pragma once

#include <irrlicht.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>

#include "ISpriteObj.h"
#include "BitMap.h"
#include "FModule.h"
#include "FMkRect.h"
#include "FMkPoint.h"
#include "Module.h"
#include "MkRect.h"
#include "MkPoint.h"
#include "Frame.h"
#include "Animation.h"

using namespace irr;

enum eSpriteEffects
{
    SPRITE_EFFECT_NONE = 0,
    SPRITE_EFFECT_FLIP_X = 1,
    SPRITE_EFFECT_FLIP_Y = 2,
};

class Sprite
{

private:

    // Tag Dictionary
    std::map<std::string, Module*> m_dicTagModules;
    std::map<std::string, MkRect*> m_dicTagMkRects;
    std::map<std::string, MkPoint*> m_dicTagMkPoints;
    std::map<std::string, Frame*> m_dicTagFrames;
    std::map<std::string, Animation*> m_dicTagAnimations;

    // Id Dictionary
    std::map<std::string, Module*> m_dicModules;
    std::map<std::string, MkRect*> m_dicMkRects;
    std::map<std::string, MkPoint*> m_dicMkPoints;
    std::map<std::string, Frame*> m_dicFrames;
    std::map<std::string, Animation*> m_dicAnims;

    irr::core::vector2df m_vPosition;
    irr::core::vector2df m_vScale;

	irr::video::IVideoDriver *m_driver;

public:
    
	std::string FILE_EXTENSION() { return std::string(".sprite"); }

    std::vector<BitMap*> BitMaps;
    std::vector<Module*> Modules;
    std::vector<MkRect*> MkRects;
    std::vector<MkPoint*> MkPoints;
    std::vector<Frame*> Frames;
    std::vector<Animation*> Animations;

	irr::u32 SpriteEffects; //eSpriteEffects.*


	std::string RootPath;

    std::string AbsoluteBitmapFilePath( int imgIdx ) { return RootPath+BitMaps[imgIdx]->BitmapFilePath; }

	irr::core::vector2di Origin;
	irr::f32 Rotation;
    irr::core::vector2df Scale;
    irr::f32 Depth;
    irr::video::SColor TintColor;

    // Key Generators
    static irr::u32 m_aKeyGenerator[];	

    Sprite( irr::video::IVideoDriver *driver ) :
		m_driver( driver ),
		RootPath(".\\"),
		SpriteEffects(0),
		TintColor( 255, 255, 255, 255),
		m_vPosition(0, 0),
		Rotation(0),
		Depth(0),
		m_vScale(0)
    { }

	~Sprite()
	{
		for( std::vector<BitMap*>::iterator iter( BitMaps.begin() ); iter != BitMaps.end(); ++iter )			delete (*iter);
		for( std::vector<Module*>::iterator iter( Modules.begin() ); iter != Modules.end(); ++iter )			delete (*iter);
		for( std::vector<MkRect*>::iterator iter( MkRects.begin() ); iter != MkRects.end(); ++iter )			delete (*iter);
		for( std::vector<MkPoint*>::iterator iter( MkPoints.begin() ); iter != MkPoints.end(); ++iter )			delete (*iter);
		for( std::vector<Frame*>::iterator iter( Frames.begin() ); iter != Frames.end(); ++iter )				delete (*iter);
		for( std::vector<Animation*>::iterator iter( Animations.begin() ); iter != Animations.end(); ++iter )	delete (*iter);
	}

    void DrawFrameByTag( const std::string& sFrameTag_, irr::core::vector2df offset_)
    {
        DrawFrame(m_dicTagFrames[sFrameTag_], offset_);
    }

    void DrawFrameById( const std::string& sFrameId_, irr::core::vector2df offset_)
    {
        DrawFrame(m_dicFrames[sFrameId_], offset_);
    }

    void DrawFrameByIdx(int iFrameId_, irr::core::vector2df offset_)
    {
        DrawFrame(Frames[iFrameId_], offset_);
    }

    void DrawAFrame(Animation *anim_, int iAframeIdx_, irr::core::vector2df offset_)
    {
        Frame *objFrame = m_dicFrames[anim_->AFrames[iAframeIdx_]->Id];

        // Set anim origin
        Origin = -anim_->AFrames[iAframeIdx_]->Offset;

        DrawFrame(objFrame, offset_);
    }

    void DrawFrame(Frame *frame_, irr::core::vector2df position_);

    Animation *GetAnimByTag( const std::string& sAnimTag_)
    {
		ASSERT( m_dicTagAnimations.find( sAnimTag_ ) != m_dicTagAnimations.end(), " animation with tag " << sAnimTag_.c_str() << " not found" );
        return m_dicTagAnimations[sAnimTag_];
    }

	bool IsModuleExist( const std::string& id_) { return m_dicModules.find(id_) != m_dicModules.end(); }
    bool IsMkRectExist( const std::string& id_) { return m_dicMkRects.find(id_) != m_dicMkRects.end(); }
    bool IsMkPointExist( const std::string& id_) { return m_dicMkPoints.find(id_) != m_dicMkPoints.end(); }
    bool IsFrameExist( const std::string& id_) { return m_dicFrames.find(id_) != m_dicFrames.end(); }
    bool IsAnimExist( const std::string& id_) { return m_dicAnims.find(id_) != m_dicAnims.end(); }

    Module* GetModule( const std::string& id_) { return m_dicModules.find(id_) != m_dicModules.end()? m_dicModules[id_] : 0; }
    MkRect* GetMkRect( const std::string& id_) { return m_dicMkRects.find(id_) != m_dicMkRects.end() ? m_dicMkRects[id_] : 0; }
    MkPoint* GetMkPoint( const std::string& id_) { return m_dicMkPoints.find(id_) != m_dicMkPoints.end() ? m_dicMkPoints[id_] : 0; }
    Frame* GetFrame( const std::string& id_) { return m_dicFrames.find(id_) != m_dicFrames.end() ? m_dicFrames[id_] : 0; }
    Animation* GetAnim( const std::string& id_) { return m_dicAnims.find(id_) != m_dicAnims.end() ? m_dicAnims[id_] : 0; }

	Module* GetModuleByTag( const std::string& tag_) { return m_dicTagModules.find(tag_) != m_dicTagModules.end()? m_dicTagModules[tag_] : 0; }

    void SetScale(float x_, float y_)
    {
        m_vScale.X = x_;
        m_vScale.Y = y_;
    }

    void SetPosition(float x_, float y_)
    {
        m_vPosition.X = x_;
        m_vPosition.Y = y_;
    }

    // Use this methods to configure a sprite and save as xml. Then configure using xml file
	std::string AddModule( const std::string& sTag_, core::recti rect_, int imgIdx_, const std::string& Id)
    {
        Module *objModule = new Module( rect_, imgIdx_, sTag_, Id );

        return _AddSpriteObject<Module>(objModule, Modules, m_dicModules, m_dicTagModules);
    }

	std::string AddModule( const std::string& sTag_, core::recti rect_, int imgIdx_ )
	{
		return AddModule( sTag_, rect_, imgIdx_, GenerateId(eSpriteObjType::SPR_TYPE_MODULE));
	}

    std::string AddMkRect( const std::string& sTag_, core::recti rect_, const std::string& Id )
    {
        MkRect *objMkRect = new MkRect( rect_, sTag_, Id );

        return _AddSpriteObject<MkRect>(objMkRect, MkRects, m_dicMkRects, m_dicTagMkRects); 
    }

	std::string AddMkRect( const std::string& sTag_, core::recti rect_ )
	{
		return AddMkRect( sTag_, rect_, GenerateId(eSpriteObjType::SPR_TYPE_MKRECT ) );
	}

    std::string AddMkPoint( const std::string& sTag_, irr::core::vector2di position_, const std::string& Id)
    {
        MkPoint *objMkPoint = new MkPoint( position_, sTag_, Id );

        return _AddSpriteObject<MkPoint>(objMkPoint, MkPoints, m_dicMkPoints, m_dicTagMkPoints); 
    }

	std::string AddMkPoint( const std::string& sTag_, irr::core::vector2di position_ )
	{
		return AddMkPoint( sTag_, position_, GenerateId(eSpriteObjType::SPR_TYPE_MKPOINT ) );
	}

	std::string AddFrame( Frame* frame)
	{
		return _AddSpriteObject<Frame>(frame, Frames, m_dicFrames, m_dicTagFrames);
	}

    std::string AddFrame( const std::string& sTag_, const std::string& Id)
    {
        Frame* objFrame = new Frame( sTag_, Id );
        return AddFrame( objFrame );
    }

	std::string AddFrame( const std::string& sTag_ )
	{
		return AddFrame( sTag_, GenerateId(eSpriteObjType::SPR_TYPE_FRAME ) );
	}

	std::string AddAnimation( Animation *objAnimation )
	{
		return _AddSpriteObject<Animation>(objAnimation, Animations, m_dicAnims, m_dicTagAnimations);
	}

    std::string AddAnimation( const std::string& sTag_, const std::string& Id)
    {
        Animation *objAnimation = new Animation( sTag_, Id );
        return AddAnimation(objAnimation);
    }

	std::string AddAnimation( const std::string& sTag_)
	{
		return AddAnimation( sTag_, GenerateId(eSpriteObjType::SPR_TYPE_ANIMATION ) );
	}

    void RemoveModule( const std::string& id_)
    {
        Module* objModule = m_dicModules[id_];

        _RemoveSpriteObject<Module>(objModule, Modules, m_dicModules, m_dicTagModules);
    }

    void RemoveMkRect( const std::string& id_)
    {
        MkRect* objMkRect = m_dicMkRects[id_];

        _RemoveSpriteObject<MkRect>(objMkRect, MkRects, m_dicMkRects, m_dicTagMkRects);
    }

    void RemoveMkPoint( const std::string& id_)
    {
        MkPoint* objMkPoint = m_dicMkPoints[id_];

        _RemoveSpriteObject<MkPoint>(objMkPoint, MkPoints, m_dicMkPoints, m_dicTagMkPoints);
    }

    void RemoveFrame( const std::string& id_)
    {
        Frame* objFrame = m_dicFrames[id_];

        _RemoveSpriteObject<Frame>(objFrame, Frames, m_dicFrames, m_dicTagFrames);
    }

    void RemoveAnimation( const std::string& id_)
    {
        Animation* objAnim = m_dicAnims[id_];

        _RemoveSpriteObject<Animation>(objAnim, Animations, m_dicAnims, m_dicTagAnimations);
    }

    void RemoveSpriteObj(ISpriteObj *objItem_)
    {
        switch (objItem_->GetType())
        {
			case eSpriteObjType::SPR_TYPE_MODULE:	RemoveModule(objItem_->Id); break;
			case eSpriteObjType::SPR_TYPE_MKRECT:	RemoveMkRect(objItem_->Id); break;
			case eSpriteObjType::SPR_TYPE_MKPOINT:	RemoveMkPoint(objItem_->Id); break;
			case eSpriteObjType::SPR_TYPE_FRAME:	RemoveFrame(objItem_->Id); break;
			case eSpriteObjType::SPR_TYPE_ANIMATION:RemoveAnimation(objItem_->Id); break;
			default: printf(" Unknown sprite object of type %s" + objItem_->GetType()); break;
        }
    }


    bool SaveToFile( const std::string& sFilePath_);

    static Sprite *LoadFromFile( const std::string& sFilePath_, irr::video::IVideoDriver *driver_);

private:

	template <class T>
	std::string _AddSpriteObject(T *obj_, std::vector<T *>& iList_, std::map<std::string, T*>& iDicId_, std::map<std::string, T*>& iDicTag_)
    {
        // Add to list only if not exist already
        if (std::find(iList_.begin(), iList_.end(), obj_) == iList_.end())
        {
			iList_.push_back(obj_);

			iDicId_[obj_->Id] = obj_;

			if ( obj_->Tag.length() > 0)
			{
				iDicTag_[ obj_->Tag ] = obj_;
			}
        }

        return obj_->Id;
    }

	template <class T>
    void _RemoveSpriteObject(T *obj_, std::vector<T *> iList_, std::map<std::string, T*> iDicId_, std::map<std::string, T*> iDicTag_)
    {
		iList_.erase(std::remove(iList_.begin(), iList_.end(), obj_), iList_.end());
		iDicId_.erase( obj_->Id );
        iDicTag_.erase( obj_->Tag );

		delete obj_;
    }

	std::string GenerateId( eSpriteObjType objType_ )
    {
        std::string sId;

        switch (objType_)
        {
			case eSpriteObjType::SPR_TYPE_MODULE:		return GenerateId<Module>(objType_, m_dicModules);
			case eSpriteObjType::SPR_TYPE_MKRECT:		return GenerateId<MkRect>(objType_, m_dicMkRects);
			case eSpriteObjType::SPR_TYPE_MKPOINT:		return GenerateId<MkPoint>(objType_, m_dicMkPoints);
			case eSpriteObjType::SPR_TYPE_FRAME:		return GenerateId<Frame>(objType_, m_dicFrames);
			case eSpriteObjType::SPR_TYPE_ANIMATION:	return GenerateId<Animation>(objType_, m_dicAnims);
        }

        return sId;
    }

	template<class T>
	std::string GenerateId( eSpriteObjType objType_, std::map<std::string, T*> iDic )
    {
        std::string sId;

        do
        {
			std::stringstream stream;
			stream << "0x" << std::setfill ('0') << std::setw(sizeof(irr::u32)*2) << std::hex << m_aKeyGenerator[(int)objType_];
            sId = stream.str();

            m_aKeyGenerator[objType_]++;
        }
        while ( iDic.find(sId) != iDic.end() );

        return sId;
    }

public:
	irr::core::vector2di GetFMkPointData( const std::string& frameId_, int iFMkPointIdx_, irr::u32 spriteEffects_)
    {
        Frame* objFrame = GetFrame(frameId_);

        if (iFMkPointIdx_ >= objFrame->MkPoints.size())
        {
			printf(" <GetFMkPointData> error iFMkPointIdx_ out of bounds "+iFMkPointIdx_);
			return irr::core::vector2di();
            //return 0;
        }
        else
        {
            FMkPoint* objFMkPoint = objFrame->MkPoints[iFMkPointIdx_];

            return GetMkPointData(objFMkPoint->Id, spriteEffects_, objFMkPoint->Offset);
        }
    }
    irr::core::vector2di GetFMkPointData( const std::string& frameId_, int iFMkPointIdx_) { return GetFMkPointData(frameId_, iFMkPointIdx_, SpriteEffects); }

    irr::core::vector2di GetMkPointData( const std::string& mkPointId_, irr::u32 spriteEffects_, irr::core::vector2di offset_)
    {
		irr::core::vector2di pos_ret = GetMkPoint(mkPointId_)->Position + offset_;

        if (spriteEffects_ & eSpriteEffects::SPRITE_EFFECT_FLIP_X) { pos_ret.X = -pos_ret.X; }
        if (spriteEffects_ & eSpriteEffects::SPRITE_EFFECT_FLIP_Y) { pos_ret.Y = -pos_ret.Y; }

        return pos_ret;
    }
    irr::core::vector2di GetMkPointData( const std::string& mkPointId_, irr::u32 spriteEffects_) { return GetMkPointData(mkPointId_, spriteEffects_, irr::core::vector2di(0, 0)); }
    irr::core::vector2di GetMkPointData( const std::string& mkPointId_) { return GetMkPointData(mkPointId_, SpriteEffects); }

	irr::core::vector2di GetMkPointDataByTag( const std::string& sTag_, irr::u32 spriteEffects_)
    {
        return GetMkPointData(m_dicTagMkPoints[sTag_]->Id, spriteEffects_);
    }
    irr::core::vector2di GetMkPointDataByTag( const std::string& sTag_) { return GetMkPointDataByTag(sTag_, SpriteEffects); }

	irr::core::recti GetFMkRectData( const std::string& frameId_, int iFMkRectIdx_, irr::u32 spriteEffects_)
    {
        Frame* objFrame = GetFrame(frameId_);

        if (iFMkRectIdx_ >= objFrame->MkRects.size())
        {
           	printf(" <GetFMkRectData> error iFMkPointIdx_ out of bounds "+iFMkRectIdx_);
			return irr::core::recti(0, 0, 0, 0);
            //return 0;
        }
        else
        {
            FMkRect* objFMkRect = objFrame->MkRects[iFMkRectIdx_];

            return GetMkRectData(objFMkRect->Id, spriteEffects_, objFMkRect->Offset);
        }
    }
    irr::core::recti GetFMkRectData( const std::string& frameId_, int iFMkRectIdx_) { return GetFMkRectData(frameId_, iFMkRectIdx_, SpriteEffects); }

    irr::core::recti GetMkRectData( const std::string& mkRectId_, irr::u32 spriteEffects_, irr::core::vector2di offset_)
    {
        irr::core::recti rect_ret = GetMkRect(mkRectId_)->Rect;

		rect_ret.LowerRightCorner += offset_;
		rect_ret.UpperLeftCorner += offset_;

		if (spriteEffects_ & eSpriteEffects::SPRITE_EFFECT_FLIP_X ) 
		{ 
 			rect_ret.UpperLeftCorner.X	= -rect_ret.UpperLeftCorner.X;
			rect_ret.LowerRightCorner.X = -rect_ret.LowerRightCorner.X;
		}
		if (spriteEffects_ & eSpriteEffects::SPRITE_EFFECT_FLIP_Y) 
		{ 
			rect_ret.UpperLeftCorner.Y	= -rect_ret.UpperLeftCorner.Y;
			rect_ret.LowerRightCorner.Y = -rect_ret.LowerRightCorner.Y;
		}

		// Repair rect instead swap values
		rect_ret.repair();

        return rect_ret;
    }
	irr::core::recti GetMkRectData( const std::string& mkRectId_, irr::u32 spriteEffects_) { return GetMkRectData(mkRectId_, spriteEffects_, irr::core::vector2di(0, 0) ); }
    irr::core::recti GetMkRectData( const std::string& mkRectId_) { return GetMkRectData(mkRectId_, SpriteEffects); }

    /// <summary>
    /// Return a rectangle from a MkRect
    /// </summary>
    /// <param name="sTag_"></param>
    /// <returns></returns>
	irr::core::recti GetMkRectDataByTag( const std::string& sTag_, irr::u32 spriteEffects_)
    {
        return GetMkRectData(m_dicTagMkRects[sTag_]->Id, spriteEffects_);
    }
    irr::core::recti GetMkRectDataByTag( const std::string& sTag_) { return GetMkRectDataByTag(sTag_, SpriteEffects); }

};