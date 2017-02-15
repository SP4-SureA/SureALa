#include "NetworkEntity.h"

#include "MeshBuilder.h"
#include "Bitstream.h"
#include "EntityManager.h"

NetworkEntity::NetworkEntity(Mesh* _modelMesh) :
GenericEntity(_modelMesh),
ratio_(1),
ID(0),
oldx(0),
oldy(0),
clientPos(Vector3()),
serverPos(Vector3()),
serverVel(Vector3()),
handleUpdate(true)
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
		ratio_ += dt * 8;
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

NetworkEntity* Create::networkEntity(EntityManager* em,
	const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	if (em == NULL)
		return NULL;
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	NetworkEntity* result = new NetworkEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetHasCollider(false);
	em->AddEntity(result, true);
	return result;
}