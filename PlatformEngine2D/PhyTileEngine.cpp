#include "PhyTileEngine.h"


void PhyTileEngine::_DoPhysics( PhyObject* phyObject )
{
    // Do Rect Collisions
	if (phyObject->GetType() ==  ePhyObjectType::PHY_OBJECT_TYPE_RECT)
    {
        _DoPhysics_Rect(phyObject);
    }
}

void PhyTileEngine::_DoPhysics_Rect(PhyObject* phyObject)
{
	core::vector2df vDist = (phyObject->Position - phyObject->PrevPosition);
	float fTotalDist = vDist.getLength();

//Return: no displacement done
    if (fTotalDist == 0) return;

    core::vector2df vDir = vDist;
	vDir = vDir.normalize();

    core::vector2df vLastFreePosition = phyObject->Position;

    // Entity will be moved 1 by 1 from previous to final position, changing previous each step
    // Each collision reset velocity X or Y depending of blocked axis, when no velocity at all, stop moving aswell
	for (float fCurrDist = 0; (fCurrDist < fTotalDist) && (phyObject->Velocity.getLengthSQ() != 0); fCurrDist++)
    {

        // Advance previous position one unit facing direction ( exept last step to avoid go  over final position )
        if (fCurrDist >= (fTotalDist - 1))
        {
            vDir *= (fTotalDist - fCurrDist);
        }

        // Entity position is updated here
        _DoBlockPhysics( phyObject, vDir );

        // Once entity is placed in a free of block collision area, floating physic is applied
		_DoFloatingPhysics(phyObject, vDir);

    }// for

    // set Position as previous ( that now is the last free position found )
    phyObject->Position = phyObject->PrevPosition;

}

void PhyTileEngine::_DoBlockPhysics(PhyObject* phyObject, core::vector2df vDir_ )
{
    // Set current position as the last free position
	core::vector2df vLastFreePosition = phyObject->PrevPosition;


    phyObject->PrevPosition += vDir_;


    u32 collRegister = _GetCollisionAt(phyObject, phyObject->PrevPosition, vDir_);

    // Check if there is a collision
    if (collRegister != eDirFlag::NONE)
    {
        int slopeDist = 0;

        if( 
			(phyObject->Velocity.Y >= 0) // if falling or moving horizontally
			//( phyObject->IsOnPlatform ) // and is on platform ( optional )
		)
        {
            if (
				(vDir_.X != 0) && 
				( collRegister & (eDirFlag::LEFT | eDirFlag::RIGHT))
			)
            {
                // Try go up first ( it could be a slope )
                slopeDist = _GetFreeVertAreaHeight(phyObject, phyObject->PrevPosition, phyObject->GndSlopeHigh, Color_Block);
            }
        }

        if (slopeDist > 0)
        // ! High is lower enough to allow entity to go up slope
        {

            // Temporary set last free position as prev. to make easy access to Y attribute
            vLastFreePosition = phyObject->PrevPosition;

            // Increase Y with high distance
            vLastFreePosition.Y -= slopeDist;

            //Snap Y position
			vLastFreePosition.Y = ceilf(vLastFreePosition.Y);

            if (vDir_.Y > 0)
            {
                // Reset vertical direction ( set horizontal only )
				vDir_ = vDir_.X > 0 ? core::vector2df(1, 0) : core::vector2df(-1, 0);

                // Reset entity Y velocity
				phyObject->Velocity.Y = 0;

				// Set collision flags
                phyObject->PhyCollisionFlags |= eDirFlag::BOTTOM;
            }
        }
        else
        {
            //
            // +++ lock collided direction +++
            //


            // Snap only axis that produced collision
            bool bSnapX = (vDir_.X != 0);
            bool bSnapY = (vDir_.Y != 0);

            // In case we have a collision when moving on X and Y. We check if a moving on one axis is possible
            if (bSnapX && bSnapY)
            {
				core::vector2df vDirX = core::vector2df(vDir_.X, 0);
                core::vector2df vDirY = core::vector2df(0, vDir_.Y);

                u32 collRegX = _GetCollisionAt(phyObject, vLastFreePosition + vDirX, vDirX);
                if (collRegX == eDirFlag::NONE)
                {
                    // X axis movement allowed so don't snap
                    bSnapX = false;

                    // set X velocity only
                    vDir_ = vDirX;

                    // move last free position
                    vLastFreePosition += vDir_;

                }
                else
                {
                    u32 collRegY = _GetCollisionAt(phyObject, vLastFreePosition + vDirY, vDirY);
                    if (collRegY == eDirFlag::NONE)
                    {
                        // Y axis movement allowed so don't snap
                        bSnapY = false;

                        // set Y velocity only
                        vDir_ = vDirY;

                        // move last free position
                        vLastFreePosition += vDir_;

                    }
                }
            }


            // Snap coordinates
            if (bSnapX)
            {
                // Reset X velocity, because it was blocked in that direction
				phyObject->Velocity.X = 0;

                // Set collision flags
                phyObject->PhyCollisionFlags |= (vDir_.X > 0) ? eDirFlag::RIGHT : eDirFlag::LEFT;

                // Snap X
                if ( collRegister & eDirFlag::LEFT )
                {
					vLastFreePosition.X = floorf(vLastFreePosition.X);
                }
                else
                {
					vLastFreePosition.X = ceilf(vLastFreePosition.X);
                }
            }
            if (bSnapY)
            {
                // Reset Y velocity, because it was blocked in that direction
				phyObject->Velocity.Y = 0;

                // Set collision flags
                phyObject->PhyCollisionFlags |= (vDir_.Y > 0) ? eDirFlag::BOTTOM : eDirFlag::TOP;

                // Snap Y
                if ( collRegister & eDirFlag::TOP )
                {
                    vLastFreePosition.Y = floorf(vLastFreePosition.Y);
                }
                else
                {
                    vLastFreePosition.Y = ceilf(vLastFreePosition.Y);
                }
            }
        }

        // If there was a collision, set last free position
        phyObject->PrevPosition = vLastFreePosition;
    }
}

