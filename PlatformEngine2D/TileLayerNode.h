#pragma once

#include <irrlicht.h>

#include "TileLayer.h"
#include "Tilemap.h"
#include "LayerNode.h"

using namespace irr;

class TileLayerNode : public LayerNode
{

private:

	Tilemap *m_tilemap;
	std::string m_tilelayerName;

public:

	TileLayerNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IGameManager* gameMgr, Tilemap *tilemap, const std::string& tilelayerName);

	virtual void render();
};