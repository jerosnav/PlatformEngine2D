#pragma once

#include <irrlicht.h>


using namespace irr;

class ICamera
{
public:

	virtual void Update( u32 gameTime );

	ICamera( scene::ISceneManager* smgr, f32 width, f32 height ) :
	m_boundingBox(1, 1, -1, -1),
	m_viewRect( -width/2, -height/2, width/2, height/2 ), // camera position is centered
	m_isMoveToTarget(false)
	{
		m_cameraNode = smgr->addCameraSceneNode(0, core::vector3df(0,0,1), core::vector3df(0,0,0));
		core::matrix4 proj;
		proj.buildProjectionMatrixOrthoLH( width, height, 0.0f, 1000.0f );
		m_cameraNode->setProjectionMatrix(proj, true);
		m_cameraNode->setRotation(core::vector3df(180, 180, 0));
	}

	virtual void Reset()
	{
		m_boundingBox = core::rectf(1, 1, -1, -1);
		m_isMoveToTarget = false;
		SetPosition( core::vector2df(0) );
	}

	void ApplyOffset( core::vector2df position )
	{
		m_cameraNode->setPosition( m_cameraNode->getPosition() + core::vector3df( -position.X, -position.Y, 0 ) );
	}

	void SetPosition( core::vector2df camPos_ );

	core::vector2df GetPosition()
	{
		core::vector3df camPos = m_cameraNode->getPosition();
		
		return core::vector2df( -camPos.X, -camPos.Y );
	}

	void SetScale( f32 scale )
	{
		m_cameraNode->setScale( core::vector3df(scale, scale, 1));
	}

	f32 GetScale( )
	{
		return m_cameraNode->getScale().X;
	}

	core::rectf GetViewRect() { return m_viewRect; }

	core::rectf GetWorldViewRect()
	{
		/*
		core::vector3df camTopLeft = core::vector3df( m_viewRect.UpperLeftCorner.X, m_viewRect.UpperLeftCorner.Y, 0);
		core::vector3df camBottomRight = core::vector3df( m_viewRect.LowerRightCorner.X, m_viewRect.LowerRightCorner.Y, 0);
		
		core::vector3df vCamPos		= m_cameraNode->getPosition();
		core::vector3df vCamScale	= m_cameraNode->getScale();
		core::vector3df vCamRot		= m_cameraNode->getRotation();

		core::matrix4 mxTrans = 
		core::matrix4().setScale(vCamScale)*
		core::matrix4().setTranslation(-vCamPos)
		;

		mxTrans.transformVect( camTopLeft );
		mxTrans.transformVect( camBottomRight );
		*/

		core::vector3df vCamPos		= -m_cameraNode->getPosition();
		core::vector3df vCamScale	= m_cameraNode->getScale();

		return core::rectf( 
			(m_viewRect.UpperLeftCorner.X / vCamScale.X) + vCamPos.X, 
			(m_viewRect.UpperLeftCorner.Y / vCamScale.Y) + vCamPos.Y, 
			(m_viewRect.LowerRightCorner.X / vCamScale.X) + vCamPos.X, 
			(m_viewRect.LowerRightCorner.Y / vCamScale.Y) + vCamPos.Y 
		);

	}

	void SetBoundingBox( core::rectf boundingBox )
	{
		m_boundingBox = boundingBox;
	}

	const core::rectf GetBoundingBox( ) const
	{
		return m_boundingBox;
	}

	bool MoveTo( core::vector2df target )
	{
		m_isMoveToTarget = true;
		m_targetPosition = target;
		core::vector2df camPos = GetPosition();

		return camPos != target;
	}

protected:

	scene::ICameraSceneNode* m_cameraNode;

	core::rectf m_boundingBox;
	core::rectf m_viewRect;

	bool m_isMoveToTarget; // used to move when updating for syncronization
	core::vector2df m_targetPosition;
};