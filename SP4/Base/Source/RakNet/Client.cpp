#include "Client.h"

#include <RakNetworkFactory.h>
#include <RakPeerInterface.h>

#include "Bitstream.h"
#include <fstream>

#include "config.h"
#include "MyMsgIDs.h"

#include "Scene.h"

#include "NetworkEntityManager.h"
#include "EntityManager.h"
#include "NetworkEntity.h"
#include "..\PlayerInfo\PlayerInfo.h"
#include "..\PlayerInfo\PlayerEntityBase.h"

//testy
#include "SceneManager.h"

Client::Client() :
rakpeer_(RakNetworkFactory::GetRakPeerInterface()),
newID(0),
sendTimer(0.0f),
receiveTimer(0.0f)
{
}

Client::~Client() throw()
{
}

/**
* Initialises the graphics system
* It should also initialise the network system
*/

void Client::Host(Scene* scene)
{
	this->scene = scene;
	rakpeer_->Startup(100, 30, &SocketDescriptor(DFL_PORTNUMBER, 0), 1);
	rakpeer_->SetMaximumIncomingConnections(DFL_MAX_CONNECTION);
	rakpeer_->SetOccasionalPing(true);
	std::cout << "Server Started" << std::endl;
	//std::cout << rakpeer_->GetLocalIP(0) << std::endl;
}

bool Client::Join(Scene* scene)
{
	this->scene = scene;
	std::ifstream inData;
	std::string serverip;
	float init_pos_x, init_pos_y;

	inData.open("serverip.txt");

	inData >> serverip;

	//create player entity here
	if (rakpeer_->Startup(1, 30, &SocketDescriptor(), 1))
	{
		rakpeer_->SetOccasionalPing(true);
		return rakpeer_->Connect(serverip.c_str(), DFL_PORTNUMBER, 0, 0);
		//serverip.c_str()
	}
	return false;
}

void Client::Close()
{
	rakpeer_->Shutdown(100);
	RakNetworkFactory::DestroyRakPeerInterface(rakpeer_);
}

