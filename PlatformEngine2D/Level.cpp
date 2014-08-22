#include "Level.h"

#include <irrXML.h>
#include "TileLayerNode.h"
#include "TextUtil.h"
#include "PhyTileEngine.h"
#include "Properties.h"

using namespace irr;

Layer::Layer( LayerNode* layerNode ) :
	m_layerNodeRef( layerNode )
{}

Layer::~Layer()
{
	/* removed by calling GetSceneManager()->clear(); inside UnloadRoom
	if( m_layerNodeRef )
	{
		m_layerNodeRef->getSceneManager()->addToDeletionQueue( m_layerNodeRef );
	}
	*/
}

Level::~Level()
{
	// reset phyengine
	m_gameMgr->SetPhysicEngine(0);

	if( m_tilemap ) 
	{
		delete m_tilemap;
	}
}

LayerNode* Level::GetLayerNode( const std::string& layerName ) 
{ 
	object_t* pObjContainer = IObjectManager::GetFirstByTag(layerName);
	return pObjContainer? pObjContainer->GetData()->GetLayerNode() : 0;
}

LayerTile* Level::GetPhyLayerTileAtPos(int x, int y)
{
	int tileX = x / GetTilemap()->GetTileWidth();
	int tileY = y / GetTilemap()->GetTileHeight();

	return GetTilemap()->GetTileLayer("Physic")->GetLayerTile( tileX, tileY);
}

