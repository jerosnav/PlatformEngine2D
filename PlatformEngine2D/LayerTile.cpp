#include "LayerTile.h"


LayerTile::LayerTile( void ) :
m_tilesetId(-1),
m_tileId(0)
{
}


LayerTile::~LayerTile(void)
{
}


int LayerTile::GetTilesetId() const
{
	return m_tilesetId;
}

void LayerTile::SetTilesetId( int tilesetId )
{
	m_tilesetId = tilesetId;
}

void LayerTile::SetTileId( irr::u32 tileId )
{
	m_tileId = tileId;
}

void LayerTile::SetTileFlags( irr::u32 flags )
{
	m_tileFlags = flags;
}