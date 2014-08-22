#include "Sprite.h"

#include "..\tinyxml2\tinyxml2.h"
#include "..\TextUtil.h"

using namespace tinyxml2;

irr::u32 Sprite::m_aKeyGenerator[ ] =
{
    0x1000, // Module
    0x2000, // MkRect
    0x3000, // MkPoint
    0x4000, // Frame
    0x5000, // Animation
};


static void draw2DImage(
	irr::video::IVideoDriver *driver, 
	irr::video::ITexture* texture , 
	irr::core::vector2df position,
	irr::core::rect<irr::s32> sourceRect,
	irr::core::vector2di origin,
	bool useAlphaChannel, 
	irr::video::SColor color,
	u32 spriteEffects

) 
{
   
	irr::video::SMaterial material;

	// Find the positions of corners
	irr::core::vector3df corner[4];// = { static_cast<irr::f32>(sourceRect.UpperLeftCorner.X), sourceRect.UpperLeftCorner.Y, sourceRect.UpperLeftCorner.X };

	corner[0] = irr::core::vector3df( 0, 0, 0 );
	corner[1] = irr::core::vector3df( sourceRect.getWidth(), 0, 0 );
	corner[2] = irr::core::vector3df( 0, sourceRect.getHeight(), 0 );
	corner[3] = irr::core::vector3df( sourceRect.getWidth(), sourceRect.getHeight(), 0 );

	irr::core::vector3df tempV3( position.X - origin.X, position.Y - origin.Y, 0);
	corner[0] += tempV3;
	corner[1] += tempV3;
	corner[2] += tempV3;
	corner[3] += tempV3;

	// Find the uv coordinates of the sourceRect
	irr::core::vector2df uvCorner[4];
	uvCorner[0] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.UpperLeftCorner.Y);
	uvCorner[1] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.UpperLeftCorner.Y);
	uvCorner[2] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.LowerRightCorner.Y);
	uvCorner[3] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.LowerRightCorner.Y);

	// Apply Sprite effects
	if (spriteEffects & eSpriteEffects::SPRITE_EFFECT_FLIP_X)
    {
		core::swap<core::vector2df, core::vector2df>( uvCorner[0], uvCorner[1] );
		core::swap<core::vector2df, core::vector2df>( uvCorner[2], uvCorner[3] );
    }
	if (spriteEffects & eSpriteEffects::SPRITE_EFFECT_FLIP_Y)
	{
		core::swap<core::vector2df, core::vector2df>( uvCorner[0], uvCorner[2] );
		core::swap<core::vector2df, core::vector2df>( uvCorner[1], uvCorner[3] );
	}

	for (int x = 0; x < 4; x++) {
		float uvX = uvCorner[x].X/(float)texture->getSize().Width;
		float uvY = uvCorner[x].Y/(float)texture->getSize().Height;
		uvCorner[x] = irr::core::vector2df(uvX,uvY);
	}

	// Vertices for the image
	irr::video::S3DVertex vertices[4];
	irr::u16 indices[6] = { 0, 1, 2, 3 ,2 ,1 };

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
}


 void Sprite::DrawFrame(Frame *frame_, irr::core::vector2df position_)
{
	std::vector <FModule*>::iterator iter_end(frame_->Modules.end());
	for( std::vector <FModule*>::iterator iter(frame_->Modules.begin()); iter != iter_end; ++iter )
    {

		FModule *objFModule = (*iter);

        Module *objModule = m_dicModules[objFModule->Id];

		core::recti srcRect = objModule->Rect;

        irr::core::vector2di finalOrigin = Origin - objFModule->Offset;

        // adjust final position to center flip in origin
		if (SpriteEffects & eSpriteEffects::SPRITE_EFFECT_FLIP_X)
        {
			finalOrigin.X -= 2 * finalOrigin.X - srcRect.getWidth();
        }

		if (SpriteEffects & eSpriteEffects::SPRITE_EFFECT_FLIP_Y)
        {
			finalOrigin.Y -= 2 * finalOrigin.Y - srcRect.getHeight();
        }

		draw2DImage( 
			m_driver,
			BitMaps[objModule->ImgIdx]->GetTexture( m_driver ),
			position_,
			objModule->Rect,
			finalOrigin,
			true,
			TintColor,
			SpriteEffects
//!!!TODO:  scale, flip, depth
		);

			
/*
        SpriteBatch.Draw(
            BitMaps[objModule.ImgIdx].Bitmap,
            position_,
            objModule->Rect,
            TintColor,
            Rotation,
            finalOrigin,
            Scale,
            SpriteEffects,
            Depth
            );
*/
    }
}


