#ifndef _PROJECTILE_BASE_H
#define _PROJECTILE_BASE_H

#include "Vector3.h"
#include "../RakNet/NetworkEntity.h"

// Forward declaration
namespace RakNet
{
	class BitStream;
};

class ProjectileBase : public NetworkEntity
{
public:
    virtual ~ProjectileBase();

    virtual void Update(double dt);
    virtual void Render();

    inline void SetProjectileDamage(const float& _damage){ this->projectile_Damage = _damage; };
    inline float GetProjectileDamage(void){ return this->projectile_Damage; };

protected:
    ProjectileBase(Mesh* _modelMesh);

    float projectile_Damage;
};


#endif // _PROJECTILE_BASE_H