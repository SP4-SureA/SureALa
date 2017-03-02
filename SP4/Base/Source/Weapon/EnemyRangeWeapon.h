#ifndef _ENEMY_RANGE_WEAPON_H
#define _ENEMY_RANGE_WEAPON_H

#include "Vector3.h"
#include "WeaponBase.h"

class EnemyRangeWeapon : public WeaponBase
{
public:
    EnemyRangeWeapon();
    virtual ~EnemyRangeWeapon();

    virtual void Update(double dt);
	virtual void Discharge(Vector3 Direction);

protected:
};

namespace Create
{
    EnemyRangeWeapon* enemyRangeWeapon(
        const float& _fireRate = 0,
        const float& _damage = 0
        );
};


#endif // _ENEMY_RANGE_WEAPON_H