#ifndef _BOSS_RAPID_FIRE_WEAPON_H
#define _BOSS_RAPID_FIRE_WEAPON_H

#include "Vector3.h"
#include "WeaponBase.h"

#include <vector>
using std::vector;

class ProjectileBase;

class BossRapidFireWeapon : public WeaponBase
{
public:
    BossRapidFireWeapon();
    virtual ~BossRapidFireWeapon();

    virtual void Update(double dt);
	virtual void Discharge(Vector3 Direction);

    void UpdateDischarge(double dt);
    void DischargeWave(double dt, Vector3 direction = Vector3(0, -1, 0));

    inline unsigned int GetNumberOfWaves(){ return this->numWaves; };
    inline void SetNumberOfWaves(unsigned int _count){ this->numWaves = _count; };

    inline unsigned int GetCurrentWaveNumber(){ return this->currentWave; };
    inline void SetCurrentWaveNumber(unsigned int _count){ this->currentWave = _count; };

    inline unsigned int GetNumberOfBulletsPerWave(){ return this->numBullets; };
    inline void SetNumberOfBulletsPerWave(unsigned int _count){ this->numBullets = _count; };

    inline float GetTimeBetweenWave(){ return this->timeBetweenWaves; };
    inline void SetTimeBetweenWave(float _duration){ this->timeBetweenWaves = _duration; };

protected:
    unsigned int numWaves;
    unsigned int numBullets;
    unsigned int currentWave;

    float timeBetweenWaves;
    float currentTimeBetweenWave;
};

namespace Create
{
    BossRapidFireWeapon* bossRapidFireWeapon(
        const unsigned int& _numWaves = 0,
        const unsigned int& _numBullets = 1,
        const float& _timeBetweenWaves = 1.f
        );
};


#endif // _BOSS_RAPID_FIRE_WEAPON_H