/// <summary>
/// This just move player over floating block using slope distance
/// </summary>
/// <param name="iPhyEntity_"></param>
/// <param name="vDir_"></param>
void PhyTileEngine::_DoFloatingPhysics(PhyObject* phyObject, core::vector2df vDir_)
{

    if (phyObject->Velocity.Y >= 0)
    {

		core::vector2df vFinalPosition = phyObject->PrevPosition;

        // Check Botton Line Collision
		core::recti rectColl = phyObject->BoundingBox;
		rectColl.UpperLeftCorner.Y = rectColl.LowerRightCorner.Y - 1;

        // Check if there is a collision
        if (_IsRectCollisionAt(rectColl, phyObject->PrevPosition, Color_Floating))
        {
			if( !phyObject->SkipFloatingPlatformCheck )
			{
				int slopeDist = _GetFreeVertAreaHeight(phyObject, phyObject->PrevPosition, phyObject->GndSlopeHigh, Color_Floating, eDirFlag::BOTTOM);

				if (slopeDist > 0)
				// ! High is lower enough to allow entity to go up slope
				{

					// Increase Y with high distance
					vFinalPosition.Y -= slopeDist;

					//Snap Y position
					vFinalPosition.Y = ceilf(vFinalPosition.Y);

					if (false == _IsRectCollisionAt(phyObject->BoundingBox, vFinalPosition, Color_Block))
					{
						if (vDir_.Y > 0)
						{
							// Reset vertical direction ( set horizontal only )
							vDir_ = vDir_.X > 0 ? core::vector2df(1, 0) : core::vector2df(-1, 0);

							// Reset entity Y velocity
							phyObject->Velocity.Y = 0;
						}
					}
					else
					{
						// go back final position
						vFinalPosition = phyObject->PrevPosition;
					}
				}

				// Set final position
				phyObject->PrevPosition = vFinalPosition;
			}
        }
		else
		{
			phyObject->SkipFloatingPlatformCheck = false;
		}

    }
}

// Used for climbing slopes. Check for a free position using a max distance
int PhyTileEngine::_GetFreeVertAreaHeight( PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_)
{
    return _GetFreeVertAreaHeight( phyObject, vPosition_, maxDist_, blockType_, eDirFlag::ALL );
}

