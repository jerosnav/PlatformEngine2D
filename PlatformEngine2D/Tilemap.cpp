#include "Tilemap.h"

#include "Drawing.h"
#include "tinyxml2\tinyxml2.h"

#include "..\TextUtil.h"

using namespace tinyxml2;
using namespace irr;

Tilemap::Tilemap(void)
{
}


Tilemap::~Tilemap(void)
{
	// Remove Tilesets
	{
		std::vector<Tileset*>::iterator iter(m_aTilesets.begin());
		std::vector<Tileset*>::iterator iter_end(m_aTilesets.end());
		while( iter != iter_end )
		{
			delete *iter;
			++iter;
		}
	}

	// Remove Tilelayers
	{
		std::vector<TileLayer*>::iterator iter(m_aTileLayers.begin());
		std::vector<TileLayer*>::iterator iter_end(m_aTileLayers.end());
		while( iter != iter_end )
		{
			delete *iter;
			++iter;
		}
	}
}


Tilemap::Tilemap(int iMapWidth, int iMapHeight, int iTileWidth_, int iTileHeight_) :
	m_tileWidth(iTileWidth_),
	m_tileHeight(iTileHeight_),
	m_mapWidth(iMapWidth),
	m_mapHeight(iMapHeight),
	m_dicTilesets(),
	m_aTileLayers(),
	_m_tilesetIdCounter(0)
{
}

int Tilemap::AddTileset( const std::string& sTextureAsset_, int iWidth_, int iHeight_, int iTileWidth_, int iTileHeight_, const std::string& tag_)
{
	// while key exists
	while (m_dicTilesets.find(_m_tilesetIdCounter) != m_dicTilesets.end())
    {
        _m_tilesetIdCounter++;
		_m_tilesetIdCounter %= 0x7FFFFFFF;
    }

	Tileset * tileset = new Tileset(sTextureAsset_, iWidth_, iHeight_, iTileWidth_, iTileHeight_);
	m_aTilesets.push_back(tileset);
    m_dicTilesets[ _m_tilesetIdCounter ] = tileset;
	m_dicTilesetsTag[ tag_ ] = _m_tilesetIdCounter;

    return _m_tilesetIdCounter;
}
	
int Tilemap::AddTileLayer(int width_, int height_, const std::string& sTag_)
{
	TileLayer* tileLayer = new TileLayer(this, width_, height_, sTag_);
	m_aTileLayers.push_back( tileLayer );
	m_dicTileLayers[sTag_] = tileLayer;
	return m_aTileLayers.size() - 1;
}
	
void Tilemap::SetTileset(int tilesetId_, Tileset *tileset_)
{
	// remove previous value
	if (m_dicTilesets.find(tilesetId_) != m_dicTilesets.end())
	{
		delete m_dicTilesets[tilesetId_];
	}

	m_dicTilesets[tilesetId_] = tileset_;
}

Tileset *Tilemap::GetTileset(int tilesetId_)
{
	return m_dicTilesets[tilesetId_];
}

int Tilemap::GetTilesetId( const std::string& tilesetTag_)
{
	return m_dicTilesetsTag.find(tilesetTag_) != m_dicTilesetsTag.end() ?  m_dicTilesetsTag[tilesetTag_] : -1;
}

TileLayer *Tilemap::GetTileLayer(int tileLayerIdx_)
{
	return m_aTileLayers[tileLayerIdx_];
}

int Tilemap::GetTileWidth()
{
	return m_tileWidth;
}

int Tilemap::GetTileHeight()
{
	return m_tileHeight;
}

int Tilemap::GetMapWidth()
{
	return m_mapWidth;
}

int Tilemap::GetMapHeight()
{
	return m_mapHeight;
}

// temporal vector with firstgid for each found tileset. Keeps pair (firstgid / tilesetId).
static std::vector<int*> _tempTilesetFirstGId;

