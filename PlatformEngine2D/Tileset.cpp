#include "Tileset.h"


Tileset::Tileset(void)
{

}


Tileset::~Tileset(void)
{
	// Remove Tile properties
	{
		std::map< irr::u32, Properties * >::iterator iter( m_dicTileProperties.begin() );
		std::map< irr::u32, Properties * >::iterator iter_end( m_dicTileProperties.end() );

		while( iter != iter_end )
		{
			delete iter->second;
			++iter;
		}
	}
}


Tileset::Tileset( const std::string& sTextureAsset_, int iWidth_, int iHeight_, int iTileWidth_, int iTileHeight_)	: 
	m_sTextureAsset( sTextureAsset_ ), 
	m_tileWidth( iTileWidth_ ), 
	m_tileHeight( iTileHeight_ ), 
	m_width( iWidth_),
	m_height( iHeight_ ),
	m_dicTileProperties(),
	m_texture(0)
{ 

}

irr::video::ITexture * Tileset::GetTexture( irr::video::IVideoDriver* driver )
{
	if( !m_texture )
	{
		m_texture = driver->getTexture(m_sTextureAsset.c_str());
		//driver->makeColorKeyTexture(m_texture, irr::core::position2d<irr::s32>(0,0));
	}

	return m_texture;
}


void Tileset::AddTileProperty( int tileIdx, const std::string& sPropertyName_, const std::string& value_ )
{
	int tileX = tileIdx % this->m_width;
	int tileY = tileIdx / this->m_width; 
	Tileset::AddTileProperty( tileX, tileY, sPropertyName_, value_ );
}

void Tileset::AddTileProperty( int tileX_, int tileY_, const std::string& sPropertyName_, const std::string& value_ )
{
	irr::u32 tileKey = GetTileId(tileX_, tileY_);

	// Check if key exist
	if ( m_dicTileProperties.find( tileKey ) == m_dicTileProperties.end())
	{
		m_dicTileProperties[tileKey] = new Properties();
	}

	m_dicTileProperties[tileKey]->AddProperty(sPropertyName_, value_);
}

std::string Tileset::GetTileProperty( int tileIdx, const std::string& sPropertyName_ )
{
	int tileX = tileIdx % this->m_width;
	int tileY = tileIdx / this->m_width; 
	return GetTileProperty( tileX, tileY, sPropertyName_ );
}

std::string Tileset::GetTileProperty( int tileX_, int tileY_, const std::string& sPropertyName_ )
{
	irr::u32 tileKey = GetTileId(tileX_, tileY_);

	return m_dicTileProperties.find(tileKey) != m_dicTileProperties.end()? m_dicTileProperties[tileKey]->GetProperty( sPropertyName_ ) : "";
}

irr::u32 Tileset::GetTileId(int tileIdx_)
{
	int tileX = tileIdx_ % this->m_width;
	int tileY = tileIdx_ / this->m_width; 
	return GetTileId( tileX, tileY );
}

irr::u32 Tileset::GetTileId(int tileX_, int tileY_)
{
	return (irr::u32)(((tileX_ & 0xFFFF) << 16) | (tileY_ & 0xFFFF));
}

int Tileset::GetTileX(irr::u32 tileId_)
{
	return (int)((tileId_ >> 16) & 0xFFFF);
}

int Tileset::GetTileY(irr::u32 tileId_)
{
	return (int)(tileId_ & 0xFFFF);
}