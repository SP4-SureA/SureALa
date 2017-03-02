#ifndef _PLAYER_ENTITY_BASE_H
#define _PLAYER_ENTITY_BASE_H

#include "Vector3.h"
#include "../GenericEntity.h"
#include "Animation.h"

class WeaponBase;

class PlayerEntityBase : public GenericEntity
{
public:
	virtual ~PlayerEntityBase();

	virtual void UpdateAnimation(double dt);

	virtual void UpdateInputs(double dt);
	virtual void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);
	virtual void Update(double dt);
	virtual void Render();

	virtual void TakeDamage(int amount);
	void Respawn();

	inline void SetMoveDir(const Vector3& _value){ this->moveDirection = _value; };
	inline void AddMoveDir(const Vector3& _value){ if (hasDropped) return; this->moveDirection += _value; };
	inline Vector3 GetMoveDir(void){ return this->moveDirection; };

	inline void SetShootDir(const Vector3& _dir){ this->shootDirection = _dir; };
	inline void AddShootDir(const Vector3& _dir){ if (hasDropped) return; this->shootDirection += _dir; };
	inline Vector3 GetShootDir(void){ return this->shootDirection; };

	inline void SetSpawnPoint(const Vector3& _point){ this->spawnPoint = _point; }
	inline Vector3 GetSpawnPoint(){ return this->spawnPoint; }

	inline void SetHasDropped(bool _bool){ this->hasDropped = _bool; }
	inline bool GetHasDropped(){ return this->hasDropped; }

	inline void SetDefaultScale(const Vector3& _scale){ this->defaultScale = _scale; }
	inline Vector3 GetDefaultScale(){ return this->defaultScale; }

	virtual void UseSpecial(){}

	inline void SetMoveSpeed(const float& _value){ this->moveSpeed = _value; };
	inline float GetMoveSpeed(void){ return this->moveSpeed; };

	inline void SetWeapon(WeaponBase* _weapon){ this->weapon = _weapon; };
	inline WeaponBase* GetWeapon(void){ return this->weapon; };

	AnimationPlayer animationPlayer;
protected:
	PlayerEntityBase(Mesh* _modelMesh);

	void Drop();
	virtual void HandleFloorCollision(double dt);

	float bufferBlinkShowTimer;
	float bufferBlinkHideTimer;
	float bufferBlinkTimer;
	float bufferTime;
	float moveSpeed;

	bool blinking;
	bool hasDropped;

	WeaponBase* weapon;

	Vector3 spawnPoint;
	Vector3 defaultScale;
	Vector3 moveDirection;
	Vector3 shootDirection;
};

#endif // _PLAYER_ENTITY_BASE_H