bool Client::Update(double dt)
{
	if (receiveTimer <= 0.0f)
	{
		receiveTimer = 0.01f;

		if (Packet* packet = rakpeer_->Receive())
		{
			std::cout << "receive" << std::endl;
			RakNet::BitStream bs(packet->data, packet->length, false);

			unsigned char msgid = 0;
			RakNetTime timestamp = 0;

			bs.Read(msgid);

			if (msgid == ID_TIMESTAMP)
			{
				bs.Read(timestamp);
				std::cout << timestamp << std::endl;
				bs.Read(msgid);
			}

			switch (msgid)
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				std::cout << "Connected to Server" << std::endl;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "Lost Connection to Server" << std::endl;
				rakpeer_->DeallocatePacket(packet);
				return true;
			case ID_NEW_INCOMING_CONNECTION:
				SendWelcomePackage(packet->systemAddress);
				break;
			case ID_WELCOME:
			{
				unsigned int entityCount, id;
				float x_, y_;

				bs.Read(id);
				PlayerInfo::GetInstance()->GetCharacter()->SetID(id);
				bs.Read(entityCount);

				PlayerEntityBase* testy = Create::PlayerEntity(SceneManager::GetInstance()->GetScene("SceneTest")->GetEntityManager(), "circle", 0.0f, 0.0f, Vector3(), Vector3(3, 3, 3));
				NetworkEntityManager::GetInstance()->AddEntity(testy);

				//for (unsigned int i = 0; i < entityCount; ++i)
				//{
				//	bs.Read(id);
				//	NetworkEntity* go = NULL;

				//	switch (entityType)
				//	{
				//	case GameObject::GAMEOBJECT:
				//		go = Create::gameobject("circle", Vector3(0, 0, 0));
				//		break;
				//	case GameObject::FIGHTER:
				//		go = Create::fighter("character1", Vector3(0, 0, 0));
				//		break;
				//	case GameObject::PROJECTILE:
				//		go = Create::projectile2("sphere");
				//		break;
				//	case GameObject::HEALTHPACK:
				//		go = Create::healthpack ("cube", Vector3());
				//		break;
				//	default:

				//		break;
				//	}
				//	go->SetID(id);
				//	go->ReadInit(bs);
				//}

				SendInitialPosition();
			}
			break;

			//case ID_NEWSHIP:
			//{
			//	unsigned int id;
			//	bs.Read(id);

			//	if (id == Player::GetInstance()->GetCharacter()->GetID())
			//	{
			//		// if it is me
			//		break;
			//	}
			//	else
			//	{
			//		float x_, y_;
			//		char chartemp[5];

			//		bs.Read(x_);
			//		bs.Read(y_);
			//		std::cout << "New Ship pos" << x_ << " " << y_ << std::endl;
			//		Fighter* go = Create::fighter("character1", Vector3(x_, y_, 0), Vector3(3, 3, 3));
			//		go->SetID(id);
			//	}

			//}
			//break;

			//case ID_LOSTSHIP:
			//{
			//	unsigned int id;
			//	bs.Read(id);
			//	EntityManager::GetInstance()->MarkForDeletion(id);
			//}
			//break;

			case ID_INITIALPOS:
				unsigned int id;
				float xPos, yPos;
				bs.Read(id);
				bs.Read(xPos);
				bs.Read(yPos);

				for (auto q : NetworkEntityManager::GetInstance()->GetList())
				{
					if (q->GetID() == id)
					{
						q->SetServerPos(xPos, yPos, 0);
						q->DoInterpolation();

						break;
					}
				}
				break;

			case ID_MOVEMENT:
			{
				unsigned int id;

				bs.Read(id);

				for (auto q : NetworkEntityManager::GetInstance()->GetList())
				{
					if (q->GetID() == id)
					{
						float posX, posY;
						float velX, velY;

						bs.Read(posX);
						bs.Read(posY);
						bs.Read(velX);
						bs.Read(velY);

						q->SetServerPos(posX, posY);
						q->SetServerVel(velX, velY);
						q->DoInterpolation();
						break;
					}
				}
			}
			break;

			//		case ID_COLLIDE:
			//		{
			//						   unsigned int objectID;
			//						   float xVel, yVel;
			//						   float xPos, yPos;
			//						   bs.Read(objectID);
			//						   bs.Read(xPos);
			//						   bs.Read(yPos);
			//						   bs.Read(xVel);
			//						   bs.Read(yVel);
			//						   GameObject* obj = dynamic_cast<GameObject*>(EntityManager::GetInstance()->GetEntity(objectID));
			//						   if (obj)
			//						   {
			//								   obj->SetServerPos(xPos, yPos, 0);
			//								   obj->SetServerVel(xVel, yVel, 0);
			//
			//								   obj->DoInterpolateUpdate();
			//						   }
			//
			//						   bs.Read(objectID);
			//						   bs.Read(xPos);
			//						   bs.Read(yPos);
			//						   bs.Read(xVel);
			//						   bs.Read(yVel);
			//						   obj = dynamic_cast<GameObject*>(EntityManager::GetInstance()->GetEntity(objectID));
			//						   if (obj)
			//						   {
			//							   obj->SetServerPos(xPos, yPos, 0);
			//							   obj->SetServerVel(xVel, yVel, 0);
			//
			//							   obj->DoInterpolateUpdate();
			//						   }
			////			unsigned int shipid;
			////			float x, y;
			////			bs.Read(shipid);
			////
			////			if (shipid == Player::GetInstance()->GetCharacter()->GetID())
			////			{
			////				std::cout << "collided with someone!" << std::endl;
			////				bs.Read(x);
			////				bs.Read(y);
			////				Player::GetInstance()->GetCharacter()->SetX(x);
			////				Player::GetInstance()->GetCharacter()->SetY(y);
			////				bs.Read(x);
			////				bs.Read(y);
			////				Player::GetInstance()->GetCharacter()->SetVelocityX(x);
			////				Player::GetInstance()->GetCharacter()->SetVelocityY(y);
			////				// Lab 13 Task 3 : Collision update for Interpolation
			////#ifdef INTERPOLATEMOVEMENT
			////				// Step 12 : Read and update SetServerVelocityX() and SetServerVelocityY()
			////				bs.Read(x);
			////				bs.Read(y);
			////				Player::GetInstance()->GetCharacter()->SetServerVelocityX(x);
			////				Player::GetInstance()->GetCharacter()->SetServerVelocityY(y);
			////#endif	
			////			}
			//
			//
			//		}
			//		break;

			//case ID_NEWOBJ:
			//{
			//	int entityType;
			//	bs.Read(entityType);

			//	switch (entityType)
			//	{
			//	case GameObject::PROJECTILE:
			//	{
			//		Projectile* obj = Create::projectile2("sphere");
			//		obj->ReadInit(bs);
			//		
			//		unsigned int id;
			//		bs.Read(id);
			//		obj->SetID(id);
			//	}
			//	break;
			//	case GameObject::HEALTHPACK:
			//	{
			//		Healthpack* obj = Create::healthpack("cube", Vector3());
			//		obj->ReadInit(bs);

			//		unsigned int id;
			//		bs.Read(id);
			//		obj->SetID(id);
			//	}
			//	break;

			//	default:break;
			//	}
			//}
			//break;

			//case ID_DELETEOBJ:
			//{
			//	unsigned int id;
			//	bs.Read(id);

			//	EntityManager::GetInstance()->MarkForDeletion(id);
			//}
			//break;

			default:
				std::cout << "Unhandled Message Identifier: " << (int)msgid << std::endl;

			}
			rakpeer_->DeallocatePacket(packet);
		}
	}
	else
		receiveTimer -= dt;

	//send
	if (sendTimer <= 0.0f)
	{
		sendTimer = 0.1f;
		RakNet::BitStream bs2;
		unsigned char msgid = ID_MOVEMENT;
		bs2.Write(msgid);
		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetID());
		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().x);
		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().y);
		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetVelocity().x);
		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetVelocity().y);
		//
		//		// Lab 13 Task 2 : Interpolation
		//#ifdef INTERPOLATEMOVEMENT
		//		// step 9 : Instead of sending x,y,w ..... , send the server version instead
		//		bs2.Write(Player::GetInstance()->GetCharacter()->GetID());
		//		Player::GetInstance()->GetCharacter()->Write(bs2);
		//#else
		//		bs2.Write(ships_.at(0)->GetID());
		//		bs2.Write(ships_.at(0)->GetX());
		//		bs2.Write(ships_.at(0)->GetY());
		//		bs2.Write(ships_.at(0)->GetW());
		//		// Lab 13 Task 1 : Add Extrapolation Data velocity x, velocity y & angular velocity
		//#ifdef EXTRAPOLATEMOVEMENT
		//		bs2.Write(ships_.at(0)->GetVelocityX());
		//		bs2.Write(ships_.at(0)->GetVelocityY());
		//		bs2.Write(ships_.at(0)->GetAngularVelocity());
		//
		//#endif
		//#endif
		//		
		rakpeer_->Send(&bs2, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else
		sendTimer -= dt;

	return false;
}

