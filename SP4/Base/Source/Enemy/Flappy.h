#ifndef _ENEMY_FLAPPY_ENTITY_H
#define _ENEMY_FLAPPY_ENTITY_H

#include "Vector3.h"
#include "EnemyBase.h"

class WeaponBase;
class PlayerEntityBase;

class Flappy : public EnemyBase
{
public:
    Flappy(Mesh* _modelMesh);
    virtual ~Flappy();

	virtual void UpdateAnimation(double dt);
    virtual void Update(double dt);
    virtual void Render();

	virtual void Attack();

	virtual void HandleState(double dt);

	inline void SetSightWidth(float width){ this->sightWidth = width; }
	inline float GetSightWidth(){ return this->sightWidth; }

protected:
	PlayerEntityBase* GetPlayerInSight();
	float sightWidth;

};

namespace Create
{
	Flappy* flappyEntity(
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

	Flappy* flappyAsset(
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