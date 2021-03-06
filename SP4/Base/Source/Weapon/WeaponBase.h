#ifndef _WEAPON_BASE_H
#define _WEAPON_BASE_H

#include "Vector3.h"
#include "EntityBase.h"

class WeaponBase : public EntityBase
{
public:
    virtual ~WeaponBase();

    inline void SetDamage(const float& _damage){ this->weaponDamage = _damage; };
    inline float GetDamage(void){ return this->weaponDamage; };

    inline void SetFireRate(const float& _firerate){ this->fireRate = _firerate; };
    inline float GetFireRate(void){ return this->fireRate; };

    virtual void Update(double dt);
	virtual void Discharge(Vector3 Direction){};

	bool GetCanFire(){ return this->b_canFire; };

protected:
    WeaponBase();

    bool b_canFire;

    float weaponDamage;
    double fireRate;
    double bulletDelay;
};


#endif // _WEAPON_BASE_H