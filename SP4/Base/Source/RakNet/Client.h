#ifndef CLIENT_H
#define CLIENT_H

#include "SingletonTemplate.h"

class RakPeerInterface;
class NetworkEntity;

class Client : public Singleton<Client>
{
	friend Singleton<Client>;
public:
	~Client();

	bool Init();
	bool Update(double dt);

	void AnnounceNewObject(NetworkEntity* obj);
	void AnnounceDeleteObject(NetworkEntity* obj);
	void Disconnect();

protected:
	Client();

	//typedef std::vector<Ship*> ShipList;  //!< A list of ships
	//ShipList ships_; //!< List of all the ships in the universe
	RakPeerInterface* rakpeer_;
	float timer_;

	//bool checkCollisions(Ship* ship);
	//void ProcessWelcomePackage();
	bool SendInitialPosition();

	//void SendCollision(Ship* ship);
};

#endif