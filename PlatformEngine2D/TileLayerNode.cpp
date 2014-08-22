	
#include "TileLayerNode.h"
#include "IGameManager.h"

using namespace irr;

TileLayerNode::TileLayerNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IGameManager* gameMgr, Tilemap *tilemap, const std::string& m_tilelayerName)
	: LayerNode(parent, mgr, id, gameMgr, 0 ), 
	m_tilemap( tilemap ),
	m_tilelayerName( m_tilelayerName )
{
	setName( m_tilelayerName.c_str() );
}

void TileLayerNode::render()
{

	if( m_tilemap->GetTileLayer( m_tilelayerName )->GetProperty("Visible").compare("false") == 0 )
	{
		return;
	}

	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ISceneNode* cameraNode = SceneManager->getActiveCamera();

	core::matrix4 mxTrans = cameraNode->getAbsoluteTransformation();
	core::vector3df vCamPos = cameraNode->getPosition();
	core::vector3df vCamScale = cameraNode->getScale();
	core::vector3df vCamRot = cameraNode->getRotation();

	// Apply parallax factor
	vCamPos.X *= m_paraxFactorX;
	vCamPos.Y *= m_paraxFactorY;

	mxTrans = 
		core::matrix4().setTranslation(vCamPos)*
		core::matrix4().setScale(vCamScale)*
		core::matrix4().setTranslation(-vCamPos)*
		core::matrix4().setRotationDegrees(vCamRot)
		;

	driver->setMaterial(Material);
	
	driver->setTransform(video::ETS_WORLD, mxTrans*AbsoluteTransformation);

	core::rectf camViewRect = m_gameMgr->GetCamera()->GetWorldViewRect();

	//printf(" WorldViewRect: %4.2f %4.2f %4.2f %4.2f\n", camViewRect.UpperLeftCorner.X, camViewRect.UpperLeftCorner.Y, camViewRect.LowerRightCorner.X, camViewRect.LowerRightCorner.Y );

	s32 tileStartX = camViewRect.UpperLeftCorner.X / m_tilemap->GetTileWidth();
	s32 tileStartY = camViewRect.UpperLeftCorner.Y / m_tilemap->GetTileHeight();
	s32 tileEndX = camViewRect.LowerRightCorner.X / m_tilemap->GetTileWidth();
	s32 tileEndY = camViewRect.LowerRightCorner.Y / m_tilemap->GetTileHeight();

	m_tilemap->DrawTileLayer( driver, m_tilelayerName, tileStartX, tileStartY, tileEndX, tileEndY );
}