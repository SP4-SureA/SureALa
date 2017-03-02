#ifndef _ENEMY_PROJECTILE_ENTITY_H
#define _ENEMY_PROJECTILE_ENTITY_H

#include "Vector3.h"
#include "ProjectileBase.h"

class EnemyProjectileEntity : public ProjectileBase
{
public:
    EnemyProjectileEntity(Mesh* _modelMesh);
    virtual ~EnemyProjectileEntity();

    virtual void Update(double dt);
	virtual void CollisionResponse(EntityBase* other, double dt);
    virtual void Render();

protected:

};

namespace Create
{
    EnemyProjectileEntity* enemyProjectileEntity(
        EntityManager* em,
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _veloctiy = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const float& _damage = 0
        );

    EnemyProjectileEntity* enemyProjectileAsset(
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _veloctiy = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const float& _damage = 0
        );
};

#endif // _ENEMY_PROJECTILE_ENTITY_H