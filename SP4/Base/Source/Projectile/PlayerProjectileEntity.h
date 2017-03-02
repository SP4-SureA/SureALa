#ifndef _PLAYER_PROJECTILE_ENTITY_H
#define _PLAYER_PROJECTILE_ENTITY_H

#include "Vector3.h"
#include "ProjectileBase.h"

class PlayerProjectileEntity : public ProjectileBase
{
public:
    PlayerProjectileEntity(Mesh* _modelMesh);
    virtual ~PlayerProjectileEntity();

    virtual void Update(double dt);
	virtual void CollisionResponse(EntityBase* other, double dt);
    virtual void Render();

protected:

};

namespace Create
{
    PlayerProjectileEntity* playerProjectileEntity(
        EntityManager* em,
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _veloctiy = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const float& _damage = 0, 
        const float& _lifespan = 3
        );

    PlayerProjectileEntity* playerProjectileAsset(
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _veloctiy = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const float& _damage = 0,
        const float& _lifespan = 3
        );
};

#endif // _PLAYER_PROJECTILE_ENTITY_H