bool Sprite::SaveToFile( const std::string& sFilePath_)
{
    XMLDocument docTileMap;
	bool loadOkay = ( docTileMap.LoadFile( sFilePath_.c_str() ) == XML_NO_ERROR );

	if ( !loadOkay )
	{
		printf( "Could not load sprite file '%s'. Error='%s'. Exiting.\n", sFilePath_.c_str(), docTileMap.GetErrorStr1() );
		return false;
	}

	return true;
}


static void _LoadFromFile_ProcessBitmaps( Sprite * objSprite, XMLElement * xmlElem )
{
	
	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("BitmapFilePath", xmlElem->Value()) )
		{
			std::string bitmapRelativePath = xmlElem->FirstChild()->Value();

			// format path
			std::string bitmapCombinedPath = TextUtil::CombinePath( objSprite->RootPath, bitmapRelativePath );

			objSprite->BitMaps.push_back( new BitMap( bitmapCombinedPath ) );
		}
	}
}

static void _LoadFromFile_ProcessModules( Sprite * objSprite, XMLElement * xmlElem )
{
	std::string sId;
	std::string sTag;
	irr::core::recti rect;
	irr::u32 imgIdx;

	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("Id", xmlElem->Value()) )
		{
			sId = xmlElem->FirstChild()->Value();
		}
		else if( 0 == strcmp("Tag", xmlElem->Value()) )
		{
			if( xmlElem->FirstChild() )
			{
				sTag = xmlElem->FirstChild()->Value();
			}
		}
		else if( 0 == strcmp("Rect", xmlElem->Value()) )
		{
			XMLElement * xmlRectElem = xmlElem->FirstChildElement();
			int posX =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			int posY =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			int width =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			int height =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			rect.UpperLeftCorner = irr::core::vector2di(posX, posY);
			rect.LowerRightCorner = rect.UpperLeftCorner + irr::core::vector2di(width, height);
		}
		else if( 0 == strcmp("ImgIdx", xmlElem->Value()) )
		{
			imgIdx	= atoi( xmlElem->FirstChild()->Value() );
		}
	}

	objSprite->AddModule( sTag, rect, imgIdx, sId);
}

static void _LoadFromFile_ProcessMkRects( Sprite * objSprite, XMLElement * xmlElem )
{
	std::string sId;
	std::string sTag;
	irr::core::recti rect;

	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("Id", xmlElem->Value()) )
		{
			sId = xmlElem->FirstChild()->Value();
		}
		else if( 0 == strcmp("Tag", xmlElem->Value()) )
		{
			if( xmlElem->FirstChild() )
			{
				sTag = xmlElem->FirstChild()->Value();
			}
		}
		else if( 0 == strcmp("Rect", xmlElem->Value()) )
		{
			XMLElement * xmlRectElem = xmlElem->FirstChildElement();
			int posX =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			int posY =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			int width =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			int height =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			rect.UpperLeftCorner = irr::core::vector2di(posX, posY);
			rect.LowerRightCorner = rect.UpperLeftCorner + irr::core::vector2di(width, height);
		}
	}

	objSprite->AddMkRect( sTag, rect, sId);
}

