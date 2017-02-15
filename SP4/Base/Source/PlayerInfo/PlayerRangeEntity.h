#ifndef _PLAYER_RANGE_ENTITY_BASE_H
#define _PLAYER_RANGE_ENTITY_BASE_H

#include "Vector3.h"
#include "PlayerEntityBase.h"

// Forward declaration
namespace RakNet
{
	class BitStream;
};

class PlayerRangeEntity : public PlayerEntityBase
{
public:
    PlayerRangeEntity(Mesh* _modelMesh);
    virtual ~PlayerRangeEntity();

	virtual void UpdateAnimation(double dt);

    virtual void UpdateInputs(double dt);
    virtual void Update(double dt);
    virtual void Render();

	virtual void Read(RakNet::BitStream &bs);
	virtual void Write(RakNet::BitStream &bs);

	virtual void ReadInit(RakNet::BitStream &bs);
	virtual void WriteInit(RakNet::BitStream &bs);

	void MovementInputUpdate(double dt);
	void WeaponInputUpdate(double dt);

protected:

};

namespace Create
{
    PlayerRangeEntity* playerRangeEntity(
        EntityManager* em,
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
		float _fireRate = 0,
		float _damage = 0,
        const Vector3& _position = Vector3(0, 0, 0) ,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );

	PlayerRangeEntity* playerRangeAsset(
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
		float _fireRate = 0,
		float _damage = 0,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );
};

#endif // _PLAYER_RANGE_ENTITY_BASE_H