int PhyTileEngine::_GetFreeVertAreaHeight( PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_, eDirFlag collFlags_ )
{
    if (maxDist_ <= 0) return -1;

    for (int dist = 1; dist <= maxDist_; dist++)
    {
		core::vector2df vCurrPos = vPosition_;
        vCurrPos.Y -= dist;

        if (_IsRectBorderCollisionAt(phyObject, vCurrPos, blockType_, collFlags_))
        {
            continue;
        }

//return> return distance if no collision at current point
        return dist;
    }

    return -1;
}

bool PhyTileEngine::_GetIsOnPlatform( PhyObject* phyObject )
{
    bool retIsOnPlatform = false;

	// check if integer
    if (static_cast<s32>(phyObject->Position.Y) == phyObject->Position.Y)
    {
        // Check Botton Line Collision
        core::recti rectColl = phyObject->BoundingBox;
		rectColl.UpperLeftCorner.Y = rectColl.LowerRightCorner.Y - 1;

		core::vector2df vPosToCheck = phyObject->Position + core::vector2df( 0, 1);

        retIsOnPlatform = 
            _IsRectCollisionAt(rectColl, vPosToCheck, Color_Block) || 
            !phyObject->SkipFloatingPlatformCheck && _IsRectCollisionAt(rectColl, vPosToCheck, Color_Floating);
                                
    }

    return retIsOnPlatform;
}


u32 PhyTileEngine::_GetDistToGround(PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_)
{
    return _GetDistToGround(phyObject, vPosition_, maxDist_, blockType_, eDirFlag::ALL);
}

u32 PhyTileEngine::_GetDistToGround(PhyObject* phyObject, core::vector2df vPosition_, s32 maxDist_, video::SColor blockType_, eDirFlag collFlags_)
{
    if (maxDist_ <= 0) return 0xFFFFFFFF;

    for (int dist = 1; dist <= maxDist_; dist++)
    {
        core::vector2df vCurrPos = vPosition_;
        vCurrPos.Y += dist;

        if (false == _IsRectBorderCollisionAt(phyObject, vCurrPos, blockType_))
        {
            continue;
        }

        //return> return distance if no collision at current point
        return dist;
    }

    return 0xFFFFFFFF;
}

// Check collision using border lines of rectangle reparately
bool PhyTileEngine::_IsRectBorderCollisionAt(PhyObject* phyObject, core::vector2df vPosition_, video::SColor blockType_)
{
    return _IsRectBorderCollisionAt(phyObject, vPosition_, blockType_, eDirFlag::ALL);
}

bool PhyTileEngine::_IsRectBorderCollisionAt(PhyObject* phyObject, core::vector2df vPosition_, video::SColor blockType_, eDirFlag collFlags_)
{

		core::recti rectColl;

        if ( collFlags_ & eDirFlag::TOP )
        {
            // Check Top Line collision
            rectColl = phyObject->BoundingBox;
			rectColl.LowerRightCorner.Y = rectColl.UpperLeftCorner.Y + 1; //set height 1

            if (_IsRectCollisionAt(rectColl, vPosition_, blockType_))
            {
//return> true
                return true;
            }
        }

        if ( collFlags_ & eDirFlag::BOTTOM )
        {
            // Check Botton Line Collision
            rectColl = phyObject->BoundingBox;
			rectColl.UpperLeftCorner.Y = rectColl.LowerRightCorner.Y - 1;

            if (_IsRectCollisionAt(rectColl, vPosition_, blockType_))
            {
//return> true
                return true;
            }

        }

        if ( collFlags_ & eDirFlag::RIGHT )
        {
            // Check Right Line collision
            rectColl = phyObject->BoundingBox;
			rectColl.UpperLeftCorner.X = rectColl.LowerRightCorner.X - 1;

            if (_IsRectCollisionAt(rectColl, vPosition_, blockType_))
            {
//return> true
                return true;
            }
        }

        if ( collFlags_ & eDirFlag::LEFT )
        {
            // Check Left Line Collision
            rectColl = phyObject->BoundingBox;
            
			rectColl.LowerRightCorner.X = rectColl.UpperLeftCorner.X + 1;

            if (_IsRectCollisionAt(rectColl, vPosition_, blockType_))
            {
//return> true
                return true;
            }
        }

//return> false
        return false;
}