static void _LoadFromFile_ProcessMkPoints( Sprite * objSprite, XMLElement * xmlElem )
{
	std::string sId;
	std::string sTag;
	irr::core::vector2di position;

	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("Id", xmlElem->Value()) )
		{
			sId = xmlElem->FirstChild()->Value();
		}
		else if( 0 == strcmp("Tag", xmlElem->Value()) )
		{
			if( xmlElem->FirstChild() )
			{
				sTag = xmlElem->FirstChild()->Value();
			}
		}
		else if( 0 == strcmp("Position", xmlElem->Value()) )
		{
			XMLElement * xmlRectElem = xmlElem->FirstChildElement();
			position.X =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();
			position.Y =  atoi( xmlRectElem->FirstChild()->Value() ); xmlRectElem = xmlRectElem->NextSiblingElement();			
		}
	}

	objSprite->AddMkPoint( sTag, position, sId);
}

static void _LoadFromFile_ProcessFrames( Sprite * objSprite, XMLElement * xmlElem )
{
	std::string sId;
	std::string sTag;

	Frame* frame = new Frame("", "");

	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("Id", xmlElem->Value()) )
		{
			sId = xmlElem->FirstChild()->Value();
			frame->Id = sId;
		}
		else if( 0 == strcmp("Tag", xmlElem->Value()) )
		{
			if( xmlElem->FirstChild() )
			{
				sTag = xmlElem->FirstChild()->Value();
				frame->Tag = sTag;
			}
		}
		else if( 0 == strcmp("FModule", xmlElem->Value()) )
		{
			FModule * fmodule = new FModule();
			for( XMLElement * xmlElemFModule = xmlElem->FirstChildElement(); xmlElemFModule; xmlElemFModule = xmlElemFModule->NextSiblingElement()  )
			{
				if( 0 == strcmp("Id", xmlElemFModule->Value()) )
				{
					fmodule->Id = xmlElemFModule->FirstChild()->Value();
				}
				else if( 0 == strcmp("Offset", xmlElemFModule->Value()) )
				{
					XMLElement * xmlPosElem = xmlElemFModule->FirstChildElement();
					
					fmodule->Offset.X =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();
					fmodule->Offset.Y =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();			
				}
				else if( 0 == strcmp("Flags", xmlElemFModule->Value()) )
				{
					fmodule->Flags = atoi(xmlElemFModule->FirstChild()->Value());
				}
			}
			frame->Modules.push_back( fmodule );
		}
		else if( 0 == strcmp("FMkRect", xmlElem->Value()) )
		{
			FMkRect * fmkrect = new FMkRect();
			for( XMLElement * xmlElemFModule = xmlElem->FirstChildElement(); xmlElemFModule; xmlElemFModule = xmlElemFModule->NextSiblingElement()  )
			{
				if( 0 == strcmp("Id", xmlElemFModule->Value()) )
				{
					fmkrect->Id = xmlElemFModule->FirstChild()->Value();
				}
				else if( 0 == strcmp("Offset", xmlElemFModule->Value()) )
				{
					XMLElement * xmlPosElem = xmlElemFModule->FirstChildElement();
					
					fmkrect->Offset.X =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();
					fmkrect->Offset.Y =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();			
				}
			}
			frame->MkRects.push_back( fmkrect );
		}
		else if( 0 == strcmp("FMkPoint", xmlElem->Value()) )
		{
			FMkPoint * fmkpoint = new FMkPoint();
			for( XMLElement * xmlElemFModule = xmlElem->FirstChildElement(); xmlElemFModule; xmlElemFModule = xmlElemFModule->NextSiblingElement()  )
			{
				if( 0 == strcmp("Id", xmlElemFModule->Value()) )
				{
					fmkpoint->Id = xmlElemFModule->FirstChild()->Value();
				}
				else if( 0 == strcmp("Offset", xmlElemFModule->Value()) )
				{
					XMLElement * xmlPosElem = xmlElemFModule->FirstChildElement();
					
					fmkpoint->Offset.X =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();
					fmkpoint->Offset.Y =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();			
				}
			}
			frame->MkPoints.push_back( fmkpoint );
		}
	}

	objSprite->AddFrame( frame );
}

