#include "ICamera.h"

void ICamera::Update( u32 gameTime )
{
	if( m_isMoveToTarget )
	{
		m_isMoveToTarget = false;
		core::vector2df camPos = GetPosition();
		camPos.X += (m_targetPosition.X - camPos.X)/8;
		camPos.Y += (m_targetPosition.Y - camPos.Y)/8;
		SetPosition( camPos );
	}
}

void ICamera::SetPosition( core::vector2df camPos_ )
{

	core::vector2df position(camPos_.X, camPos_.Y);

	// clip to bounding box
	if( m_boundingBox.isValid() )
	{
		core::rectf transRect( 
			(m_viewRect.UpperLeftCorner.X / m_cameraNode->getScale().X) + position.X,
			(m_viewRect.UpperLeftCorner.Y / m_cameraNode->getScale().Y) + position.Y,
			(m_viewRect.LowerRightCorner.X / m_cameraNode->getScale().X) + position.X,
			(m_viewRect.LowerRightCorner.Y / m_cameraNode->getScale().Y) + position.Y
		);

		transRect.constrainTo( m_boundingBox );

		position = transRect.getCenter();
	}

	core::vector3df v3Pos( -position.X, -position.Y, 1 );
	m_cameraNode->setPosition( v3Pos );
	v3Pos.Z = 0;
	m_cameraNode->setTarget( v3Pos );
}