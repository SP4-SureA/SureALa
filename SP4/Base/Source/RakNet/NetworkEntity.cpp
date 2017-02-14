#include "NetworkEntity.h"

#include "Bitstream.h"

NetworkEntity::NetworkEntity(Mesh* _modelMesh) :
GenericEntity(_modelMesh),
ratio_(1),
ID(0),
oldx(0),
oldy(0),
clientPos(0),
serverPos(0),
serverVel(0)
{
}

NetworkEntity::~NetworkEntity()
{
}

void NetworkEntity::Read(RakNet::BitStream &bs){}
void NetworkEntity::Write(RakNet::BitStream &bs){}

void NetworkEntity::ReadInit(RakNet::BitStream &bs){}
void NetworkEntity::WriteInit(RakNet::BitStream &bs){}

void NetworkEntity::UpdateInterpolation(double dt)
{
	serverPos += serverVel * dt;

	clientPos += velocity * dt;

	position = ratio_ * serverPos + (1 - ratio_) * clientPos;

	if (ratio_ < 1)
	{
		ratio_ += dt * 4;
		if (ratio_ > 1)
			ratio_ = 1;
	}
}

void NetworkEntity::DoInterpolation()
{
	clientPos = position;
	velocity = serverVel;
	ratio_ = 0;
}