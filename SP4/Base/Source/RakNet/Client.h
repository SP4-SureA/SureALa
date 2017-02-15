#ifndef CLIENT_H
#define CLIENT_H

#include "SingletonTemplate.h"

#include "RakNetTypes.h"

class Scene;
class RakPeerInterface;
class NetworkEntity;

class Client : public Singleton<Client>
{
	friend Singleton<Client>;
public:
	~Client();

	void Host(Scene* scene);
	bool Join(Scene* scene);
	void Close();
	bool Update(double dt);

	void AnnounceNewObject(NetworkEntity* obj);
	void AnnounceDeleteObject(NetworkEntity* obj);
	void Disconnect();

protected:
	Client();

	unsigned int newID;
	Scene* scene;

	//typedef std::vector<Ship*> ShipList;  //!< A list of ships
	//ShipList ships_; //!< List of all the ships in the universe
	RakPeerInterface* rakpeer_;
	float sendTimer;
	float receiveTimer;

	//bool checkCollisions(Ship* ship);
	//void ProcessWelcomePackage();
	void SendWelcomePackage(SystemAddress& addr);
	void SendDisconnectionNotification(SystemAddress& addr);
	bool SendInitialPosition();

	//void SendCollision(Ship* ship);
};

#endif

//#ifndef CLIENT_H
//#define CLIENT_H
//
//#include "SingletonTemplate.h"
//
//#include "RakNetTypes.h"
//
//class Scene;
//class RakPeerInterface;
//class NetworkEntity;
//
//class Client : public Singleton<Client>
//{
//	friend Singleton<Client>;
//public:
//	~Client();
//
//	void Host(Scene* scene);
//	bool Join(Scene* scene);
//	void Close();
//	bool Update(double dt);
//
//	void AnnounceNewObject(NetworkEntity* obj);
//	void AnnounceDeleteObject(NetworkEntity* obj);
//	void Disconnect();
//
//protected:
//	Client();
//
//	unsigned int newID;
//	Scene* scene;
//
//	//typedef std::vector<Ship*> ShipList;  //!< A list of ships
//	//ShipList ships_; //!< List of all the ships in the universe
//	RakPeerInterface* rakpeer_;
//	float sendTimer;
//	float receiveTimer;
//
//	//bool checkCollisions(Ship* ship);
//	//void ProcessWelcomePackage();
//	void SendWelcomePackage(SystemAddress& addr);
//	void SendDisconnectionNotification(SystemAddress& addr);
//	bool SendInitialPosition();
//
//	//void SendCollision(Ship* ship);
//};
//
//#endif