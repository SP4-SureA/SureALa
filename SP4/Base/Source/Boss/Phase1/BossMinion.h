#ifndef _BOSS_MINION_H
#define _BOSS_MINION_H

#include "../../Enemy/EnemyBase.h"
#include "Animation.h"

class BossMinion : public EnemyBase
{
public:
	BossMinion(Mesh* _modelMesh);
	virtual ~BossMinion();

	virtual void Update(double dt);
	virtual void Render();

    virtual bool TakeDamage(float amount, EntityBase* other = NULL);

	void SetDamage(int _damage){ this->damage = _damage; };
	int GetDamage(){ return this->damage; };

	virtual void CollisionResponse(EntityBase* other, double dt);
protected :
	int damage;
};

namespace Create
{
	BossMinion* bossMinionEntity(
		EntityManager* em,
		const std::string& _meshName,
		const std::string& _animationName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const float& _health = 10,
        const float& _moveSpeed = 0,
        const float& _maxSpeed = 0,
        const float& _damage = 0
		);

	BossMinion* bossMinionAsset(
		const std::string& _meshName,
		const std::string& _animationName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const float& _health = 10,
        const float& _moveSpeed = 0,
        const float& _maxSpeed = 0,
        const float& _damage = 0
		);

};


#endif // _BOSS_MINION_H