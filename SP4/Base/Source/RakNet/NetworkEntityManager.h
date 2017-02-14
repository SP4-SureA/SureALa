#ifndef NETWORK_ENTITY_MANAGER
#define NETWORK_ENTITY_MANAGER

#include "SingletonTemplate.h"

#include <list>
using std::list;

class NetworkEntity;

class NetworkEntityManager : public Singleton<NetworkEntityManager>
{
	friend Singleton<NetworkEntityManager>;
public:
	virtual ~NetworkEntityManager();
	virtual void UpdateInterpolations(double dt);

	void AddEntity(NetworkEntity* _newEntity, bool bAddToSpatialPartition = false);
	bool RemoveEntity(NetworkEntity* _existingEntity);
	bool RemoveEntity(unsigned int id);
	void ClearList();

	std::list<NetworkEntity*> GetList();

protected:
	NetworkEntityManager();

	std::list<NetworkEntity*> networkEntityList;
};

#endif