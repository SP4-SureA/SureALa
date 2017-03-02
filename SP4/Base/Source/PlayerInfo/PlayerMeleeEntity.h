#ifndef _PLAYER_MELEE_ENTITY_BASE_H
#define _PLAYER_MELEE_ENTITY_BASE_H

#include "Vector3.h"
#include "PlayerEntityBase.h"

class PlayerMeleeEntity : public PlayerEntityBase
{
public:
    PlayerMeleeEntity(Mesh* _modelMesh);
    virtual ~PlayerMeleeEntity();

	virtual void UpdateAnimation(double dt);

    virtual void UpdateInputs(double dt);
    virtual void Update(double dt);
    virtual void Render();
	virtual void UseSpecial();

	void MovementInputUpdate(double dt);
	void WeaponInputUpdate(double dt);

	inline void SetShield(WeaponBase* _shield){ this->shield = _shield; };
	inline WeaponBase* GetShield(void){ return this->shield; };

protected:
	Vector3 lastDirection;
	WeaponBase* shield;

};

namespace Create
{
    PlayerMeleeEntity* playerMeleeEntity(
        EntityManager* em,
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
		float _fireRate = 0,
		float _damage = 0,
        const Vector3& _position = Vector3(0, 0, 0) ,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );

	PlayerMeleeEntity* playerMeleeAsset(
        const std::string& _meshName,
        float _moveSpeed = 0,
        float _maxSpeed = 0,
		float _fireRate = 0,
		float _damage = 0,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );
};

#endif // _PLAYER_MELEE_ENTITY_BASE_H