#ifndef _PLAYER_SHIELD_ENTITY_H
#define _PLAYER_SHIELD_ENTITY_H

#include "Vector3.h"
#include "ProjectileBase.h"

#include <vector>
using std::vector;

class PlayerShieldEntity : public ProjectileBase
{
public:
	PlayerShieldEntity(Mesh* _modelMesh);
	virtual ~PlayerShieldEntity();

	virtual void CollisionResponse(EntityBase* other, double dt);

    virtual void Update(double dt);
    virtual void Render();

protected:
	vector<EntityBase*> v_CollidedEntities;
};

namespace Create
{
	PlayerShieldEntity* playerShieldEntity(
        EntityManager* em,
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _front = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const float& _lifespan = FLT_MAX,
        const float& _damage = 0
        );

	PlayerShieldEntity* playerShieldAsset(
        const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _front = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const float& _lifespan = FLT_MAX,
        const float& _damage = 0
        );
};

#endif // _PLAYER_SHIELD_ENTITY_H