static void _LoadFromFile_ProcessAnimations( Sprite * objSprite, XMLElement * xmlElem )
{
	std::string sId;
	std::string sTag;

	Animation* animation = new Animation("", "");

	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("Id", xmlElem->Value()) )
		{
			sId = xmlElem->FirstChild()->Value();
			animation->Id = sId;
		}
		else if( 0 == strcmp("Tag", xmlElem->Value()) )
		{
			if( xmlElem->FirstChild() )
			{
				sTag = xmlElem->FirstChild()->Value();
				animation->Tag = sTag;
			}
		}
		else if( 0 == strcmp("AFrames", xmlElem->Value()) )
		{
			AFrame * aframe = new AFrame();
			for( XMLElement * xmlElemFModule = xmlElem->FirstChildElement(); xmlElemFModule; xmlElemFModule = xmlElemFModule->NextSiblingElement()  )
			{
				if( 0 == strcmp("Id", xmlElemFModule->Value()) )
				{
					aframe->Id = xmlElemFModule->FirstChild()->Value();
				}
				else if( 0 == strcmp("Offset", xmlElemFModule->Value()) )
				{
					XMLElement * xmlPosElem = xmlElemFModule->FirstChildElement();
					
					aframe->Offset.X =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();
					aframe->Offset.Y =  atoi( xmlPosElem->FirstChild()->Value() ); xmlPosElem = xmlPosElem->NextSiblingElement();			
				}
				else if( 0 == strcmp("TimeMs", xmlElemFModule->Value()) )
				{
					aframe->TimeMs = atoi(xmlElemFModule->FirstChild()->Value());
				}
			}
			animation->AFrames.push_back( aframe );
		}
	}

	objSprite->AddAnimation( animation );
}

Sprite* Sprite::LoadFromFile( const std::string& sFilePath_, irr::video::IVideoDriver* driver_)
{

    XMLDocument docTileMap;
	bool loadOkay = ( docTileMap.LoadFile( sFilePath_.c_str() ) == XML_NO_ERROR );

	if ( !loadOkay )
	{
		printf( "Could not load sprite file '%s'. Error='%s'. Exiting.\n", sFilePath_.c_str(), docTileMap.GetErrorStr1() );
		return NULL;
	}

    
	Sprite * objSprite = new Sprite( driver_ );
    

    // Root
	objSprite->RootPath = TextUtil::GetPath( sFilePath_ );

	// Get Sprite element
	XMLElement * xmlElem = docTileMap.FirstChildElement();

	for( xmlElem = xmlElem->FirstChildElement(); xmlElem; xmlElem = xmlElem->NextSiblingElement()  )
	{
		if( 0 == strcmp("Bitmaps", xmlElem->Value()) )
		{
			_LoadFromFile_ProcessBitmaps( objSprite, xmlElem );
		}
		else if( 0 == strcmp("Modules", xmlElem->Value()) )
		{
			_LoadFromFile_ProcessModules( objSprite, xmlElem );
		}
		else if( 0 == strcmp("MkRects", xmlElem->Value()) )
		{
			_LoadFromFile_ProcessMkRects( objSprite, xmlElem );
		}
		else if( 0 == strcmp("MkPoints", xmlElem->Value()) )
		{
			_LoadFromFile_ProcessMkPoints( objSprite, xmlElem );
		}
		else if( 0 == strcmp("Frames", xmlElem->Value()) )
		{
			_LoadFromFile_ProcessFrames( objSprite, xmlElem );
		}
		else if( 0 == strcmp("Animations", xmlElem->Value()) )
		{
			_LoadFromFile_ProcessAnimations( objSprite, xmlElem );
		}

	}

    return objSprite;
}