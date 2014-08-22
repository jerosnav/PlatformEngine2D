#pragma once

#include <irrlicht.h>

#include "TileMap.h"
#include "LayerTile.h"

class Tilemap;

class TileLayer
{

private:

	std::string m_tag;

	int m_width;
	int m_height;

	irr::core::rect<irr::s32> m_boundRect;

	LayerTile *m_LayerTiles; // array of LayerTile

	Properties *m_properties;

	Tilemap *m_parentTilemap;

public:

	TileLayer(void);
	~TileLayer(void);

	TileLayer( Tilemap *tilemapRef, int width_, int height_, const std::string& sTag_ );

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	LayerTile* GetLayerTile( int tileX, int tileY ) { return &m_LayerTiles[ tileY * m_width + tileX ]; }
	LayerTile* GetLayerTile( int tileIdx ) { return &m_LayerTiles[ tileIdx ]; }
	int GetLayerTileCount() { return m_width * m_height; }
	Tilemap * GetParentTilemap() { return m_parentTilemap; }

	irr::core::recti GetBounds() { return m_boundRect; }

	void AddProperty( const std::string& propertyName, const std::string& value ) { m_properties->AddProperty(propertyName, value); }
	std::string GetProperty( const std::string& propertyName ) { return m_properties->GetProperty(propertyName); }
	std::string GetTag() { return m_tag; }

	void SetLayerTile(int tileIdx_, int tilesetId_, irr::u32 tileId_, irr::u32 flags_);

	//void SetLayerTile(int tileX_, int tileY_, int tilesetId_, irr::u32 tileId_);

	//void ResetLayerTiles();

};