void Client::Disconnect()
{
	RakNet::BitStream bs;
	unsigned char msgid = ID_DISCONNECTION_NOTIFICATION;
	bs.Write(msgid);

	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

bool Client::SendInitialPosition()
{
	RakNet::BitStream bs;
	unsigned char msgid = ID_INITIALPOS;
	bs.Write(msgid);
	bs.Write(PlayerInfo::GetInstance()->GetCharacter()->GetID());
	bs.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().x);
	bs.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().y);

	std::cout << "Sending pos" << PlayerInfo::GetInstance()->GetCharacter()->GetPosition() << std::endl;

	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

	return true;
}

//void Client::AnnounceNewObject(GameObject* obj)
//{
//	RakNet::BitStream bs;
//	unsigned char msgid = ID_NEWOBJ;
//
//	bs.Write(msgid);//message ID
//	bs.Write(obj->GetEntityType());//entity type
//	obj->WriteInit(bs);//required entity variables
//
//	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//}
//
//void Client::AnnounceDeleteObject(GameObject* obj)
//{
//	RakNet::BitStream bs;
//	unsigned char msgid = ID_DELETEOBJ;
//
//	bs.Write(msgid);//message ID
//	bs.Write(obj->GetID());
//
//	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//}

void Client::SendWelcomePackage(SystemAddress& addr)
{
	++newID;

	unsigned char msgid = ID_WELCOME;
	int objCount = NetworkEntityManager::GetInstance()->GetList().size();

	RakNet::BitStream bs;
	bs.Write(msgid);
	bs.Write(newID);
	bs.Write(objCount);

	//std::list<EntityBase*>::iterator itr;
	if (NetworkEntityManager::GetInstance()->GetList().size() > 0)
	{
		for (auto q : NetworkEntityManager::GetInstance()->GetList())
		{
			//std::cout << "go:  " << (*itr)->GetID() << " pos" << (*itr)->GetPosition().x << " " << (*itr)->GetPosition().y << std::endl;
			bs.Write(q->GetID());
			q->WriteInit(bs);
		}
	}

	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, false);

	bs.Reset();

	NetworkEntity* go = Create::networkEntity(scene->GetEntityManager(), "circle", Vector3(), Vector3(3, 3, 3));
	NetworkEntityManager::GetInstance()->AddEntity(go);
	go->SetID(newID);
	//clients_.insert(std::make_pair(addr, go));

	std::cout << "New guy, assigned id " << newID << std::endl;
}

