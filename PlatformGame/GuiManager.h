#pragma once

#include "GameManager.h"
#include "Sprite.h"

class GuiManager
{

public:

	GuiManager( GameManager* gameMgr );

	~GuiManager(){}

	void Update();

	void Render();

private:
	GameManager* m_gameMgr;

	Sprite* m_sprite;
};