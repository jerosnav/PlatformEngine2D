#pragma once

#include <irrlicht.h>
#include "IGameManager.h"

using namespace irr;

class DebugMgr
{
public:
	static void DrawWorldRect( IGameManager* gameMgr, core::recti rect, video::SColor color, core::matrix4 mxTrans = core::matrix4() )
	{
		scene::ISceneNode* cameraNode = gameMgr->GetSceneManager()->getActiveCamera();
	
		core::vector3df vCamPos = cameraNode->getPosition();
		core::vector3df vCamScale = cameraNode->getScale();

		core::recti rectDebug(rect);
		core::matrix4 mxTemp = 
			core::matrix4().setTranslation( core::vector3df(gameMgr->GetDriver()->getScreenSize().Width / 2, gameMgr->GetDriver()->getScreenSize().Height / 2, 0) )*
				core::matrix4().setScale(vCamScale)*
				core::matrix4().setTranslation(vCamPos)*
				mxTrans
				;
		core::vector3df vTopLeft( rectDebug.UpperLeftCorner.X, rectDebug.UpperLeftCorner.Y, 0);
		core::vector3df vBottomRight( rectDebug.LowerRightCorner.X, rectDebug.LowerRightCorner.Y, 0);
		mxTemp.transformVect( vTopLeft );
		mxTemp.transformVect( vBottomRight );
		rectDebug.UpperLeftCorner = core::vector2di( vTopLeft.X, vTopLeft.Y );
		rectDebug.LowerRightCorner = core::vector2di( vBottomRight.X, vBottomRight.Y );

		gameMgr->GetDriver()->draw2DRectangle( color, rectDebug );
	}
};