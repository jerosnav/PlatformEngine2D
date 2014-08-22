#include "EntityBlock.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityBlock::State_Dying(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->SetAnimationByTag("Dying", 1);
		entityNode_->SetIntParam("Alpha", 255);
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_EXIT
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_EXIT )
	{
		;
	}
	else
//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_UPDATE
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_UPDATE )
	{

		PhyObject* phyObj = entityNode_->GetPhysicObject();
		EntityNode* player = entityNode_->GetGameManager()->GetEntityManager()->GetEntityByTag("player");

		// fade out fx
		int alpha = entityNode_->GetIntParam("Alpha");
		alpha /= pow( 256., entityNode_->GetGameManager()->GetFrameDt() / 1000.);// This divide alpha by 40 each second
		entityNode_->SetIntParam("Alpha", alpha);
		entityNode_->GetSpriteComponent()->GetAnimPlayer()->TintColor.setAlpha(alpha);
		

		if( entityNode_->GetSpriteComponent()->GetAnimPlayer()->IsAnimOver() )
		{
			// Set current tile as free
			LayerTile* layerTile = entityNode_->GetGameManager()->GetLevel()->GetPhyLayerTileAtPos( phyObj->Position.X, phyObj->Position.Y );
			int phyTilesetId = entityNode_->GetGameManager()->GetLevel()->GetTilemap()->GetTilesetId("physics");
			layerTile->SetTilesetId(-1);
			layerTile->SetTileId(-1);
			
			// Restore tileset
			layerTile->SetTilesetId(entityNode_->GetIntParam("TilesetId"));
			layerTile->SetTileId(entityNode_->GetIntParam("TileId"));

			entityNode_->DestroyEntity();
		}
	}
}