bool PhyTileEngine::_IsRectCollisionAt(core::recti rect_, core::vector2df vPosition_, video::SColor blockType_)
{
    core::recti rectColl = rect_;

    // Set Line Position
	rectColl += core::vector2di( static_cast<int>( vPosition_.X ), static_cast<int>( vPosition_.Y ) );

    // Increase rectangle if doesn't fit pixel grid ( when coord is not an integer )
    if (static_cast<int>( vPosition_.X ) != vPosition_.X) { ++rectColl.LowerRightCorner.X;} // increase X if checking Right line
	if (static_cast<int>( vPosition_.Y ) != vPosition_.Y) { ++rectColl.LowerRightCorner.Y;} // same here

    // Get intersection with physic layer image to be in bounds
	//rectColl.clipAgainst( m_physicLayerTile->GetBounds() );
	rectColl.UpperLeftCorner.X =  core::max_( rectColl.UpperLeftCorner.X, m_physicLayerTile->GetBounds().UpperLeftCorner.X);
	rectColl.LowerRightCorner.X =  core::min_( rectColl.LowerRightCorner.X, m_physicLayerTile->GetBounds().LowerRightCorner.X);
	rectColl.UpperLeftCorner.Y =  core::max_( rectColl.UpperLeftCorner.Y, m_physicLayerTile->GetBounds().UpperLeftCorner.Y);
	rectColl.LowerRightCorner.Y =  core::min_( rectColl.LowerRightCorner.Y, m_physicLayerTile->GetBounds().LowerRightCorner.Y);

    if (
		rectColl.isValid()
    )
    {
        return IsRectCollidesPhyTileLayer(rectColl, blockType_);
    }

    return false;
}

bool PhyTileEngine::IsRectCollidesPhyTileLayer(core::recti rect_, video::SColor blockType_)
{
	int topLeftTileX = rect_.UpperLeftCorner.X / m_tilemap->GetTileWidth();
	int topLeftTileY = rect_.UpperLeftCorner.Y / m_tilemap->GetTileHeight();
	int bottomRightTileX = (rect_.LowerRightCorner.X - 1) / m_tilemap->GetTileWidth();
    int bottomRightTileY = (rect_.LowerRightCorner.Y - 1) / m_tilemap->GetTileHeight();

    for (int tileX = topLeftTileX; tileX <= bottomRightTileX; tileX++)
    {
        for (int tileY = topLeftTileY; tileY <= bottomRightTileY; tileY++)
        {
            if (IsRectCollidesTile(rect_, blockType_, tileX, tileY))
            {
                return true;
            }
        }
    }

    return false;
}

bool PhyTileEngine::IsRectCollidesTile(core::recti rect_, video::SColor blockType_, int tileX, int tileY)
{
	const LayerTile* layerTile = m_physicLayerTile->GetLayerTile(tileX, tileY);

	if (layerTile->GetTilesetId() >= 0)
    {
		Tileset* tileset = m_tilemap->GetTileset( layerTile->GetTilesetId() );
		video::ITexture* tilesetText = tileset->GetTexture( m_driver );

		// Get intersection with physic layer image to be in bounds
		core::recti srcRect = rect_;
		core::recti tileRect( tileX * m_tilemap->GetTileWidth(), tileY * m_tilemap->GetTileHeight(), (tileX + 1) * m_tilemap->GetTileWidth(), (tileY + 1) * m_tilemap->GetTileHeight() );
		//srcRect.clipAgainst(tileRect);
		srcRect.UpperLeftCorner.X =  core::max_( rect_.UpperLeftCorner.X, tileRect.UpperLeftCorner.X);
		srcRect.LowerRightCorner.X =  core::min_( rect_.LowerRightCorner.X, tileRect.LowerRightCorner.X);
		srcRect.UpperLeftCorner.Y =  core::max_( rect_.UpperLeftCorner.Y, tileRect.UpperLeftCorner.Y);
		srcRect.LowerRightCorner.Y =  core::min_( rect_.LowerRightCorner.Y, tileRect.LowerRightCorner.Y);

        // move to tileset coords
		s32 rectWidth = srcRect.getWidth();
		s32 rectHeight = srcRect.getHeight();
		srcRect.UpperLeftCorner.X = (srcRect.UpperLeftCorner.X % m_tilemap->GetTileWidth()) + tileset->GetTileX(layerTile->GetTileId()) * m_tilemap->GetTileWidth();
		srcRect.UpperLeftCorner.Y = (srcRect.UpperLeftCorner.Y % m_tilemap->GetTileHeight()) + tileset->GetTileY(layerTile->GetTileId()) * m_tilemap->GetTileHeight();

		srcRect.LowerRightCorner.X = srcRect.UpperLeftCorner.X + rectWidth;
		srcRect.LowerRightCorner.Y = srcRect.UpperLeftCorner.Y + rectHeight;

		u8* pixelData = static_cast<u8 *>( tilesetText->lock(true) );
		core::dimension2du textSize = tilesetText->getSize();
		u32 pixelSize = tilesetText->getPitch() / textSize.Width;
		
		bool isCollision = false;

		//!!!NOTE: suppose format video::ECOLOR_FORMAT::ECF_A8R8G8B8
		for( s32 x = srcRect.UpperLeftCorner.X; x < srcRect.LowerRightCorner.X && !isCollision; ++x )
		for( s32 y = srcRect.UpperLeftCorner.Y; y < srcRect.LowerRightCorner.Y && !isCollision; ++y )
		{
			s32 index = y * tilesetText->getPitch() + x * pixelSize;

			video::SColor color( *((u32*)(pixelData + index)) );

			if( color == blockType_ )
			{
				isCollision = true;
			}
		}

		tilesetText->unlock();
        
        return isCollision;
    }

    return false;
}