static void _LoadFromFile_LoadTileset( XMLElement* xmlElem, Tilemap* tilemap_ret, int firstgid)
{
	std::string name( xmlElem->Attribute("name") );
	int tilewidth = xmlElem->IntAttribute("tilewidth");
	int tileheight= xmlElem->IntAttribute("tileheight");

	//go into image element
	xmlElem = xmlElem->FirstChildElement();

	std::string source( xmlElem->Attribute("source") );
	source = TextUtil::CombinePath( tilemap_ret->GetRootPath(), source );

	int width = xmlElem->IntAttribute("width");
	int height = xmlElem->IntAttribute("height");

	int tilesetId = tilemap_ret->AddTileset( source, width / tilewidth, height / tileheight, tilewidth, tileheight, name);

	// Add tileset gid to temp vector to be used later to calculate tileset id
	int *data = new int[2]; data[0] = firstgid; data[1] = tilesetId;
	_tempTilesetFirstGId.push_back( data );

	// Go to tile Attributes
	xmlElem = xmlElem->NextSiblingElement();
	for( xmlElem; xmlElem; xmlElem = xmlElem->NextSiblingElement() )
	{
		int tileId = xmlElem->IntAttribute( "id" );

		XMLElement* xmlElemProperty = xmlElem->FirstChildElement();
		xmlElemProperty = xmlElemProperty->FirstChildElement(); //<--- first property
		for( xmlElemProperty; xmlElemProperty; xmlElemProperty = xmlElemProperty->NextSiblingElement() )
		{
			std::string name( xmlElemProperty->Attribute("name") );
			std::string value( xmlElemProperty->Attribute("value") );
			tilemap_ret->GetTileset( tilesetId )->AddTileProperty( tileId, name, value );
		}
	}
}


static void _LoadFromFile_LoadLayerData( XMLElement* xmlElem, Tilemap* tilemap_ret, int tileLayerId)
{
	// Go inner tile data
	xmlElem = xmlElem->FirstChildElement();
	for( int tileIdx = 0; xmlElem; xmlElem = xmlElem->NextSiblingElement(), ++tileIdx )
	{
		int gid = xmlElem->IntAttribute("gid");

		// get flip flags
		u32 tileFlags = (gid & 0xFF000000);

		// remove flags from gid
		gid = (gid & 0x00FFFFFF);

		// gid == 0 means no tileset associated
		if( gid > 0 )
		{
			std::vector<int*>::iterator iter( _tempTilesetFirstGId.begin() );
			for( ; iter != _tempTilesetFirstGId.end(); ++iter )
			{
				int *data = (*iter);
	//break>
				if( gid < data[0] ) break;
			}
			--iter;

			int tilesetGId = (*iter)[0];
			int tilesetId = (*iter)[1];
			TileLayer *tileLayer = tilemap_ret->GetTileLayer( tileLayerId );
			Tileset *tileset = tilemap_ret->GetTileset( tilesetId );

			tileLayer->SetLayerTile( tileIdx, tilesetId, tileset->GetTileId( gid - tilesetGId ), tileFlags );
		}
	}
}

static void _LoadFromFile_LoadLayerProperties( XMLElement* xmlElem, Tilemap* tilemap_ret, int tileLayerId)
{
	// Go inner layer properties
	xmlElem = xmlElem->FirstChildElement();
	for( int tileIdx = 0; xmlElem; xmlElem = xmlElem->NextSiblingElement(), ++tileIdx )
	{
		std::string name( xmlElem->Attribute("name") );
		std::string value( xmlElem->Attribute("value") );

		TileLayer *tileLayer = tilemap_ret->GetTileLayer( tileLayerId );
		
		tileLayer->AddProperty( name, value);
	}
}

