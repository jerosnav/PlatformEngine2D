#pragma once

#include <irrlicht.h>
#include <string>
#include <map>

#include "Properties.h"

class Tileset
{

private:

	std::string m_sTextureAsset;

	irr::video::ITexture* m_texture;

	int m_tileWidth;
	int m_tileHeight;

	int m_width;
	int m_height;


	// UInt32: should be tile index: most significant half for x position, less significant half for y position
	std::map< irr::u32, Properties * > m_dicTileProperties;

	// Mapped tiles ( use a different tileid when painting, used for animations )
	std::map< irr::u32, irr::u32 > m_dicTileMapping;


public:
	Tileset(void);
	~Tileset(void);

	Tileset( const std::string& sTextureAsset_, int iWidth_, int iHeight_, int iTileWidth_, int iTileHeight_);

	std::string GetTextureAsset() { return m_sTextureAsset; }

	irr::video::ITexture * GetTexture( irr::video::IVideoDriver* driver );

	int GetTileWidth() { return m_tileWidth; }
	int GetTileHeight() { return m_tileHeight; }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	void AddTileProperty( int tileIdx_, const std::string& sPropertyName_, const std::string& value_ );
	void AddTileProperty( int tileX_, int tileY_, const std::string& sPropertyName_, const std::string& value_ );

	std::string GetTilePropertyById( irr::u32 tileId_, const std::string& sPropertyName_ ) { return m_dicTileProperties[tileId_] ? m_dicTileProperties[tileId_]->GetProperty( sPropertyName_ ) : ""; }
	Properties * GetTilePropertiesById( irr::u32 tileId_ ) { return m_dicTileProperties[tileId_] ? m_dicTileProperties[tileId_] : 0; }
	std::string GetTileProperty( int tileIdx_, const std::string& sPropertyName_ );
	std::string GetTileProperty( int tileX_, int tileY_, const std::string& sPropertyName_ );

	irr::u32 GetTileMapping( irr::u32 tileId_ ){ return ( m_dicTileMapping.find(tileId_) != m_dicTileMapping.end() )? m_dicTileMapping[tileId_] : -1; }
	void SetTileMapping( irr::u32 tileId_, irr::u32 tileMapId_ ){ m_dicTileMapping[tileId_] = tileMapId_; }

	irr::u32 GetTileId(int tileIdx_);
	irr::u32 GetTileId(int tileX_, int tileY_);

	int GetTileX(irr::u32 tileId_);

	int GetTileY(irr::u32 tileId_);

};

