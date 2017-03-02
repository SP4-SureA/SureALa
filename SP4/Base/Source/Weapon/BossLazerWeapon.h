#ifndef _BOSS_LAZER_WEAPON_H
#define _BOSS_LAZER_WEAPON_H

#include "Vector3.h"
#include "WeaponBase.h"

#include <vector>
using std::vector;

class ProjectileBase;

class BossLazerWeapon : public WeaponBase
{
public:
    BossLazerWeapon();
    virtual ~BossLazerWeapon();

    virtual void Update(double dt);
	virtual void Discharge(Vector3 Direction);

    inline float GetChargeTime(){ return this->chargeTime; };
    inline void SetChargeTime(float _chargeTime){ this->chargeTime = _chargeTime; };

    inline float GetLazerLength(){ return this->lazerLength; };
    inline void SetLazerLength(float _length){ this->lazerLength = _length; };

    inline float GetLazerLifetime(){ return this->lazerLifeTime; };
    inline void SetLazerLifetime(float _lifetime){ this->lazerLifeTime = _lifetime; };

	inline unsigned int GetNumLazers(){ return this->v_LazerEntities.size(); };

protected:
    float chargeTime;
    float lazerLength;
    float lazerLifeTime;
    
	vector<ProjectileBase*> v_LazerEntities;
};

namespace Create
{
    BossLazerWeapon* bossLazerWeapon(
        const float& _fireRate = 0,
        const float& _chargeTime = 1.f,
        const float& _lazerLength = 1.f,
        const float& _lazerLifeTime = 1.f
        );
};


#endif // _BOSS_LAZER_WEAPON_H