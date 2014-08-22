#pragma once

#include <irrlicht.h>
#include <string>

class BitMap
{
public:
    std::string BitmapFilePath;
    std::string BitmapAssetPath;
    irr::video::ITexture* m_bitmap;

	BitMap( const std::string& BitmapFilePath  ) :
		BitmapFilePath(BitmapFilePath),
		m_bitmap(0)
	{

	}

	irr::video::ITexture* GetTexture( irr::video::IVideoDriver * driver )
	{
		if( m_bitmap == 0 )
		{
			m_bitmap = driver->getTexture( BitmapFilePath.c_str() );
		}
		return m_bitmap;
	}
};