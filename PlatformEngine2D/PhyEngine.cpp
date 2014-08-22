#include "PhyEngine.h"



static float _ToFrameDT(float fVal, u32 frameDT)
{
    return fVal * frameDT / 1000.;
}

static core::vector2df _ToFrameDT(core::vector2df vect_, u32 frameDT)
{
    vect_.X = _ToFrameDT(vect_.X, frameDT);
    vect_.Y = _ToFrameDT(vect_.Y, frameDT);

    return vect_;
}

static float _ToFrameDT2(float fVal, u32 frameDT)
{
    return fVal * pow(frameDT / 1000., 2);
}

static  core::vector2df _ToFrameDT2(core::vector2df vect_, u32 frameDT)
{
    vect_.X = _ToFrameDT2(vect_.X, frameDT);
    vect_.Y = _ToFrameDT2(vect_.Y, frameDT);

    return vect_;
}

void PhyEngine::_UpdatePosition( PhyObject* phyObject, irr::u32 frameDT )
{
    // Keep Current Position
    phyObject->PrevPosition = phyObject->Position;

    // Update Position ( e = v0*t + (a*t^2)/2 ) )
    phyObject->Position += _ToFrameDT(phyObject->Velocity, frameDT) + _ToFrameDT2(phyObject->Acceleration, frameDT) / 2.;
}

void PhyEngine::_UpdateVelocity( PhyObject* phyObject, irr::u32 frameDT )
{
    // Update Velocity ( v = v0 + a*t )
    phyObject->Velocity += _ToFrameDT( phyObject->Acceleration, frameDT);

    // Set limits
	if (phyObject->MaxVelocity.X > 0){ phyObject->Velocity.X = core::clamp(phyObject->Velocity.X, -phyObject->MaxVelocity.X, phyObject->MaxVelocity.X);}
    if (phyObject->MaxVelocity.Y > 0){ phyObject->Velocity.Y = core::clamp(phyObject->Velocity.Y, -phyObject->MaxVelocity.Y, phyObject->MaxVelocity.Y); }

    // Reset acceleration
	phyObject->Acceleration = core::vector2df(0, 0);
}

void PhyEngine::Update( irr::u32 frameDT, bool isResetCollisionFlags )
{

	//skip zero frameDt to avoid weird bugs
	if( frameDT == 0 )
	{
//return
		return;
	}

	TPhyObjContainerVector::iterator iter( m_vPhyObjects.begin() );
	TPhyObjContainerVector::iterator iter_end( m_vPhyObjects.end() );
	while( iter != iter_end )
	{
		PhyObjectContainer* phyObjContainer = (*iter);

		// Remove marked for deletion
		if( phyObjContainer->isMarkedForDeletion )
		{
			iter = m_vPhyObjects.erase( iter );
			iter_end = m_vPhyObjects.end();
			delete phyObjContainer;
		}
		else
		{
			PhyObject* phyObject = phyObjContainer->phyObject;

			// This is used when more than one update is called and you want keep the collision flags
			if( isResetCollisionFlags )
			{
				phyObject->PhyCollisionFlags = eDirFlag::NONE;
			}

            if (
				phyObject->IsApplyGravity &&
				(false == phyObject->IsOnPlatform || phyObject->IsClipping)
			)
            {
				// Applygravity
				phyObject->ApplyAcceleration( m_gravity );
            }

			_UpdatePosition(phyObject, frameDT);

			// keep last position, this is due PrevPosition is changed during physic calculation
			core::vector2df savedPrevPosition = phyObject->PrevPosition;

			_UpdateVelocity(phyObject, frameDT);

			if( !phyObject->IsClipping )
			{
				_DoPhysics(phyObject);

				bool oldIsOnPlatform = phyObject->IsOnPlatform;

				// Check if On Platform
				// TODO: Improve this setting to true when falling down and stopped on Y, and set to false when no collision using botton line ( without  + 1 )
				// Position Y is integer and botton line + 1 is not colliding
				phyObject->IsOnPlatform = _GetIsOnPlatform( phyObject );

				// fix a bug when sometimes object is on platform but velocity.Y has a small positive value
				if( phyObject->IsOnPlatform )
				{
					phyObject->Velocity.Y = 0;
				}

				// Check ground magnetic offset for moving to ground directly
				if( 
					(phyObject->Velocity.Y >= 0) &&
					(phyObject->IsOnPlatform != oldIsOnPlatform) &&
					(false == phyObject->IsOnPlatform) &&
					!( phyObject->PhyCollisionFlags & eDirFlag::TOP ) // when roof is too close to head, this allow player to jump
				)
				{
					_ApplyMagneticDistance( phyObject );
				}
			}

			// restore last position
			phyObject->PrevPosition = savedPrevPosition;

			// next element
			++iter;
		}
	}
}

u32 PhyEngine::CreatePhyObject( ePhyObjectType type )
{
	PhyObjectContainer* phyObjContainer = new PhyObjectContainer( new PhyObject() );
	
	m_vPhyObjects.push_back( phyObjContainer );
	m_dicPhyObjects[ m_nextId ] = phyObjContainer;

	int phyObjId = m_nextId;
	++m_nextId;

	return phyObjId;
}

void PhyEngine::RemovePhyObject( u32 id )
{
	TPhyObjContainerMap::iterator iter = m_dicPhyObjects.find( id );
	if( iter != m_dicPhyObjects.end())
	{
		iter->second->isMarkedForDeletion = true;
		m_dicPhyObjects.erase( iter );
	}
}