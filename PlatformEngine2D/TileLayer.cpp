#include "TileLayer.h"


TileLayer::TileLayer(void)
{
}


TileLayer::~TileLayer(void)
{
	DBG("~TileLayer " << this->GetTag());
	delete [] m_LayerTiles;

	delete m_properties;
}

TileLayer::TileLayer( Tilemap *tilemapRef, int width_, int height_, const std::string& sTag_ ) :
m_parentTilemap( tilemapRef),
	m_width( width_ ),
	m_height( height_ ),
	m_tag( sTag_ ),
	m_boundRect( 0, 0, width_ * tilemapRef->GetTileWidth(), height_ * tilemapRef->GetTileHeight() )	
{

  	m_properties = new Properties();
    m_LayerTiles = new LayerTile[width_ * height_];
           
    //ResetLayerTiles(); //TODO
}

void TileLayer::SetLayerTile(int tileIdx_, int tilesetId_, irr::u32 tileId_, irr::u32 flags_)
{
	m_LayerTiles[tileIdx_].SetTilesetId( tilesetId_ );
	m_LayerTiles[tileIdx_].SetTileId( tileId_ );
	m_LayerTiles[tileIdx_].SetTileFlags( flags_ );
}

/* TODO
void TileLayer::SetLayerTile(int tileX_, int tileY_, int tilesetId_, irr::u32 tileId_)
{
}

void TileLayer::ResetLayerTiles()
{
}
*/
