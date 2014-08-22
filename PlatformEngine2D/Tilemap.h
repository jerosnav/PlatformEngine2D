#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>

#include "Tileset.h"
#include "TileLayer.h"

class TileLayer;

class Tilemap
{

private:

	std::vector<Tileset*> m_aTilesets;
	std::map< int , Tileset* > m_dicTilesets;
	std::map< std::string , int > m_dicTilesetsTag;

	std::map< std::string , TileLayer* > m_dicTileLayers;
	std::vector< TileLayer* > m_aTileLayers;

	int m_tileWidth;
	int m_tileHeight;

	int m_mapWidth;
	int m_mapHeight;

	int _m_tilesetIdCounter;

	std::string m_sRootPath;

public:
	Tilemap(void);
	~Tilemap(void);

	Tilemap(int iMapWidth, int iMapHeight, int iTileWidth_, int iTileHeight_);

	void Draw( irr::video::IVideoDriver* driver );
	void DrawTileLayer( irr::video::IVideoDriver* driver, const std::string& tileLayerName_ );
	void DrawTileLayer( irr::video::IVideoDriver* driver, int tileLayerIdx_ );
	void DrawTileLayer( irr::video::IVideoDriver* driver, const std::string& tileLayerName_, int tileStartX, int tileStartY, int tileEndX, int tileEndY  );
	void DrawTileLayer( irr::video::IVideoDriver* driver, int tileLayerIdx_, int tileStartX, int tileStartY, int tileEndX, int tileEndY  );

	int AddTileset( const std::string& sTextureAsset_, int iWidth_, int iHeight_, int iTileWidth_, int iTileHeight_, const std::string& tag_ = "");
	int AddTileLayer(int width_, int height_, const std::string& sTag_);
	void SetTileset(int tilesetId_, Tileset *tileset_);
	Tileset *GetTileset(int tilesetId_);
	int GetTilesetId( const std::string& tilesetTag_);
	int GetTilesetCounter() { return m_aTilesets.size(); }
	Tileset* GetTilesetByIdx( int idx ) { return (idx >= 0 && idx < GetTilesetCounter())?  m_aTilesets[idx] : 0; }
	TileLayer *GetTileLayer(int tileLayerIdx_);
	TileLayer *GetTileLayer( const std::string& sTag ) { return m_dicTileLayers[sTag]; }
	int GetTileLayerCount() { return m_aTileLayers.size(); }
	int GetTileWidth();
	int GetTileHeight();
	int GetMapWidth();
	int GetMapHeight();

	const std::string& GetRootPath() { return m_sRootPath; }


	static Tilemap* LoadFromFile( const std::string& sFileName );
};

