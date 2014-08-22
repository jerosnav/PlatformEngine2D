#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>

#include "Tilemap.h"
#include "PhyEngine.h"
#include "PhyObject.h"

using namespace irr;

class PhyTileEngine : public PhyEngine
{

private:

	video::SColor Color_Block;
    video::SColor Color_Floating;

	video::IVideoDriver* m_driver;

	Tilemap* m_tilemap;
	TileLayer* m_physicLayerTile;

	virtual void _DoPhysics( PhyObject* phyObject );
	virtual bool _GetIsOnPlatform( PhyObject* phyObject );
	virtual void _ApplyMagneticDistance( PhyObject* phyObject );

	void _DoPhysics_Rect(PhyObject* phyObject);
	void _DoBlockPhysics(PhyObject* phyObject, core::vector2df vDir_ );
	void _DoFloatingPhysics(PhyObject* phyObject, core::vector2df vDir_);
	int PhyTileEngine::_GetFreeVertAreaHeight( PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_);
	int PhyTileEngine::_GetFreeVertAreaHeight( PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_, eDirFlag collFlags_ );
	u32 _GetDistToGround(PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_);
	u32 _GetDistToGround(PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_, eDirFlag collFlags_);
	bool _IsRectBorderCollisionAt(PhyObject* phyObject, core::vector2df vPosition_, video::SColor blockType_);
	bool _IsRectBorderCollisionAt(PhyObject* phyObject, core::vector2df vPosition_, video::SColor blockType_, eDirFlag collFlags_);
	bool _IsRectCollisionAt(core::recti rect_, core::vector2df vPosition_, video::SColor blockType_);
	bool IsRectCollidesPhyTileLayer(core::recti rect_, video::SColor blockType_);
	bool PhyTileEngine::IsRectCollidesTile(core::recti rect_, video::SColor blockType_, int tileX, int tileY);
	u32 _GetCollisionAt(PhyObject* phyObject, core::vector2df vPosition_, core::vector2df vDir_);

public:

	PhyTileEngine( video::IVideoDriver* driver, Tilemap* tilemap, TileLayer* m_physicLayerTile ) : PhyEngine(),
		m_driver( driver ),
		m_tilemap( tilemap ),
		m_physicLayerTile( m_physicLayerTile ),
		Color_Block(255, 255, 0, 0),
		Color_Floating(255, 0, 0, 255)
	{}
};