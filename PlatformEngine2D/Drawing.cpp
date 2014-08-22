
#include "Drawing.h"

#include <irrlicht.h>

using namespace irr;

void Drawing::draw2DImage(
		irr::video::IVideoDriver *driver, 
		irr::video::ITexture* texture , 
		irr::core::rect<irr::s32> sourceRect,
		irr::core::CMatrix4<irr::f32> transformationMatrix,
		bool useAlphaChannel, 
		irr::video::SColor color,
		irr::u32	flags
	)
	{
   
	   irr::video::SMaterial material;

	   // Find the positions of corners
	   irr::core::vector3df corner[4];// = { static_cast<irr::f32>(sourceRect.UpperLeftCorner.X), sourceRect.UpperLeftCorner.Y, sourceRect.UpperLeftCorner.X };

	   corner[0] = irr::core::vector3df( 0, 0, 0 );
	   corner[1] = irr::core::vector3df( sourceRect.getWidth(), 0, 0 );
	   corner[2] = irr::core::vector3df( 0, sourceRect.getHeight(), 0 );
	   corner[3] = irr::core::vector3df( sourceRect.getWidth(), sourceRect.getHeight(), 0 );

	   transformationMatrix.transformVect(corner[0]);
	   transformationMatrix.transformVect(corner[1]);
	   transformationMatrix.transformVect(corner[2]);
	   transformationMatrix.transformVect(corner[3]);

	   static f32 _offset = 0.01f;// this looks like fix problem of draw a pixel outside tile

	   // Find the uv coordinates of the sourceRect
	   irr::core::vector2df uvCorner[4];
	   uvCorner[0] = irr::core::vector2df( sourceRect.UpperLeftCorner.X+_offset,	sourceRect.UpperLeftCorner.Y+_offset );
	   uvCorner[1] = irr::core::vector2df( sourceRect.LowerRightCorner.X-_offset,	sourceRect.UpperLeftCorner.Y+_offset );
	   uvCorner[2] = irr::core::vector2df( sourceRect.UpperLeftCorner.X+_offset,	sourceRect.LowerRightCorner.Y-_offset );
	   uvCorner[3] = irr::core::vector2df( sourceRect.LowerRightCorner.X-_offset,	sourceRect.LowerRightCorner.Y-_offset );
	   for (int x = 0; x < 4; x++) {
		  float uvX = uvCorner[x].X/(float)texture->getSize().Width;
		  float uvY = uvCorner[x].Y/(float)texture->getSize().Height;
		  uvCorner[x] = irr::core::vector2df(uvX,uvY);
	   }

	   // Vertices for the image
	   irr::video::S3DVertex vertices[4];
	   irr::u16 indices[6] = { 0, 1, 2, 3 ,2 ,1 };

	   // apply flags
	   if( flags & FLIPPED_HORIZONTALLY_FLAG )
	   {
		   std::swap( uvCorner[0], uvCorner[1] );
		   std::swap( uvCorner[3], uvCorner[2] );
	   }
	   if( flags & FLIPPED_VERTICALLY_FLAG )
	   {
		   std::swap( uvCorner[0], uvCorner[2] );
		   std::swap( uvCorner[1], uvCorner[3] );
	   }
	   if( flags & FLIPPED_DIAGONALLY_FLAG )
	   {
		   std::swap( uvCorner[0], uvCorner[3] );
	   }

	   // Convert pixels to world coordinates
	   float screenWidth = driver->getScreenSize().Width;
	   float screenHeight = driver->getScreenSize().Height;
	   for (int x = 0; x < 4; x++) 
	   {
		  float screenPosX = corner[x].X;
		  float screenPosY = corner[x].Y;
		  vertices[x].Pos = irr::core::vector3df(screenPosX,screenPosY,1);
		  vertices[x].TCoords = uvCorner[x];
		  vertices[x].Color = color;
	   }

	   material.AntiAliasing = 0;
	   material.TextureLayer[0].BilinearFilter = false;
	   material.TextureLayer[0].TrilinearFilter = false;
	   material.TextureLayer[0].AnisotropicFilter = false;
	   material.TextureLayer[0].TextureWrapU = video::E_TEXTURE_CLAMP::ETC_REPEAT;
	   material.TextureLayer[0].TextureWrapV = video::E_TEXTURE_CLAMP::ETC_REPEAT;
	   material.Lighting = false;
	   material.ZWriteEnable = false;
	   material.ZBuffer = false;
	   material.TextureLayer[0].Texture = texture;
	   material.MaterialTypeParam = irr::video::pack_texureBlendFunc(irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_TEXTURE | irr::video::EAS_VERTEX_COLOR); 

	   if (useAlphaChannel)
		  material.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
	   else
		  material.MaterialType = irr::video::EMT_SOLID;

	   driver->setMaterial(material);
	   driver->drawIndexedTriangleList(&vertices[0],4,&indices[0],2);

	   // Restore projection and view matrices
	   //driver->setTransform(irr::video::ETS_PROJECTION,oldProjMat);
	   //driver->setTransform(irr::video::ETS_VIEW,oldViewMat);
	}