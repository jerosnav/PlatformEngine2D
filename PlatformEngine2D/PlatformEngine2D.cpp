/** Example 003 Custom SceneNode

This Tutorial is more advanced than the previous ones.
If you are currently just playing around with the Irrlicht
engine, you may want to look at other examples first.
This tutorials shows how to create a custom scene node and
how to use it in the engine. A custom scene node is needed
if you want to implement a render technique the Irrlicht
Engine currently does not support. For example, you can write
an indoor portal based renderer or an advanced terrain scene
node with it. By creating custom scene nodes, you can
easily extend the Irrlicht Engine and adapt it to your own
needs.

I will keep the tutorial simple: Keep everything very
short, everything in one .cpp file, and I'll use the engine
here as in all other tutorials.

To start, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
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

/*
That's it. Compile and play around with the program.
**/