static void _LoadFromFile_LoadLayer( XMLElement* xmlElem, Tilemap* tilemap_ret)
{
	std::string name( xmlElem->Attribute("name") );
	int width = xmlElem->IntAttribute("width");
	int height = xmlElem->IntAttribute("height");

	int tileLayerId = tilemap_ret->AddTileLayer( width, height, name);

	// iterate childs
	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement() )
	{
		if( 0 == strcmp("properties", xmlElem->Value()) )
		{
			_LoadFromFile_LoadLayerProperties( xmlElem, tilemap_ret, tileLayerId );
		}
		else if( 0 == strcmp("data", xmlElem->Value()) )
		{
			_LoadFromFile_LoadLayerData( xmlElem, tilemap_ret, tileLayerId );
		}
	}
}

Tilemap *Tilemap::LoadFromFile( const std::string& sFileName )
{
	XMLDocument docTileMap;
	bool loadOkay = ( docTileMap.LoadFile( sFileName.c_str() ) == XML_NO_ERROR );

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'.\n", sFileName.c_str(), docTileMap.GetErrorStr1() );
		return NULL;
	}

	// Get first element: map element
	XMLElement * xmlElem = docTileMap.FirstChildElement();
	
	int mapWidth = xmlElem->IntAttribute("width");
	int mapHeight = xmlElem->IntAttribute("height");
	int tileWidth = xmlElem->IntAttribute("tilewidth");
	int tileHeight = xmlElem->IntAttribute("tileheight");

	Tilemap *tilemap_ret = new Tilemap( mapWidth, mapHeight, tileWidth, tileHeight);

	//Set root path
	tilemap_ret->m_sRootPath = TextUtil::GetPath( sFileName );

	// go inner map
	xmlElem = xmlElem->FirstChildElement();

	while( xmlElem )
	{

		printf(" Loding %s... \n ", xmlElem->Value() );

		if( 0 == strcmp("tileset", xmlElem->Value()) )
		{
			int firstgid = xmlElem->IntAttribute("firstgid");
			const char* szsTilesetSource = xmlElem->Attribute("source");
			if( szsTilesetSource )
			{
				std::string sTilesetSource = xmlElem->Attribute("source");		
				sTilesetSource = TextUtil::CombinePath( tilemap_ret->m_sRootPath, sTilesetSource );
				XMLDocument docTileset;
				if ( !( docTileset.LoadFile(sTilesetSource.c_str()) == XML_NO_ERROR ) )
				{
					printf( "Could not load file '%s'. Error='%s'.\n", sTilesetSource, docTileset.GetErrorStr1() );
					return NULL;
				}

				// Get first element: tileset element
				XMLElement * xmlTilesetElem = docTileset.FirstChildElement();
				
				printf(" %s\n ", xmlTilesetElem->Value());

				_LoadFromFile_LoadTileset( xmlTilesetElem, tilemap_ret, firstgid );
			}
			else
			{
				_LoadFromFile_LoadTileset( xmlElem, tilemap_ret, firstgid );
			}
		}
		else if( 0 == strcmp("layer", xmlElem->Value()) )
		{
			_LoadFromFile_LoadLayer( xmlElem, tilemap_ret );
		}

		xmlElem = xmlElem->NextSiblingElement();
	}
	printf(" Tilemap loading finished. \n ");
 	return tilemap_ret;
}


