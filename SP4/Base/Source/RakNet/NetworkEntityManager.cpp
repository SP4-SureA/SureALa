#include "NetworkEntityManager.h"

#include "NetworkEntity.h"

NetworkEntityManager::NetworkEntityManager()
{
}

NetworkEntityManager::~NetworkEntityManager()
{
}

void NetworkEntityManager::UpdateInterpolations(double dt)
{
	for (auto q : networkEntityList)
	{
		if (!q->GetShouldUpdate())
			q->UpdateInterpolation(dt);
	}
}

void NetworkEntityManager::AddEntity(NetworkEntity* _newEntity)
{
	networkEntityList.push_back(_newEntity);
}

bool NetworkEntityManager::RemoveEntity(NetworkEntity* _existingEntity)
{
	// Find the entity's iterator
	std::list<NetworkEntity*>::iterator findIter = std::find(networkEntityList.begin(), networkEntityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != networkEntityList.end())
	{
		NetworkEntity* placeHolder = *findIter;
		findIter = networkEntityList.erase(findIter);

		return true;
	}
	// Return false if not found
	return false;
}

bool NetworkEntityManager::RemoveEntity(unsigned int id)
{
	std::list<NetworkEntity*>::iterator it;
	for (it = networkEntityList.begin(); it != networkEntityList.end(); ++it)
	{
		if ((*it)->GetID() == id)
		{
			networkEntityList.erase(it);
			
			return true;
		}
	}

	// Return false if not found
	return false;
}

void NetworkEntityManager::ClearList()
{
	std::list<NetworkEntity*>::iterator it;
	for (it = networkEntityList.begin(); it != networkEntityList.end();)
	{
		it = networkEntityList.erase(it);
	}
}