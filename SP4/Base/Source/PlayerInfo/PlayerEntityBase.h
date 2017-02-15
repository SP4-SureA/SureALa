#ifndef _PLAYER_ENTITY_BASE_H
#define _PLAYER_ENTITY_BASE_H

#include "Vector3.h"
#include "../RakNet/NetworkEntity.h"

// Forward declaration
namespace RakNet
{
	class BitStream;
};

class WeaponBase;

class PlayerEntityBase : public NetworkEntity
{
public:
    PlayerEntityBase(Mesh* _modelMesh);
    virtual ~PlayerEntityBase();

    virtual void UpdateInputs(double dt);
    virtual void Update(double dt);
    virtual void Render();

	virtual void Read(RakNet::BitStream &bs);
	virtual void Write(RakNet::BitStream &bs);

	virtual void ReadInit(RakNet::BitStream &bs);
	virtual void WriteInit(RakNet::BitStream &bs);

    inline void SetMoveDir(const Vector3& _value){ this->moveDirection = _value; };
    inline void AddMoveDir(const Vector3& _value){ this->moveDirection += _value; };
    inline Vector3 GetMoveDir(void){ return this->moveDirection; };

	inline void SetShootDir(const Vector3& _dir){ this->shootDirection = _dir; };
	inline void AddShootDir(const Vector3& _dir){ this->shootDirection += _dir; };
	inline Vector3 GetShootDir(void){ return this->shootDirection; };

    inline void SetMoveSpeed(const float& _value){ this->moveSpeed = _value; };
    inline float GetMoveSpeed(void){ return this->moveSpeed; };

	inline void SetWeapon(WeaponBase* _weapon){ this->weapon = _weapon; };
	inline WeaponBase* GetWeapon(void){ return this->weapon; };

protected:
    float moveSpeed;
	
	WeaponBase* weapon;

    Vector3 moveDirection;
	Vector3 shootDirection;
};

namespace Create
{
    PlayerEntityBase* PlayerEntity(
        EntityManager* em,
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
        const Vector3& _position = Vector3(0, 0, 0) ,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );

    PlayerEntityBase* PlayerAsset(
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );
};

#endif // _PLAYER_ENTITY_BASE_H