Level* Level::LoadFromFile( IGameManager* gameMgr, const std::string& sFilePath )
{
	Level* level_ret = 0;
	io::IrrXMLReader* xml = io::createIrrXMLReader(sFilePath.c_str());

	// get first tilemap node
	while( xml && xml->read() && (std::string("tilemap").compare(xml->getNodeName()) != 0) );

	// Check if tilemap node found
	if( xml )
	{
		// get tilemap source
		std::string tilemapSource = xml->getAttributeValue("source");

		level_ret = new Level( gameMgr );
		gameMgr->SetLevel( level_ret );

		// Root
		level_ret->m_sRootPath = TextUtil::GetPath( sFilePath );

		printf(" Loding tilemap... \n ");
		tilemapSource = TextUtil::CombinePath( level_ret->m_sRootPath, tilemapSource );
		level_ret->m_tilemap = Tilemap::LoadFromFile( tilemapSource );

		printf(" Loding animated tiles... \n ");
		for( int i = 0; i < level_ret->m_tilemap->GetTilesetCounter(); ++i )
		{
			level_ret->m_animatedTiles.push_back( std::vector<AnimTile>() );
			Tileset* tileset = level_ret->m_tilemap->GetTilesetByIdx(i);
			int tilesCounter = tileset->GetWidth() * tileset->GetHeight();
			for( int j = 0; j < tilesCounter; ++j )
			{
				std::string sSetTileAnim = tileset->GetTileProperty(j, "SetTileAnim");
				if( sSetTileAnim.length() > 0 )
				{
					size_t tokenIdx0;
					size_t tokenIdx1;

					//get param frame nb
					tokenIdx0 = sSetTileAnim.find(';');
					int frameNb = strtol( sSetTileAnim.substr(0, tokenIdx0).c_str(), 0, 10);

					// get param frame delay
					tokenIdx1 = sSetTileAnim.find(';', tokenIdx0+1);
					int frameDelay = strtol( sSetTileAnim.substr(tokenIdx0+1, tokenIdx1 - tokenIdx0 - 1).c_str(), 0, 10);

					// get param frame skip
					tokenIdx0 = sSetTileAnim.find(';', tokenIdx1 + 1);
					int frameSkip = strtol( sSetTileAnim.substr(tokenIdx1+1, tokenIdx0 - tokenIdx1 - 1).c_str(), 0, 10);


					AnimTile animTile( tileset->GetTileId(j), frameNb, frameDelay, frameSkip );
					level_ret->m_animatedTiles[i].push_back( animTile );
				}
			}
		}

		// Create and set physics engine
		gameMgr->SetPhysicEngine( new PhyTileEngine( gameMgr->GetDriver(), level_ret->m_tilemap, level_ret->m_tilemap->GetTileLayer( "Physic" )) );

		// TODO: load gravity from level
		gameMgr->GetPhyEngine()->SetGravity( core::vector2df(0, 600) );

		// Load layers
		printf(" Loding layers... \n " );
		u16 nextLayerId = 1; // reserve 0 for camera???
		std::string sLastAddedLayerName;
		while( xml && xml->read() )
		{

//continue> skip all nodes but EXN_ELEMENT
			if( xml->getNodeType() != io::EXML_NODE::EXN_ELEMENT ) continue;

			if(std::string("layer").compare(xml->getNodeName()) == 0)
			{
				std::string layerName = xml->getAttributeValueSafe("name");
				std::string layerImgSource = xml->getAttributeValueSafe("imgSrc");
				f32 layerWidth = xml->getAttributeValueAsFloat("width");
				f32 layerHeight = xml->getAttributeValueAsFloat("height");
				f32 layerOffsetX = xml->getAttributeValueAsFloat("offsetX");
				f32 layerOffsetY = xml->getAttributeValueAsFloat("offsetY");
				f32 parallaxFactorX = xml->getAttributeValueAsFloat("parallaxFactorX");
				f32 parallaxFactorY = xml->getAttributeValueAsFloat("parallaxFactorY");
				f32 parallaxZoomFactor = xml->getAttributeValueAsFloat("parallaxZoomFactor");

				printf(" Loding layer %s ... \n ", layerName.c_str() );

				// Layer has an incremental id set on most significant half, less significant half is set to 0
				s32 layerId = nextLayerId << 16;

				LayerNode *layerNode;
				TileLayer* tileLayer = level_ret->m_tilemap->GetTileLayer(layerName);
				sLastAddedLayerName = layerName;

				// if layer is a tileLayer
				if( tileLayer )
				{
					layerNode = new TileLayerNode(gameMgr->GetSceneManager()->getRootSceneNode(), gameMgr->GetSceneManager(), layerId, gameMgr, level_ret->m_tilemap, layerName);

					// Add object to collection
					Layer* layer = new Layer( layerNode );
					level_ret->AddObjectData( layer, layerName );

					//if( layerName.compare("Objects") == 0 )
					{
						for( s32 iTileIdx = 0; iTileIdx < tileLayer->GetLayerTileCount(); iTileIdx++)
						{
							const LayerTile* layerTile = tileLayer->GetLayerTile( iTileIdx );
							Tileset* tileset = level_ret->m_tilemap->GetTileset( layerTile->GetTilesetId() );

//continue>
							if( !tileset || (layerTile->GetTileId() == 0) ) continue;

							std::string sCreateEntityTemplate = tileset->GetTilePropertyById( layerTile->GetTileId(), "CreateEntity");
							if( sCreateEntityTemplate.length() > 0 )
							{
								EntityNode* entity = level_ret->m_gameMgr->GetEntityFactory()->CreateEntity( sLastAddedLayerName, "");

								// Read all properties from tile and copy in entity
								Properties * tileProperties = tileset->GetTilePropertiesById( layerTile->GetTileId() );
								for( std::string sTileParam = tileProperties->GetFirstProperty(); sTileParam.length() > 0; sTileParam = tileProperties->GetNextProperty() )
								{
									std::string sValue = tileset->GetTilePropertyById( layerTile->GetTileId(), sTileParam );

									// Check integer param
									if( sTileParam.find("int:") == 0 )
									{
										sTileParam.erase(0, (sizeof("int:")-1) );
										entity->SetIntParam( sTileParam, atoi(sValue.c_str()) );
									}
									// String param
									else
									{
										entity->SetStringParam( sTileParam, sValue );
									}
								}
								//---

								// Build entity
								level_ret->m_gameMgr->GetEntityFactory()->BuildEntityFromTemplate( entity, sCreateEntityTemplate, 0 );	

								// Set position
								core::vector2df position = core::vector2df(
									(iTileIdx % tileLayer->GetWidth()) * tileset->GetTileWidth(), //x
									(iTileIdx / tileLayer->GetWidth()) * tileset->GetTileHeight() //y
								);
								if( entity->GetPhysicObject() )
								{
									//NOTE: add position so in case position was offset was set during BuildEntityFromTemplate it remains
									entity->GetPhysicObject()->Position += position;
								}
								else
								{
									// Entity collision is not compatible with entities without physicObject
									entity->setPosition( core::vector3df( position.X, position.Y, 0  ));
								}

								printf(" Creating entity %s pos: %f, %f ... \n ", sCreateEntityTemplate.c_str(), position.X, position.Y );
								//DBG(" CreateEntity: " << sCreateEntityTemplate << " pos: " << position.X << ", " << position.Y );

								//+++Special Property: SetSprite
								std::string sSprite = tileset->GetTilePropertyById( layerTile->GetTileId(), "SetSprite");
								if( sSprite.length() > 0 )
								{
									entity->SetSpriteComponent(sSprite);
								}
								//---
								//+++Special Property: SetAnim
								std::string sAnim = tileset->GetTilePropertyById( layerTile->GetTileId(), "SetAnim");
								if( sAnim.length() > 0 )
								{
									entity->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag( sAnim, -1);
								}
								//---

							}

						}						
					}
				}
				else
				{
					video::ITexture * layerTexture = level_ret->m_gameMgr->GetDriver()->getTexture( layerImgSource.c_str() );
					layerNode = new LayerNode( gameMgr->GetSceneManager()->getRootSceneNode(), gameMgr->GetSceneManager(), layerId, gameMgr, layerTexture);
					layerNode->setName(layerName.c_str());

					// Add object to collection
					Layer* layer = new Layer( layerNode );
					level_ret->AddObjectData( layer, layerName );
				}

				layerNode->SetParallaxFactorX( parallaxFactorX );
				layerNode->SetParallaxFactorY( parallaxFactorY );
				layerNode->SetParallaxZoomFactor( parallaxZoomFactor );
				layerNode->SetOffset( layerOffsetX, layerOffsetY );
				layerNode->SetWidth( layerWidth );
				layerNode->SetHeight( layerHeight );				

				layerNode->drop();
				layerNode = 0;
			}

			if(std::string("entity").compare(xml->getNodeName()) == 0)
			{

				std::string entityTemplate = xml->getAttributeValueSafe("template");
				std::string entityName = xml->getAttributeValueSafe("name");

				EntityNode* entityNode = level_ret->m_gameMgr->GetEntityFactory()->CreateEntity( sLastAddedLayerName, entityName );

				level_ret->m_gameMgr->GetEntityFactory()->BuildEntityFromTemplate( entityNode, entityTemplate, xml );
			}
		}
//return> level
		return level_ret;
	}

	// delete the xml parser after usage
	delete xml;

	// Error
	return 0;
}

