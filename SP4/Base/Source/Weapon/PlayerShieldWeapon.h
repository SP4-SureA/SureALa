#ifndef _PLAYER_SHIELD_WEAPON_H
#define _PLAYER_SHIELD_WEAPON_H

#include "Vector3.h"
#include "WeaponBase.h"

class PlayerShieldWeapon : public WeaponBase
{
public:
    PlayerShieldWeapon();
    virtual ~PlayerShieldWeapon();

    virtual void Update(double dt);
	virtual void Discharge(Vector3 Direction);

	inline void SetShieldDuration(const float& _shieldDuration){ this->shieldDuration = _shieldDuration; };
	inline float GetShieldDuration(void){ return this->shieldDuration; };

	inline void SetShield(EntityBase* _shield){ this->shield = _shield; };
	inline EntityBase* GetShield(void){ return this->shield; };

protected:
	EntityBase* shield;
	float shieldDuration;

};

namespace Create
{
	PlayerShieldWeapon* playerShieldWeapon(
		const float& _fireRate = 0,
		const float& _damage = 0,
		const float& _shieldDuration = FLT_MAX
        );
};


#endif // _PLAYER_SHIELD_WEAPON_H