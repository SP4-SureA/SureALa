#ifndef _ENEMY_BITEY_ENTITY_H
#define _ENEMY_BITEY_ENTITY_H

#include "Vector3.h"
#include "EnemyBase.h"

class WeaponBase;
class PlayerEntityBase;

class Bitey : public EnemyBase
{
public:
	enum CHARGE_DRECTION
	{
		CHARGE_FROMTOP,
		CHARGE_FROMLEFT,
		CHARGE_FROMRIGHT,

		CHARGE_END
	};
    Bitey(Mesh* _modelMesh);
    virtual ~Bitey();

	virtual void UpdateAnimation(double dt);
    virtual void Update(double dt);
    virtual void Render();
	virtual void CollisionResponse(EntityBase* other, double dt);

	bool isExitingBoundary();

	virtual void Charge(Vector3 dir);
	virtual void ChargeFromDirection(CHARGE_DRECTION dir);
	virtual void Attack();

	virtual void HandleState(double dt);

	inline void SetSightWidth(float width){ this->sightWidth = width; }
	inline float GetSightWidth(){ return this->sightWidth; }

	inline void SetAttackPhaseMaxSpeed(float speed){ this->attackPhaseMaxSpeed = speed; }
	inline float GetAttackPhaseMaxSpeed(){ return this->attackPhaseMaxSpeed; }

	inline void SetIsCharging(bool _charging){ this->isCharging = _charging; }
	inline bool GetIsCharging(){ return this->isCharging; }

	inline Vector3 GetMinBoundary(){ return this->minBoundary; }
	inline Vector3 GetMaxBoundary(){ return this->maxBoundary; }
	inline void SetBoundary(Vector3 min, Vector3 max){ this->minBoundary = min; this->maxBoundary = max; }

protected:
	PlayerEntityBase* GetPlayerInSight();
	
	PlayerEntityBase* target;
	float sightWidth;
	Vector3 minBoundary;
	Vector3 maxBoundary;

	float attackPhaseMaxSpeed;
	float attackCooldown;
	float attackCooldownTimer;
	bool isCharging;
};

namespace Create
{
	Bitey* biteyEntity(
        EntityManager* em,
        const std::string& _meshName,
        float _moveSpeed = 0,
		float _maxSpeed = 0,
		float _damage = 0,
		float _fireRate = 0,
		float _health = 0,
        const Vector3& _position = Vector3(0, 0, 0) ,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );

	Bitey* biteyAsset(
        const std::string& _meshName,
        float _moveSpeed = 0,
		float _maxSpeed = 0,
		float _damage = 0,
		float _fireRate = 0,
		float _health = 0,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );
};

#endif // _ENEMY_RANGE_ENTITY_H