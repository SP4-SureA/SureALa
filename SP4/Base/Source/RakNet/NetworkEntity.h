#ifndef NETWORK_ENTITY_H
#define NETWORK_ENTITY_H

#include "Vector3.h"
#include "..\GenericEntity.h"

// Forward declaration
class Mesh;
namespace RakNet
{
	class BitStream;
};

class NetworkEntity : public GenericEntity
{
public:
	NetworkEntity(Mesh* _modelMesh);
	virtual ~NetworkEntity();

	virtual void Read(RakNet::BitStream &bs);
	virtual void Write(RakNet::BitStream &bs);

	virtual void ReadInit(RakNet::BitStream &bs);
	virtual void WriteInit(RakNet::BitStream &bs);

	inline void SetID(unsigned int newID){ this->ID = newID; }
	inline unsigned int GetID(){ return this->ID; }

	inline void SetClientPos(Vector3 pos){ this->clientPos = pos; }
	inline void SetClientPos(float x, float y, float z = 0.0f){ this->clientPos.Set(x, y, z); }
	inline Vector3 GetClientPos(){ return this->clientPos; }

	inline void SetServerPos(Vector3 pos){ this->serverPos = pos; }
	inline void SetServerPos(float x, float y, float z = 0.0f){ this->serverPos.Set(x, y, z); }
	inline Vector3 GetServerPos(){ return this->serverPos; }

	inline void SetServerVel(Vector3 vel){ this->serverVel = vel; }
	inline void SetServerVel(float x, float y, float z = 0.0f){ this->serverVel.Set(x, y, z); }
	inline Vector3 GetServerVel(){ return this->serverVel; }

	virtual void UpdateInterpolation(double dt);
	virtual void DoInterpolation();

protected:
	bool handleUpdate;
	unsigned int ID;

	float oldx, oldy;	// for reset back to previous location if collision detected

	Vector3 clientPos;
	Vector3 serverPos;
	Vector3 serverVel;
	float ratio_;
};

namespace Create
{
	NetworkEntity* networkEntity(EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif