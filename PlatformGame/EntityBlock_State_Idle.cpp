#include "EntityBlock.h"

#include "EntityFactory.h"

//---------------------------------------------------------------------------------------------------------
// State Code
//---------------------------------------------------------------------------------------------------------
void EntityBlock::State_Idle(s32 messageType_, EntityNode* entityNode_, s32 dataSize_)
{

//---------------------------------------------------------------------------------------------------------
// STATE_MESSAGE_ENTER
//---------------------------------------------------------------------------------------------------------
	if( messageType_ == eCommonMessageTypes::STATE_MESSAGE_ENTER )
	{
		PhyObject* phyObj = entityNode_->GetPhysicObject();
		phyObj->Velocity.X = 0;

		// Set current tile as solid
		LayerTile* layerTile = entityNode_->GetGameManager()->GetLevel()->GetPhyLayerTileAtPos( phyObj->Position.X, phyObj->Position.Y );
		int phyTilesetId = entityNode_->GetGameManager()->GetLevel()->GetTilemap()->GetTilesetId("physics");

		if( false == entityNode_->IsIntParamSet("TilesetId") )
		{
			// Keep old value
			entityNode_->SetIntParam("TilesetId", layerTile->GetTilesetId());
			entityNode_->SetIntParam("TileId", layerTile->GetTileId());

			// Set blocked
			layerTile->SetTilesetId(phyTilesetId);
			layerTile->SetTileId(0x00000000);
		}
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
	}
}