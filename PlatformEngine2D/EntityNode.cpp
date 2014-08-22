#include "EntityNode.h"

#include <irrlicht.h>
#include "PhyObject.h"
#include "IGameManager.h"
#include "IStateMachine.h"
#include "DebugMgr.h"
#include "MathUtil.h"


bool EntityNode::s_isDebugPhyCollRect = false;

EntityNode::EntityNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IGameManager* gameMgr) 
	: scene::ISceneNode(parent, mgr, id), 
	LastAnimationTime(0),
	m_spriteComponent( 0 ) ,
	m_gameMgr(gameMgr),
	m_phyObjectId(-1),
	m_stateMachine(0)
{
	Material.Wireframe = false;
	Material.Lighting = false;

	setAutomaticCulling( irr::scene::EAC_OFF );
}

EntityNode::~EntityNode()
{
 	DBG(" <~EntityNode> id: " << getID() << " name: " << getName() << " PhyObjId: " << m_phyObjectId );
	
	// Release sprite render
	if( m_spriteComponent )
	{
		delete m_spriteComponent;
	}

	if( m_stateMachine )
	{
		delete m_stateMachine;
	}

	if( m_phyObjectId >= 0 )
	{
		//DBG( " <~EntityNode>      Removing PhyObjId: " << m_phyObjectId );
		if( m_gameMgr->GetPhyEngine() ) // could be null when unloading a room
		{
			m_gameMgr->GetPhyEngine()->RemovePhyObject( m_phyObjectId );
		}
	}

	while( m_vEntCollBodies.size() > 0 )
	{
		m_gameMgr->GetEntityCollisionMgr()->RemoveCollisionBody( m_vEntCollBodies.back() );
		m_vEntCollBodies.pop_back();
	}
}

void EntityNode::DestroyEntity()
{ 
	GetGameManager()->GetEntityManager()->DestroyEntity( this ); 
}

void EntityNode::OnRegisterSceneNode()
{
	if (IsVisible)
		// use ESNRP_CAMERA to allow draw nodes in the same order of parent children list
		SceneManager->registerNodeForRendering(this, scene::E_SCENE_NODE_RENDER_PASS::ESNRP_CAMERA);

	ISceneNode::OnRegisterSceneNode();
}

void EntityNode::OnAnimate(u32 timeMs)
{

	// Call base
	ISceneNode::OnAnimate( timeMs );

	// !!! Note using game manager frameDt
	s32 frameDt = m_gameMgr->GetFrameDt();

	u32 timeDt = timeMs - LastAnimationTime;
	if( m_spriteComponent )
	{
		// !!! Note using game manager frameDt
		m_spriteComponent->Update( frameDt );
	}

	// Update Physic Object
	if( m_phyObjectId >= 0)
	{
		PhyObject* phyObject = m_gameMgr->GetPhyEngine()->GetPhyObject( m_phyObjectId );

		if( phyObject )
		{
			this->setPosition( irr::core::vector3df(phyObject->Position.X, phyObject->Position.Y, getPosition().Z) );
		}

		//fastFix> because updateAbsolutePosition is called by irrlicht before call OnAnimate AbsoluteTransformation is updated with previous position
		updateAbsolutePosition();
	}

	// Update Entity State Machine
	if( m_stateMachine )
	{

		// NOTE: send Update msg after update state machine can make player go from walking to idle and the falling with no velocity (idle reset v.x)

		// Set State Update
		m_stateMachine->SendMsg( eCommonMessageTypes::STATE_MESSAGE_UPDATE );

		// Change state if needed and sebd Enter for new and Exit for previous
		m_stateMachine->Update();
	}
}

void EntityNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ISceneNode* cameraNode = SceneManager->getActiveCamera();
	
	core::matrix4 mxTrans = cameraNode->getAbsoluteTransformation();
	core::vector3df vCamPos = cameraNode->getPosition();
	core::vector3df vCamScale = cameraNode->getScale();
	core::vector3df vCamRot = cameraNode->getRotation();

	mxTrans = 
		core::matrix4().setTranslation(vCamPos)*
		core::matrix4().setScale(vCamScale)*
		core::matrix4().setTranslation(-vCamPos)*
		core::matrix4().setRotationDegrees(vCamRot)
		;

	driver->setMaterial(Material);


	driver->setTransform(video::ETS_WORLD, mxTrans*AbsoluteTransformation);

	if( m_spriteComponent )
	{
		m_spriteComponent->Draw( core::vector2df(0, 0) );
	}
	

	// Draw Phy Collision Rect
	if( s_isDebugPhyCollRect && ( m_phyObjectId >= 0 ) )
	{
		PhyObject* phyObject = m_gameMgr->GetPhyEngine()->GetPhyObject( m_phyObjectId );

		if( phyObject )
		{
			DebugMgr::DrawWorldRect( m_gameMgr, phyObject->BoundingBox, video::SColor(32, 255, 255, 0), AbsoluteTransformation);
			DebugMgr::DrawWorldRect( m_gameMgr, core::recti(0, 0, 1, 1), video::SColor(32, 255, 255, 255), AbsoluteTransformation);
		}
	}
	//---

	// Draw Entity Collision Rect
	if( s_isDebugPhyCollRect  )
	{
		std::vector<u16>::iterator iter_end( m_vEntCollBodies.end() );
		for( std::vector<u16>::iterator iter( m_vEntCollBodies.begin() ); iter != iter_end; ++iter )
		{
			EntityCollisionBody* entityCollBody = m_gameMgr->GetEntityCollisionMgr()->GetById( *iter );

			video::SColor color = GetGameManager()->GetEntityCollisionMgr()->IsCollision( *iter )? video::SColor(128, 0, 255, 0) : video::SColor(128, 0, 128, 0);

			if( entityCollBody->IsEnabled )
			{
				core::matrix4 mxDrawMatrix;
				mxDrawMatrix.setTranslation( RelativeTranslation );
				DebugMgr::DrawWorldRect( m_gameMgr, MathUtil::RectF2I( entityCollBody->CollData ), color, mxDrawMatrix);
			}
		}
	}
	//---
}

const core::aabbox3d<f32>& EntityNode::getBoundingBox() const
{
	return Box;
}

u32 EntityNode::getMaterialCount() const
{
	return 1;
}

video::SMaterial& EntityNode::getMaterial(u32 i)
{
	return Material;
}	

void EntityNode::SetSpriteComponent( const std::string& spritePath )
{
	if( m_spriteComponent ) delete m_spriteComponent;

	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	m_spriteComponent = new SpriteComponent( driver );
	m_spriteComponent->SetSprite( spritePath );
}

PhyObject* EntityNode::GetPhysicObject()
{ 
	return m_gameMgr->GetPhyEngine()->GetPhyObject( m_phyObjectId ); 
}

void EntityNode::CreatePhysicObject()
{ 
	m_phyObjectId = m_gameMgr->GetPhyEngine()->CreatePhyObject( ePhyObjectType::PHY_OBJECT_TYPE_RECT); 
}

u16 EntityNode::AddEntityCollisionBody( const std::string& name, core::rectf collRect, u32 collGroup, u32 flags, TEntCollCB collCBFunc )
{ 
	u16 entCollBodyId = m_gameMgr->GetEntityCollisionMgr()->AddCollisionBody( this, eEntityCollisionBodyType::COLLISION_TYPE_RECT, collGroup, collCBFunc );
	m_gameMgr->GetEntityCollisionMgr()->GetById( entCollBodyId )->CollData = collRect;
	m_gameMgr->GetEntityCollisionMgr()->GetById( entCollBodyId )->Flags = flags;
	m_vEntCollBodies.push_back( entCollBodyId );
	m_dicEntCollBodies[name] = entCollBodyId;
	return entCollBodyId;
}

bool EntityNode::RemoveCollisionBody( const std::string& name  )
{
	std::map< std::string, u16 >::iterator iter = m_dicEntCollBodies.find( name );
	if ( iter != m_dicEntCollBodies.end())
	{
		u16 id = m_dicEntCollBodies[name];
		m_dicEntCollBodies.erase( iter );
		m_gameMgr->GetEntityCollisionMgr()->RemoveCollisionBody( id );

		std::vector<u16>::iterator iter2 = std::find(m_vEntCollBodies.begin(), m_vEntCollBodies.end(), id);
		if( iter2 != m_vEntCollBodies.end() )
		{
			m_vEntCollBodies.erase( iter2 );
		}
		return true;
	}
	return false;
}

EntityCollisionBody* EntityNode::GetEntityCollisionBody( const std::string& name )
{ 
	std::map< std::string, u16 >::iterator iter = m_dicEntCollBodies.find( name );
	if ( iter != m_dicEntCollBodies.end())
	{
		u16 id = iter->second;
		return m_gameMgr->GetEntityCollisionMgr()->GetById( id ); 
	}
	return 0;
}