/// <summary>
/// Check if there is a collision at specified position using direction for improving.
/// In order to work, entity should be moving from a free area in the specified collision. Dont's use 
/// </summary>
/// <param name="iPhyEntity_"></param>
/// <param name="vPosition_"></param>
/// <param name="vDir_">Precalculated direction of movement for improving performance</param>
/// <returns></returns>
u32 PhyTileEngine::_GetCollisionAt(PhyObject* phyObject, core::vector2df vPosition_, core::vector2df vDir_)
{
	core::recti collBox = phyObject->BoundingBox;
    // ! Get horizontal & vertical lines pointing direction of movement or null ( offset -1) if axis movement is 0 

    // Get horizontal line
    int nHorLineOffset_Y;
    bool isCheckHorLine = false;
    core::recti rectHorLine = phyObject->BoundingBox;
	rectHorLine.LowerRightCorner.Y = rectHorLine.UpperLeftCorner.Y + 1;
    if (vDir_.Y != 0)
    {
        // Set Line Position
        nHorLineOffset_Y = (vDir_.Y > 0) ? collBox.getHeight() - 1 : 0;
        float fTempPosY = vPosition_.Y + nHorLineOffset_Y;

		// move rect to position
		rectHorLine += core::vector2di( static_cast<s32>(vPosition_.X), static_cast<s32>(fTempPosY) );

        // Increase rectangle if doen't fit pixel grid ( when coord is not an integer )
		if (static_cast<s32>(vPosition_.X) != vPosition_.X) { ++rectHorLine.LowerRightCorner.X; } // increse width
        if (static_cast<s32>(vPosition_.Y) != vPosition_.Y) { if (vDir_.Y > 0) rectHorLine += core::vector2di(0, 1); } // increase Y if checking botton line

		// Get intersection with physic layer image to be in bounds
		//rectHorLine.clipAgainst(m_physicLayerTile->GetBounds()); //commented because is not working as I thought ( when rectangle is outside on top )
		rectHorLine.UpperLeftCorner.X =  core::max_( rectHorLine.UpperLeftCorner.X, m_physicLayerTile->GetBounds().UpperLeftCorner.X);
		rectHorLine.LowerRightCorner.X =  core::min_( rectHorLine.LowerRightCorner.X, m_physicLayerTile->GetBounds().LowerRightCorner.X);
		rectHorLine.UpperLeftCorner.Y =  core::max_( rectHorLine.UpperLeftCorner.Y, m_physicLayerTile->GetBounds().UpperLeftCorner.Y);
		rectHorLine.LowerRightCorner.Y =  core::min_( rectHorLine.LowerRightCorner.Y, m_physicLayerTile->GetBounds().LowerRightCorner.Y);

        if (
			rectHorLine.isValid() &&
			rectHorLine.getWidth() > 0
		)
        {
            isCheckHorLine = true;
        }
    }
    else
    {
        nHorLineOffset_Y = -1;
    }

    // Get vertical line
    int nVertLineOffset_X;
    bool isCheckVertLine = false;
	core::recti rectVertLine = phyObject->BoundingBox;
	rectVertLine.LowerRightCorner.X = rectVertLine.UpperLeftCorner.X + 1;
    if (vDir_.X != 0)
    {
        // Set Line Position
        nVertLineOffset_X = (vDir_.X > 0) ? collBox.getWidth() - 1 : 0;
        float fTempPosX = vPosition_.X + nVertLineOffset_X;

		rectVertLine += core::vector2di( static_cast<s32>(fTempPosX), static_cast<s32>(vPosition_.Y) );

        // Increase rectangle if doen't fit pixel grid ( when coord is not an integer )
		if (static_cast<s32>(vPosition_.X) != vPosition_.X) { if (vDir_.X > 0) rectVertLine += core::vector2di( 1, 0 ); } // increase X if checking Right line
		if (static_cast<s32>(vPosition_.Y) != vPosition_.Y) { ++rectVertLine.LowerRightCorner.Y; }

        // Get intersection with physic layer image to be in bounds
		//rectVertLine.clipAgainst(m_physicLayerTile->GetBounds());
		rectVertLine.UpperLeftCorner.X =  core::max_( rectVertLine.UpperLeftCorner.X, m_physicLayerTile->GetBounds().UpperLeftCorner.X);
		rectVertLine.LowerRightCorner.X =  core::min_( rectVertLine.LowerRightCorner.X, m_physicLayerTile->GetBounds().LowerRightCorner.X);
		rectVertLine.UpperLeftCorner.Y =  core::max_( rectVertLine.UpperLeftCorner.Y, m_physicLayerTile->GetBounds().UpperLeftCorner.Y);
		rectVertLine.LowerRightCorner.Y =  core::min_( rectVertLine.LowerRightCorner.Y, m_physicLayerTile->GetBounds().LowerRightCorner.Y);

       if (
			rectVertLine.isValid() &&
			rectVertLine.getHeight() > 0
		)
        {
            isCheckVertLine = true;
        }
    }
    else
    {
        nVertLineOffset_X = -1;
    }

    // !!!
    // !!! Now aVertLineColor and aHorLineColor are not null if should be checked
    // !!! 

    u32 collRegister = eDirFlag::NONE;

    // Check horizontal Line
    if (isCheckHorLine)
    {
        if (IsRectCollidesPhyTileLayer(rectHorLine, Color_Block))
        {
            collRegister |= (vDir_.Y > 0) ? eDirFlag::BOTTOM : eDirFlag::TOP;
        }
    }

    // Check vertical Line
    if (isCheckVertLine)
    {
        if (IsRectCollidesPhyTileLayer(rectVertLine, Color_Block))
        {
            collRegister |= (vDir_.X > 0) ? eDirFlag::RIGHT : eDirFlag::LEFT;
        }
    }

    return collRegister;
}

void PhyTileEngine::_ApplyMagneticDistance( PhyObject* phyObject )
{
    core::vector2df vTempPos = phyObject->Position;
    int iDist = _GetDistToGround( phyObject, phyObject->Position, phyObject->GndMagneticOffset, Color_Block, eDirFlag::BOTTOM);
    if ( (iDist <= phyObject->GndMagneticOffset ) && (iDist > 1))
    {
        vTempPos.Y += (iDist - 1);
        phyObject->IsOnPlatform = true;
    }
    else if( !phyObject->SkipFloatingPlatformCheck )
    {
        iDist = _GetDistToGround(phyObject, phyObject->Position, phyObject->GndMagneticOffset, Color_Floating, eDirFlag::BOTTOM);
        if ((iDist <= phyObject->GndMagneticOffset) && (iDist > 1) )
        {
            vTempPos.Y += (iDist - 1);
            phyObject->IsOnPlatform = true;
        }
    }

    // Set updated position
    phyObject->Position = vTempPos;
}