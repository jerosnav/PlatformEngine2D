#pragma once

#include <irrlicht.h>
#include <map>
#include <string>

#include "PhyObject.h"
#include "SpriteComponent.h"
#include "IEntityStateMachine.h"
#include "EntityCollisionMgr.h"

using namespace irr;

class IGameManager;
class IEntityStateMachine;

struct EntityCollisionBody;
typedef void (*TEntCollCB)( EntityCollisionBody* thisCollBody, EntityCollisionBody* otherCollBody );

class EntityNode : public scene::ISceneNode
{

private:

	core::aabbox3d<f32> Box;
	video::SMaterial Material;

	s32 LastAnimationTime;

	SpriteComponent* m_spriteComponent;
	IGameManager* m_gameMgr;
	
	s32 m_phyObjectId;
	std::vector<u16> m_vEntCollBodies;
	std::map< std::string, u16 >	m_dicEntCollBodies;

	IEntityStateMachine* m_stateMachine;

	std::map< std::string, std::string >	m_paramsString;
	std::map< std::string, s32 >	m_paramsInt;
	std::map< std::string, f32 >	m_paramsFloat;

	static bool s_isDebugPhyCollRect;

public:

	EntityNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IGameManager* gameMgr);

	~EntityNode();

	void remove()
	{
		ISceneNode::remove();

		//DBG( " <remove> name: " << getName() );
	}

	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

	void SetSpriteComponent( const std::string& spritePath );

	void SetStateMachine( IEntityStateMachine* stateMachine ){ m_stateMachine = stateMachine; }
	IEntityStateMachine* GetStateMachine( ){ return m_stateMachine; }

	SpriteComponent *GetSpriteComponent()
	{
		return m_spriteComponent;
	}

	PhyObject* GetPhysicObject();
	void CreatePhysicObject();

	IGameManager* GetGameManager() { return m_gameMgr; }

	f32 GetDepth(  ) { return getPosition().Z; }
	void SetDepth( f32 value ) { setPosition( core::vector3df( getPosition().X, getPosition().Y, value) ); } //!!!TODO> depth work in the opposite way, should be greather the deeper

	void SetIntParam( const std::string& sParam, s32 value ) { m_paramsInt[ sParam ] = value; }
	void SetFloatParam( const std::string& sParam, f32 value ) { m_paramsFloat[ sParam ] = value; }
	void SetStringParam( const std::string& sParam, const std::string& value ) { m_paramsString[ sParam ] = value; }

	void SetIntParamOffset( const std::string& sParam, s32 value ) { m_paramsInt[ sParam ] += value; }
	void SetFloatParamOffset( const std::string& sParam, f32 value ) { m_paramsFloat[ sParam ] += value; }

	s32 GetIntParam( const std::string& sParam ) { return m_paramsInt[ sParam ]; }
	f32 GetFloatParam( const std::string& sParam ) { return m_paramsFloat[ sParam ]; }
	std::string GetStringParam( const std::string& sParam ) { return m_paramsString[ sParam ]; }

	bool IsIntParamSet( const std::string& sParam ) { return (m_paramsInt.find( sParam ) != m_paramsInt.end()); }
	bool IsFloatParamSet( const std::string& sParam ) { return (m_paramsFloat.find( sParam ) != m_paramsFloat.end()); }
	bool IsStringParamSet( const std::string& sParam ) { return (m_paramsString.find( sParam ) != m_paramsString.end()); }

	void SetDebugPhyCollRect( bool value ) { s_isDebugPhyCollRect = value; }
	bool IsDebugPhyCollRect( ) { return s_isDebugPhyCollRect; }

	u16 AddEntityCollisionBody( const std::string& name, core::rectf collRect,  u32 collGroup, u32 flags = 0, TEntCollCB collCBFunc = 0 );
	bool RemoveCollisionBody( const std::string& name );
	EntityCollisionBody* GetEntityCollisionBody( const std::string& name );

	void DestroyEntity();
};