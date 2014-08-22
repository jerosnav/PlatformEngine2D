#pragma once

#include <irrlicht.h>

class LayerTile
{

private:

	int m_tilesetId;
	irr::u32 m_tileId;
	irr::u32 m_tileFlags;

public:
	LayerTile(void);
	~LayerTile(void);

	int GetTilesetId() const;
	irr::u32 GetTileId() const { return m_tileId; };
	irr::u32 GetTileFlags() const { return m_tileFlags; };

	void SetTilesetId( int tilesetId );
	void SetTileId( irr::u32 tileId );
	void SetTileFlags( irr::u32 flags );
};