static void _DrawLayerTile(TileLayer *tileLayer, irr::video::IVideoDriver* driver, int tileX_, int tileY_, int alpha)
{
	const LayerTile* layerTile = tileLayer->GetLayerTile( tileX_, tileY_);

	int tilesetId = layerTile->GetTilesetId();

	if ( tilesetId >= 0 )
    {
		Tileset *tileset = tileLayer->GetParentTilemap()->GetTileset( tilesetId );
		irr::video::ITexture* texture = tileset->GetTexture( driver );

		int tileId = layerTile->GetTileId();
		int mapTileId = tileset->GetTileMapping( tileId );
		u32 tileFlags = layerTile->GetTileFlags();

		// Check if tile is mapped to another tile ( used for animations )
		if( mapTileId != -1 )
		{
			tileId =  mapTileId;
		}

		core::rect<s32> srcRect( 
			core::position2d<s32>(tileset->GetTileX(tileId) * tileset->GetTileWidth(), tileset->GetTileY(tileId) * tileset->GetTileHeight()),
			core::dimension2d<u32>(tileset->GetTileWidth(), tileset->GetTileHeight()) 
		);

		irr::core::CMatrix4<irr::f32> matrix;
		irr::core::CMatrix4<irr::f32> mTranslation;
		irr::core::CMatrix4<irr::f32> mScale;
		irr::core::CMatrix4<irr::f32> mRotation;

		mTranslation.setTranslation(irr::core::vector3df(tileX_ * tileset->GetTileWidth(), tileY_ * tileset->GetTileHeight(), 0) );

		matrix = mRotation * mScale * mTranslation;

		Drawing::draw2DImage( 
			driver, 
			texture, 
			srcRect, 
			matrix,
			true, 
			irr::video::SColor(alpha, 255, 255, 255),
			tileFlags
		);
    }
}

static void _DrawTileLayer(TileLayer *tileLayer, irr::video::IVideoDriver* driver, int tileStartX, int tileStartY, int tileEndX, int tileEndY )
{
	if( tileStartX < 0 ) tileStartX = 0;
	if( tileStartY < 0 ) tileStartY = 0;
	if( tileEndX >= tileLayer->GetWidth() ) tileEndX = tileLayer->GetWidth() - 1;
	if( tileEndY >= tileLayer->GetHeight() ) tileEndY = tileLayer->GetHeight() - 1;

	std::string sAlpha = tileLayer->GetProperty("alpha");
	int alpha = sAlpha.length() > 0 ?  atoi( sAlpha.c_str() ) : 255;

	for (int tileX = tileStartX; tileX <= tileEndX; tileX++)
    for (int tileY = tileStartY; tileY <= tileEndY; tileY++)
    {
        _DrawLayerTile(tileLayer, driver, tileX, tileY, alpha);
    }
}

static void _DrawTileLayer(TileLayer *tileLayer, irr::video::IVideoDriver* driver)
{
	_DrawTileLayer( tileLayer, driver, 0, 0, ( tileLayer->GetWidth() - 1), ( tileLayer->GetHeight() - 1 ) );
}

void Tilemap::Draw( irr::video::IVideoDriver* driver )
{
	std::vector< TileLayer* >::iterator iter_end( m_aTileLayers.end() );
	for( std::vector< TileLayer* >::iterator iter( m_aTileLayers.begin() ); iter != iter_end; ++iter )
	{
		TileLayer* tileLayer = (*iter);

		if( tileLayer->GetProperty("Visible").compare("false") != 0 ) // dont paint layer with Visible=="false"
		{
			_DrawTileLayer( tileLayer, driver);
		}
	}
}

void Tilemap::DrawTileLayer( irr::video::IVideoDriver* driver, const std::string& tileLayerName_)
{
	_DrawTileLayer( GetTileLayer( tileLayerName_), driver);
}

void Tilemap::DrawTileLayer( irr::video::IVideoDriver* driver, int tileLayerIdx_ )
{
	_DrawTileLayer( GetTileLayer( tileLayerIdx_), driver);
}

void Tilemap::DrawTileLayer( irr::video::IVideoDriver* driver, const std::string& tileLayerName_, int tileStartX, int tileStartY, int tileEndX, int tileEndY )
{
	_DrawTileLayer( GetTileLayer( tileLayerName_), driver, tileStartX, tileStartY, tileEndX, tileEndY);
}

void Tilemap::DrawTileLayer( irr::video::IVideoDriver* driver, int tileLayerIdx_, int tileStartX, int tileStartY, int tileEndX, int tileEndY  )
{
	_DrawTileLayer( GetTileLayer( tileLayerIdx_), driver, tileStartX, tileStartY, tileEndX, tileEndY);
}