void Client::SendDisconnectionNotification(SystemAddress& addr)
{
	//ClientMap::iterator itr = clients_.find(addr);
	//if (itr == clients_.end())
	//	return;

	//unsigned char msgid = ID_LOSTSHIP;
	//RakNet::BitStream bs;
	//bs.Write(msgid);
	//bs.Write(itr->second->GetID());

	//rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, true);

	//std::cout << itr->second->GetID() << " has left the game" << std::endl;

	//EntityManager::GetInstance()->MarkForDeletion(itr->second);
	//clients_.erase(itr);
}

//#include "Client.h"
//
//#include <RakNetworkFactory.h>
//#include <RakPeerInterface.h>
//
//#include "Bitstream.h"
//#include <fstream>
//
//#include "config.h"
//#include "MyMsgIDs.h"
//
//#include "Scene.h"
//
//#include "NetworkEntityManager.h"
//#include "EntityManager.h"
//#include "NetworkEntity.h"
//#include "..\PlayerInfo\PlayerInfo.h"
//#include "..\PlayerInfo\PlayerEntityBase.h"
//
////testy
//#include "SceneManager.h"
//
//Client::Client() :
//rakpeer_(RakNetworkFactory::GetRakPeerInterface()),
//newID(0),
//sendTimer(0.0f),
//receiveTimer(0.0f)
//{
//}
//
//Client::~Client() throw()
//{
//}
//
///**
//* Initialises the graphics system
//* It should also initialise the network system
//*/
//
//void Client::Host(Scene* scene)
//{
//	this->scene = scene;
//	rakpeer_->Startup(100, 30, &SocketDescriptor(DFL_PORTNUMBER, 0), 1);
//	rakpeer_->SetMaximumIncomingConnections(DFL_MAX_CONNECTION);
//	rakpeer_->SetOccasionalPing(true);
//	std::cout << "Server Started" << std::endl;
//	//std::cout << rakpeer_->GetLocalIP(0) << std::endl;
//}
//
//bool Client::Join(Scene* scene)
//{
//	this->scene = scene;
//	std::ifstream inData;
//	std::string serverip;
//	float init_pos_x, init_pos_y;
//
//	inData.open("serverip.txt");
//
//	inData >> serverip;
//
//	//create player entity here
//	if (rakpeer_->Startup(1, 30, &SocketDescriptor(), 1))
//	{
//		rakpeer_->SetOccasionalPing(true);
//		return rakpeer_->Connect(serverip.c_str(), DFL_PORTNUMBER, 0, 0);
//		//serverip.c_str()
//	}
//	return false;
//}
//
//void Client::Close()
//{
//	rakpeer_->Shutdown(100);
//	RakNetworkFactory::DestroyRakPeerInterface(rakpeer_);
//}
///**
//* Update cycle
//*
//* Checks for keypresses:
//*   - Esc - Quits the game
//*   - Left - Rotates ship left
//*   - Right - Rotates ship right
//*   - Up - Accelerates the ship
//*   - Down - Deccelerates the ship
//*
//* Also calls Update() on all the ships in the universe
//*/
//bool Client::Update(double dt)
//{
//	if (receiveTimer <= 0.0f)
//	{
//		receiveTimer = 0.01f;
//
//		if (Packet* packet = rakpeer_->Receive())
//		{
//			RakNet::BitStream bs(packet->data, packet->length, false);
//
//			unsigned char msgid = 0;
//			RakNetTime timestamp = 0;
//
//			bs.Read(msgid);
//
//			if (msgid == ID_TIMESTAMP)
//			{
//				bs.Read(timestamp);
//				std::cout << timestamp << std::endl;
//				bs.Read(msgid);
//			}
//
//			switch (msgid)
//			{
//			case ID_CONNECTION_REQUEST_ACCEPTED:
//				std::cout << "Connected to Server" << std::endl;
//				break;
//			case ID_NO_FREE_INCOMING_CONNECTIONS:
//			case ID_CONNECTION_LOST:
//			case ID_DISCONNECTION_NOTIFICATION:
//				std::cout << "Lost Connection to Server" << std::endl;
//				rakpeer_->DeallocatePacket(packet);
//				return true;
//			case ID_NEW_INCOMING_CONNECTION:
//				SendWelcomePackage(packet->systemAddress);
//				break;
//			case ID_WELCOME:
//			{
//				unsigned int entityCount, id;
//				float x_, y_;
//
//				bs.Read(id);
//				PlayerInfo::GetInstance()->GetCharacter()->SetID(id);
//				bs.Read(entityCount);
//
//				PlayerEntityBase* testy = Create::PlayerEntity(SceneManager::GetInstance()->GetScene("SceneTest")->GetEntityManager(), "circle", 0.0f, 0.0f, Vector3(), Vector3(3, 3, 3));
//				NetworkEntityManager::GetInstance()->AddEntity(testy);
//
//				//for (unsigned int i = 0; i < entityCount; ++i)
//				//{
//				//	bs.Read(id);
//				//	NetworkEntity* go = NULL;
//
//				//	switch (entityType)
//				//	{
//				//	case GameObject::GAMEOBJECT:
//				//		go = Create::gameobject("circle", Vector3(0, 0, 0));
//				//		break;
//				//	case GameObject::FIGHTER:
//				//		go = Create::fighter("character1", Vector3(0, 0, 0));
//				//		break;
//				//	case GameObject::PROJECTILE:
//				//		go = Create::projectile2("sphere");
//				//		break;
//				//	case GameObject::HEALTHPACK:
//				//		go = Create::healthpack ("cube", Vector3());
//				//		break;
//				//	default:
//
//				//		break;
//				//	}
//				//	go->SetID(id);
//				//	go->ReadInit(bs);
//				//}
//
//				SendInitialPosition();
//			}
//			break;
//
//			//case ID_NEWSHIP:
//			//{
//			//	unsigned int id;
//			//	bs.Read(id);
//
//			//	if (id == Player::GetInstance()->GetCharacter()->GetID())
//			//	{
//			//		// if it is me
//			//		break;
//			//	}
//			//	else
//			//	{
//			//		float x_, y_;
//			//		char chartemp[5];
//
//			//		bs.Read(x_);
//			//		bs.Read(y_);
//			//		std::cout << "New Ship pos" << x_ << " " << y_ << std::endl;
//			//		Fighter* go = Create::fighter("character1", Vector3(x_, y_, 0), Vector3(3, 3, 3));
//			//		go->SetID(id);
//			//	}
//
//			//}
//			//break;
//
//			//case ID_LOSTSHIP:
//			//{
//			//	unsigned int id;
//			//	bs.Read(id);
//			//	EntityManager::GetInstance()->MarkForDeletion(id);
//			//}
//			//break;
//
//			case ID_INITIALPOS:
//				unsigned int id;
//				float xPos, yPos;
//				bs.Read(id);
//				bs.Read(xPos);
//				bs.Read(yPos);
//
//				for (auto q : NetworkEntityManager::GetInstance()->GetList())
//				{
//					if (q->GetID() == id)
//					{
//						q->SetServerPos(xPos, yPos, 0);
//						q->DoInterpolation();
//
//						break;
//					}
//				}
//				break;
//
//			case ID_MOVEMENT:
//			{
//					unsigned int id;
//				
//					bs.Read(id);
//					
//					for (auto q : NetworkEntityManager::GetInstance()->GetList())
//					{
//						if (q->GetID() == id)
//						{
//							float x, y;
//
//							bs.Read(x);
//							bs.Read(y);
//
//							q->SetServerPos(Vector3(x, y, 0));
//							q->DoInterpolation();
//							break;
//						}
//					}
//			}
//			break;
//
//			//		case ID_COLLIDE:
//			//		{
//			//						   unsigned int objectID;
//			//						   float xVel, yVel;
//			//						   float xPos, yPos;
//			//						   bs.Read(objectID);
//			//						   bs.Read(xPos);
//			//						   bs.Read(yPos);
//			//						   bs.Read(xVel);
//			//						   bs.Read(yVel);
//			//						   GameObject* obj = dynamic_cast<GameObject*>(EntityManager::GetInstance()->GetEntity(objectID));
//			//						   if (obj)
//			//						   {
//			//								   obj->SetServerPos(xPos, yPos, 0);
//			//								   obj->SetServerVel(xVel, yVel, 0);
//			//
//			//								   obj->DoInterpolateUpdate();
//			//						   }
//			//
//			//						   bs.Read(objectID);
//			//						   bs.Read(xPos);
//			//						   bs.Read(yPos);
//			//						   bs.Read(xVel);
//			//						   bs.Read(yVel);
//			//						   obj = dynamic_cast<GameObject*>(EntityManager::GetInstance()->GetEntity(objectID));
//			//						   if (obj)
//			//						   {
//			//							   obj->SetServerPos(xPos, yPos, 0);
//			//							   obj->SetServerVel(xVel, yVel, 0);
//			//
//			//							   obj->DoInterpolateUpdate();
//			//						   }
//			////			unsigned int shipid;
//			////			float x, y;
//			////			bs.Read(shipid);
//			////
//			////			if (shipid == Player::GetInstance()->GetCharacter()->GetID())
//			////			{
//			////				std::cout << "collided with someone!" << std::endl;
//			////				bs.Read(x);
//			////				bs.Read(y);
//			////				Player::GetInstance()->GetCharacter()->SetX(x);
//			////				Player::GetInstance()->GetCharacter()->SetY(y);
//			////				bs.Read(x);
//			////				bs.Read(y);
//			////				Player::GetInstance()->GetCharacter()->SetVelocityX(x);
//			////				Player::GetInstance()->GetCharacter()->SetVelocityY(y);
//			////				// Lab 13 Task 3 : Collision update for Interpolation
//			////#ifdef INTERPOLATEMOVEMENT
//			////				// Step 12 : Read and update SetServerVelocityX() and SetServerVelocityY()
//			////				bs.Read(x);
//			////				bs.Read(y);
//			////				Player::GetInstance()->GetCharacter()->SetServerVelocityX(x);
//			////				Player::GetInstance()->GetCharacter()->SetServerVelocityY(y);
//			////#endif	
//			////			}
//			//
//			//
//			//		}
//			//		break;
//
//			//case ID_NEWOBJ:
//			//{
//			//	int entityType;
//			//	bs.Read(entityType);
//
//			//	switch (entityType)
//			//	{
//			//	case GameObject::PROJECTILE:
//			//	{
//			//		Projectile* obj = Create::projectile2("sphere");
//			//		obj->ReadInit(bs);
//			//		
//			//		unsigned int id;
//			//		bs.Read(id);
//			//		obj->SetID(id);
//			//	}
//			//	break;
//			//	case GameObject::HEALTHPACK:
//			//	{
//			//		Healthpack* obj = Create::healthpack("cube", Vector3());
//			//		obj->ReadInit(bs);
//
//			//		unsigned int id;
//			//		bs.Read(id);
//			//		obj->SetID(id);
//			//	}
//			//	break;
//
//			//	default:break;
//			//	}
//			//}
//			//break;
//
//			//case ID_DELETEOBJ:
//			//{
//			//	unsigned int id;
//			//	bs.Read(id);
//
//			//	EntityManager::GetInstance()->MarkForDeletion(id);
//			//}
//			//break;
//
//			default:
//				std::cout << "Unhandled Message Identifier: " << (int)msgid << std::endl;
//
//			}
//			rakpeer_->DeallocatePacket(packet);
//		}
//	}
//	else
//		receiveTimer -= dt;
//
//	//send
//	if (sendTimer <= 0.0f)
//	{
//		sendTimer = 0.05f;
//		RakNet::BitStream bs2;
//		unsigned char msgid = ID_MOVEMENT;
//		bs2.Write(msgid);
//		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetID());
//		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().x);
//		bs2.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().y);
////
////		// Lab 13 Task 2 : Interpolation
////#ifdef INTERPOLATEMOVEMENT
////		// step 9 : Instead of sending x,y,w ..... , send the server version instead
////		bs2.Write(Player::GetInstance()->GetCharacter()->GetID());
////		Player::GetInstance()->GetCharacter()->Write(bs2);
////#else
////		bs2.Write(ships_.at(0)->GetID());
////		bs2.Write(ships_.at(0)->GetX());
////		bs2.Write(ships_.at(0)->GetY());
////		bs2.Write(ships_.at(0)->GetW());
////		// Lab 13 Task 1 : Add Extrapolation Data velocity x, velocity y & angular velocity
////#ifdef EXTRAPOLATEMOVEMENT
////		bs2.Write(ships_.at(0)->GetVelocityX());
////		bs2.Write(ships_.at(0)->GetVelocityY());
////		bs2.Write(ships_.at(0)->GetAngularVelocity());
////
////#endif
////#endif
////		
//		rakpeer_->Send(&bs2, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//	}
//	else
//		sendTimer -= dt;
//
//	return false;
//}
//
//void Client::Disconnect()
//{
//	RakNet::BitStream bs;
//	unsigned char msgid = ID_DISCONNECTION_NOTIFICATION;
//	bs.Write(msgid);
//
//	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//}
//
//bool Client::SendInitialPosition()
//{
//	RakNet::BitStream bs;
//	unsigned char msgid = ID_INITIALPOS;
//	bs.Write(msgid);
//	bs.Write(PlayerInfo::GetInstance()->GetCharacter()->GetID());
//	bs.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().x);
//	bs.Write(PlayerInfo::GetInstance()->GetCharacter()->GetPosition().y);
//
//	std::cout << "Sending pos" << PlayerInfo::GetInstance()->GetCharacter()->GetPosition() << std::endl;
//
//	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//
//	return true;
//}
//
////void Client::AnnounceNewObject(GameObject* obj)
////{
////	RakNet::BitStream bs;
////	unsigned char msgid = ID_NEWOBJ;
////
////	bs.Write(msgid);//message ID
////	bs.Write(obj->GetEntityType());//entity type
////	obj->WriteInit(bs);//required entity variables
////
////	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
////}
////
////void Client::AnnounceDeleteObject(GameObject* obj)
////{
////	RakNet::BitStream bs;
////	unsigned char msgid = ID_DELETEOBJ;
////
////	bs.Write(msgid);//message ID
////	bs.Write(obj->GetID());
////
////	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
////}
//
//void Client::SendWelcomePackage(SystemAddress& addr)
//{
//	++newID;
//
//	unsigned char msgid = ID_WELCOME;
//	int objCount = NetworkEntityManager::GetInstance()->GetList().size();
//
//	RakNet::BitStream bs;
//	bs.Write(msgid);
//	bs.Write(newID);
//	bs.Write(objCount);
//
//	//std::list<EntityBase*>::iterator itr;
//	if (NetworkEntityManager::GetInstance()->GetList().size() > 0)
//	{
//		for (auto q : NetworkEntityManager::GetInstance()->GetList())
//		{
//			//std::cout << "go:  " << (*itr)->GetID() << " pos" << (*itr)->GetPosition().x << " " << (*itr)->GetPosition().y << std::endl;
//			bs.Write(q->GetID());
//			q->WriteInit(bs);
//		}
//	}
//	
//	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, false);
//
//	bs.Reset();
//
//	NetworkEntity* go = Create::networkEntity(scene->GetEntityManager(), "circle", Vector3(), Vector3(3, 3, 3));
//	NetworkEntityManager::GetInstance()->AddEntity(go);
//	go->SetID(newID);
//	//clients_.insert(std::make_pair(addr, go));
//
//	std::cout << "New guy, assigned id " << newID << std::endl;
//}
//
//void Client::SendDisconnectionNotification(SystemAddress& addr)
//{
//	//ClientMap::iterator itr = clients_.find(addr);
//	//if (itr == clients_.end())
//	//	return;
//
//	//unsigned char msgid = ID_LOSTSHIP;
//	//RakNet::BitStream bs;
//	//bs.Write(msgid);
//	//bs.Write(itr->second->GetID());
//
//	//rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, true);
//
//	//std::cout << itr->second->GetID() << " has left the game" << std::endl;
//
//	//EntityManager::GetInstance()->MarkForDeletion(itr->second);
//	//clients_.erase(itr);
//}