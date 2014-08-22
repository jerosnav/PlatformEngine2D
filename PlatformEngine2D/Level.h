#pragma once

#include <irrlicht.h>
#include <map>
#include <vector>
#include <string>

#include "PhyEngine.h"
#include "EventReceiverMgr.h"
#include "EntityNode.h"
#include "LayerNode.h"
#include "Tilemap.h"
#include "IGameManager.h"
#include "EntityManager.h"

#include "IObjectManager.h"

class Layer
{
public:

	Layer( LayerNode* layerNode );
	~Layer();

	LayerNode* GetLayerNode(){ return m_layerNodeRef; }

private:
	LayerNode* m_layerNodeRef;
};

class Level : protected IObjectManager<Layer>
{
private:

	struct AnimTile
	{
		AnimTile( u32 tileId, int frameNb, int frameDelay, int frameSkip ) :
			tileId( tileId ),
			frameNb( frameNb ),
			frameDelay( frameDelay ),
			frameSkip( frameSkip ),
			timeCounter(0),
			timeTotal(frameNb*frameDelay)

		{}

		u32 tileId;
		int frameNb;
		int frameDelay;
		int frameSkip;
		int timeCounter;
		int timeTotal;
	};

	Level( IGameManager* gameMgr ) 
		: m_gameMgr( gameMgr ),
		m_tilemap(0)
	{}

	IGameManager* m_gameMgr;

	//AnimatedTiles
	std::vector<std::vector<AnimTile>> m_animatedTiles; // per each tileset save a vector of animated tiles

	Tilemap* m_tilemap;
	std::string m_sRootPath;

public:

	~Level();

	LayerNode* GetLayerNode( const std::string& layerName );
	Tilemap* GetTilemap() { return m_tilemap; }
	LayerTile* GetPhyLayerTileAtPos( int x, int y );

	const std::string& GetRootPath() { return m_sRootPath; }

	static Level* LoadFromFile( IGameManager* gameMgr, const std::string& sFilePath );

	void Update( u32 frameDt );
};