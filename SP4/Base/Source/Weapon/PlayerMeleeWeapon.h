#ifndef _PLAYER_MELEE_WEAPON_H
#define _PLAYER_MELEE_WEAPON_H

#include "Vector3.h"
#include "WeaponBase.h"

class PlayerMeleeWeapon : public WeaponBase
{
public:
    PlayerMeleeWeapon();
    virtual ~PlayerMeleeWeapon();

    virtual void Update(double dt);
	virtual void Discharge(Vector3 Direction);

	inline void SetAttackDuration(const float& _attackDuration){ this->attackDuration = _attackDuration; };
	inline float GetAttackDuration(void){ return this->attackDuration; };

protected:
	float attackDuration;
    EntityBase* meleeHitbox;
	
};

namespace Create
{
	PlayerMeleeWeapon* playerMeleeWeapon(
		const float& _fireRate = 0,
		const float& _damage = 0,
		const float& _attackDuration = FLT_MAX
        );
};


#endif // _PLAYER_MELEE_WEAPON_H