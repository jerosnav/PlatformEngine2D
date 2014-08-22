#include "LayerNode.h"

#include "Drawing.h"
#include "AlgorithmUtil.h"

void LayerNode::OnRegisterSceneNode()
{
	if (IsVisible)
		// use ESNRP_CAMERA to allow draw nodes in the same order of parent children list
		SceneManager->registerNodeForRendering(this, scene::E_SCENE_NODE_RENDER_PASS::ESNRP_CAMERA);

	ISceneNode::OnRegisterSceneNode();
}

static bool __deph_comp( scene::ISceneNode* p1, scene::ISceneNode* p2) 
{ 
	return ( p1->getPosition().Z > p2->getPosition().Z ); 
}

void LayerNode::OnAnimate(u32 timeMs)
{

	// Call base
	ISceneNode::OnAnimate( timeMs );

	if( LastAnimationTime == 0 )
	{
		LastAnimationTime = timeMs;
	}
	else
	{
		u32 timeDt = timeMs - LastAnimationTime;

		Update( timeDt ) ;
	}
	LastAnimationTime = timeMs;

	// Sort entities by Z ( TODO: improve this!!! )
	Children = AlgorithmUtil::SortList<scene::ISceneNode*>( Children, (AlgorithmUtil::TCompFP)(__deph_comp));
}

void LayerNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ISceneNode* cameraNode = SceneManager->getActiveCamera();
	
	core::matrix4 mxTrans = cameraNode->getAbsoluteTransformation();
	core::vector3df vCamPos = cameraNode->getPosition();
	core::vector3df vCamScale = cameraNode->getScale();
	core::vector3df vCamRot = cameraNode->getRotation();

	//Zoom factor: for Factor=0, zoom = 1; for Factor=1, zoom = vCamScale;
	vCamScale.X = 1 - ( (1 - vCamScale.X) * m_paraxZoomFactor );
	vCamScale.Y = 1 - ( (1 - vCamScale.Y) * m_paraxZoomFactor );

	core::vector3df vCamPos2 = cameraNode->getPosition();
	
	// Apply Offset
	core::vector3df vOffset(m_offsetX, m_offsetY, 0);

	// Apply parallax factor
	vCamPos2.X *= m_paraxFactorX;
	vCamPos2.Y *= m_paraxFactorY;

	mxTrans = 
		core::matrix4().setTranslation(vCamPos)*
		core::matrix4().setScale(vCamScale)*
		core::matrix4().setTranslation(-vCamPos)*
		core::matrix4().setRotationDegrees(vCamRot)*
		core::matrix4().setTranslation(-vCamPos)*		// remove camera offset because is applied automatically later by Irrlicht
		core::matrix4().setTranslation(vOffset)*		// apply offset without parallax factor to make easy to adjust layer
		core::matrix4().setTranslation(vCamPos2)		// then apply modified by parallax factor camera position
		;
	driver->setMaterial(Material);


	driver->setTransform(video::ETS_WORLD, mxTrans*AbsoluteTransformation);

	Drawing::draw2DImage( driver, m_texture, core::recti( 0, 0, m_width, m_height), core::IdentityMatrix, true, video::SColor(255, 255, 255, 255));

}

const core::aabbox3d<f32>& LayerNode::getBoundingBox() const
{
	return Box;
}

u32 LayerNode::getMaterialCount() const
{
	return 1;
}

video::SMaterial& LayerNode::getMaterial(u32 i)
{
	return Material;
}

void LayerNode::Update(u32 timeMs)
{
	;// update anything
}