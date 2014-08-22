#pragma once

#include <irrlicht.h>
#include <string>

#include "PhyObject.h"

using namespace irr;

class IGameManager;

class LayerNode : public scene::ISceneNode
{

private:

	core::aabbox3d<f32> Box;
	s32 LastAnimationTime;

protected:

	video::SMaterial Material;

	IGameManager* m_gameMgr;
	
	video::ITexture* m_texture;

	f32 m_paraxFactorX;
	f32 m_paraxFactorY;
	f32 m_paraxZoomFactor;

	f32 m_offsetX;
	f32 m_offsetY;

	u32 m_width;
	u32 m_height;

public:

	LayerNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IGameManager* gameMgr, video::ITexture* texture)
		: ISceneNode( parent, mgr, id ),
		LastAnimationTime(0),
		m_gameMgr(gameMgr),
		m_texture(texture),
		m_paraxFactorX(1),
		m_paraxFactorY(1)
	{
		Material.Wireframe = false;
		Material.Lighting = false;

		m_width = m_texture? m_texture->getSize().Width : 0;
		m_height = m_texture? m_texture->getSize().Height : 0;

		setAutomaticCulling( irr::scene::EAC_OFF );
	}

	~LayerNode()
	{
		DBG("~LayerNode " << this->getName());
	}

	void SetParallaxFactorX( f32 factor ) { m_paraxFactorX = factor; };
	void SetParallaxFactorY( f32 factor ) { m_paraxFactorY = factor; };
	void SetParallaxZoomFactor( f32 factor ) { m_paraxZoomFactor = factor; };

	void SetOffset( f32 xf, f32 yf ) { m_offsetX = xf; m_offsetY = yf; };

	void SetWidth( u32 value ) { m_width = value; };
	void SetHeight( u32 value ) { m_height = value; };

	void SetTexture( video::ITexture* texture ){ m_texture = texture; }

	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(u32 timeMs);

	virtual void Update(u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

};