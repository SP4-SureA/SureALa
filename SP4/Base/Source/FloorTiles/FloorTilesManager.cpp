#include "FloorTilesManager.h"

FloorTilesManager::FloorTilesManager()
{
}

FloorTilesManager::~FloorTilesManager()
{
}

bool FloorTilesManager::AddFloor(FloorTile* flr)
{
	if (flr == NULL)
		return false;

	floorTileList.push_back(flr);

	return true;
}

bool FloorTilesManager::RemoveFloor(FloorTile* flr)
{
	// Find the entity's iterator
	std::list<FloorTile*>::iterator findIter = std::find(floorTileList.begin(), floorTileList.end(), flr);

	// Delete the entity if found
	if (findIter != floorTileList.end())
	{
		findIter = floorTileList.erase(findIter);

		//if (SceneGraph::GetInstance()->DeleteNode(_existingEntity) == false)
		//{
		//	std::cout << "EntityManager::RemoveEntity: Unable to remove" << std::endl;
		//}
		//else
		//{
		//	if (spatialPartition)
		//		spatialPartition->Remove(_existingEntity);
		//}

		return true;
	}
	// Return false if not found
	return false;
}