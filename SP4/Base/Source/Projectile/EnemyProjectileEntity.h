#ifndef _ENEMY_PROJECTILE_ENTITY_H
#define _ENEMY_PROJECTILE_ENTITY_H

#include "Vector3.h"
#include "ProjectileBase.h"

// Forward declaration
namespace RakNet
{
	class BitStream;
};

class EnemyProjectileEntity : public ProjectileBase
{
public:
    EnemyProjectileEntity(Mesh* _modelMesh);
    virtual ~EnemyProjectileEntity();

    virtual void Update(double dt);
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
        const int& ID = 0,
        const float& _damage = 0
        );

    EnemyProjectileEntity* playerProjectileAsset(
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _veloctiy = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const int& ID = 0,
        const float& _damage = 0
        );
};

#endif // _ENEMY_PROJECTILE_ENTITY_H