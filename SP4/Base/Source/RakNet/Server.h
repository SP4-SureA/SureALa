#ifndef SERVER_H
#define SERVER_H

#include "SingletonTemplate.h"

#include "RakNetTypes.h"

#include <map>
using std::map;

#include <queue>
using std::queue;

class NetworkEntity;
class RakPeerInterface;

class Server : public Singleton<Server>
{
	friend Singleton<Server>;
public:
	Server();
	~Server();

	void Init();
	void Close();
	void Update(double dt);

	void AnnounceNewObject(NetworkEntity* obj);
	void AnnounceCollision(NetworkEntity* a, NetworkEntity* b);

	typedef std::map<SystemAddress, NetworkEntity*> ClientMap;

	ClientMap clients_;

	std::queue<SystemAddress> clientQueue;
protected:
	RakPeerInterface* rakpeer_;
	
	unsigned int newID;

	//a storage to store entities derived from NetworkProperties?

	void SendWelcomePackage(SystemAddress& addr);
	void SendDisconnectionNotification(SystemAddress& addr);
	void ProcessInitialPosition(SystemAddress& addr, float x_, float y_, int type_);

};

#endif