void Level::Update( u32 frameDt )
{
	int tileSetIdx = 0;
	std::vector< std::vector<AnimTile> >::iterator iterTileset_end( m_animatedTiles.end());
	for( std::vector< std::vector<AnimTile> >::iterator iterTileset( m_animatedTiles.begin());
		iterTileset != iterTileset_end;
		++iterTileset, ++tileSetIdx
	)
	{
		std::vector<AnimTile> *animTilesVect = &(*iterTileset);
		std::vector<AnimTile>::iterator iterAnimTile_end = animTilesVect->end();
		for( std::vector<AnimTile>::iterator iterAnimTile = animTilesVect->begin();
			iterAnimTile != iterAnimTile_end;
			++iterAnimTile
		)
		{
			AnimTile *animTile = &(*iterAnimTile);

			animTile->timeCounter += frameDt;
			animTile->timeCounter %= animTile->timeTotal;

// NOTE: tile id is the position in tileset, first 16bits for x pos and last 16 bits y pos. 
// This sum current anim frame to X part, so animated frame should be in the same row
			u32 curTileId = animTile->tileId + (( animTile->frameSkip * (animTile->timeCounter / animTile->frameDelay) ) << 16);

			m_tilemap->GetTileset( tileSetIdx )->SetTileMapping( animTile->tileId, curTileId );
		}
	}
}