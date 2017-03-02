#ifndef _ENEMY_FLOOPY_ENTITY_H
#define _ENEMY_FLOOPY_ENTITY_H

#include "Vector3.h"
#include "EnemyBase.h"

class WeaponBase;
class PlayerEntityBase;

class Floopy : public EnemyBase
{
public:
    Floopy(Mesh* _modelMesh);
    virtual ~Floopy();

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
	Floopy* floopyEntity(
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

	Floopy* floopyAsset(
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