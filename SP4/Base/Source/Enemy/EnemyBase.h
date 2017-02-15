#ifndef _ENEMY_ENTITY_BASE_H
#define _ENEMY_ENTITY_BASE_H

#include "Vector3.h"
#include "../RakNet/NetworkEntity.h"

// Forward declaration
namespace RakNet
{
	class BitStream;
};

class WeaponBase;

class EnemyBase : public NetworkEntity
{
public:
    EnemyBase(Mesh* _modelMesh);
    virtual ~EnemyBase();

    virtual void Update(double dt);
    virtual void Render();

	virtual void Read(RakNet::BitStream &bs);
	virtual void Write(RakNet::BitStream &bs);

	virtual void ReadInit(RakNet::BitStream &bs);
	virtual void WriteInit(RakNet::BitStream &bs);

    inline void SetMoveSpeed(const float& _value){ this->moveSpeed = _value; };
    inline float GetMoveSpeed(void){ return this->moveSpeed; };

	inline void SetWeapon(WeaponBase* _weapon){ this->weapon = _weapon; };
	inline WeaponBase* GetWeapon(void){ return this->weapon; };

protected:
    float moveSpeed;	
	WeaponBase* weapon;

};

namespace Create
{
    EnemyBase* EnemyEntity(
        EntityManager* em,
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
        const Vector3& _position = Vector3(0, 0, 0) ,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );

	EnemyBase* EnemyAsset(
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );
};

#endif // _ENEMY_ENTITY_BASE_H