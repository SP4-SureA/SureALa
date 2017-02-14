#include "Server.h"

#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "Bitstream.h"
#include "GetTime.h"

#include "NetworkEntityManager.h"
#include "NetworkEntity.h"

#include "config.h"
#include "MyMsgIDs.h"

#include <iostream>

Server::Server() :
rakpeer_(RakNetworkFactory::GetRakPeerInterface()),
newID(0)
{
}

Server::~Server()
{
	rakpeer_->Shutdown(100);
	RakNetworkFactory::DestroyRakPeerInterface(rakpeer_);
}

void Server::Init()
{
	rakpeer_->Startup(100, 30, &SocketDescriptor(DFL_PORTNUMBER, 0), 1);
	rakpeer_->SetMaximumIncomingConnections(DFL_MAX_CONNECTION);
	rakpeer_->SetOccasionalPing(true);
	std::cout << "Server Started" << std::endl;
}



void Server::Update(double dt)
{
	if (Packet* packet = rakpeer_->Receive())
	{
		RakNet::BitStream bs(packet->data, packet->length, true);

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
		case ID_NEW_INCOMING_CONNECTION:
			if (rakpeer_->NumberOfConnections() <= 2)
			{
				SendWelcomePackage(packet->systemAddress);
			}
			else
			{
				unsigned char msgid = ID_GAMEFULL;
				RakNet::BitStream bs2;
				bs2.Write(msgid);
				rakpeer_->Send(&bs2, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, false);
				//clientQueue.push(packet->systemAddress);
			}
			break;
		case ID_DISCONNECTION_NOTIFICATION:
		case ID_CONNECTION_LOST:
			SendDisconnectionNotification(packet->systemAddress);
			//if (clientQueue.size() > 0)
			//{
			//	SendWelcomePackage(clientQueue.front());
			//	clientQueue.pop();
			//}
			break;

		case ID_INITIALPOS:
		{
			float x_, y_;
			int type_;
			std::cout << "ProcessInitialPosition" << std::endl;
			bs.Read(x_);
			bs.Read(y_);

			ProcessInitialPosition(packet->systemAddress, x_, y_, 0);
		}
		break;

		case ID_MOVEMENT:
		{
			unsigned int shipid;
			bs.Read(shipid);
			//UpdatePosition(packet->systemAddress, x, y);
			ClientMap::iterator itr = clients_.find(packet->systemAddress);
			if (itr == clients_.end())
				return;

			itr->second->Read(bs);
			itr->second->DoInterpolation();

			bs.ResetReadPointer();
			rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, true);
		}
		break;

		case ID_COLLIDE:
		{
			bs.ResetReadPointer();
			rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, true);
		}
		break;
		case ID_NEWOBJ:
		{
			++newID;

			int entityType;
			bs.Read(entityType);

			//switch (entityType)
			//{
			//case NetworkEntity::PROJECTILE:
			//{
			//	Projectile* proj = Create::projectile2("sphere");
			//	proj->SetID(newID);
			//	proj->ReadInit(bs);
			//	bs.Write(newID);

			//	bs.ResetReadPointer();
			//	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			//}
			//break;

			//default:break;
			//}
		}
		break;

		case ID_DELETEOBJ:
		{
			//int id;
			//bs.Read(id);
			//
			//EntityManager::GetInstance()->MarkForDeletion(id);
			//
			//bs.ResetReadPointer();
			//rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, true);
		}
		break;

		default:
			std::cout << "Unhandled Message Identifier: " << (int)msgid << std::endl;
		}

		rakpeer_->DeallocatePacket(packet);
	}
}

void Server::SendWelcomePackage(SystemAddress& addr)
{
	//++newID;

	//unsigned char msgid = ID_WELCOME;
	//int objCount = NetworkEntityManager::GetInstance()->GetList().size();

	//RakNet::BitStream bs;
	//bs.Write(msgid);
	//bs.Write(newID);
	//bs.Write(objCount);
	//std::cout << "sent " << objCount << " entity" << std::endl;
	////std::list<EntityBase*>::iterator itr;
	//if (NetworkEntityManager::GetInstance()->GetList().size() > 0)
	//{
	//	for (auto q : NetworkEntityManager::GetInstance()->GetList())
	//	{
	//		NetworkEntity* obj = dynamic_cast<NetworkEntity*>(q);
	//		if (obj)
	//		{
	//			//std::cout << "go:  " << (*itr)->GetID() << " pos" << (*itr)->GetPosition().x << " " << (*itr)->GetPosition().y << std::endl;
	//			bs.Write(obj->GetID());
	//			obj->WriteInit(bs);
	//		}
	//	}
	//}

	//rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, false);

	//bs.Reset();

	//NetworkEntity* go = Create::NetworkEntity("circle", Vector3(), Vector3(3, 3, 3));
	//go->SetID(newID);
	//clients_.insert(std::make_pair(addr, go));

	//std::cout << "New guy, assigned id " << newID << std::endl;
}

void Server::SendDisconnectionNotification(SystemAddress& addr)
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

void Server::ProcessInitialPosition(SystemAddress& addr, float x_, float y_, int type_)
{
	unsigned char msgid;
	RakNet::BitStream bs;
	ClientMap::iterator itr = clients_.find(addr);
	if (itr == clients_.end())
		return;

	itr->second->SetPosition(Vector3(x_, y_, 0));

	//std::cout << "Received pos" << itr->second.x_ << " " << itr->second.y_ << std::endl;

	msgid = ID_NEWSHIP;
	bs.Write(msgid);
	bs.Write(itr->second->GetID());
	bs.Write(itr->second->GetPosition().x);
	bs.Write(itr->second->GetPosition().y);

	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, true);
}

void Server::AnnounceNewObject(NetworkEntity* obj)
{
	++newID;
	obj->SetID(newID);
	
	RakNet::BitStream bs;
	unsigned char msgid = ID_NEWOBJ;

	bs.Write(msgid);//message ID
	//bs.Write(obj->GetEntityType());//entity type
	obj->WriteInit(bs);//required entity variables
	bs.Write(obj->GetID());

	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Server::AnnounceCollision(NetworkEntity* a, NetworkEntity* b)
{
	RakNet::BitStream bs;
	unsigned char msgid = ID_COLLIDE;
	bs.Write(msgid);

	bs.Write(a->GetID());
	bs.Write(a->GetPosition().x);
	bs.Write(a->GetPosition().y);
	bs.Write(a->GetVelocity().x);
	bs.Write(a->GetVelocity().y);

	bs.Write(b->GetID());
	bs.Write(b->GetPosition().x);
	bs.Write(b->GetPosition().y);
	bs.Write(b->GetVelocity().x);
	bs.Write(b->GetVelocity().y);

	rakpeer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}