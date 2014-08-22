#include "GuiManager.h"

static const int HP_PER_HEART = 20;

GuiManager::GuiManager( GameManager* gameMgr )
	: m_gameMgr( gameMgr )
{	
	m_sprite = Sprite::LoadFromFile("sprites\\gui.sprite", m_gameMgr->GetDriver() );
}

void GuiManager::Update()
{
	irr::s32 frameDt = m_gameMgr->GetFrameDt();	
}

void GuiManager::Render()
{

	core::vector2df scrToWorlTrans = 
		m_gameMgr->GetCamera()->GetPosition() - 
		core::vector2df( m_gameMgr->GetCamera()->GetViewRect().getWidth()/2, m_gameMgr->GetCamera()->GetViewRect().getHeight()/2 ) / m_gameMgr->GetCamera()->GetScale();

	EntityNode* playerNode = m_gameMgr->GetEntityManager()->GetEntityByTag("player");
	int player_hp = playerNode->GetIntParam("HP");
	int player_hpMax = playerNode->GetIntParam("HP_Max");

	// Render Hearts
	Animation* heartAnim = m_sprite->GetAnimByTag("heart");

	int heartNb = (player_hpMax % HP_PER_HEART)? (player_hpMax / HP_PER_HEART) + 1 : (player_hpMax / HP_PER_HEART);
	int heartFilledNb = (player_hp / HP_PER_HEART);
	int heartMod = (player_hp % HP_PER_HEART);

	core::vector2df vHeartPos( 0, 0 );
	vHeartPos += scrToWorlTrans;

	for( int i = 0; i < heartNb; ++i, vHeartPos.X += 16 )
	{
		int iAnimFrameIdx = 0;
		if( i < heartFilledNb )
		{
			iAnimFrameIdx = heartAnim->AFrames.size() - 1;
		}
		else if( heartMod > 0 )
		{
			iAnimFrameIdx = (heartAnim->AFrames.size() * heartMod) / HP_PER_HEART;
			heartMod = 0;
		}

		m_sprite->DrawAFrame( heartAnim, iAnimFrameIdx, vHeartPos );
	}
}