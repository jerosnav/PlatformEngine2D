#include <irrlicht.h>
#include "driverChoice.h"

#include "Tilemap.h"
#include "TileLayerNode.h"

#include "Sprite.h"
#include "AnimationPlayer.h"
#include "EntityNode.h"

#include "GameManager.h"
#include "PhyEngine.h"
#include "PhyTileEngine.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//+++ Added to find memory leaks
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//---

/*
That's it. The Scene node is done. Now we simply have to start
the engine, create the scene node and a camera, and look at the result.
*/
int main()
{


	GameManager* gameMgr = new GameManager();

	gameMgr->Initialize();

	gameMgr->Run();
	
	return 0;
}