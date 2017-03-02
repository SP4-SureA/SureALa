#include "BossRapidFireWeapon.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Projectile/EnemyProjectileEntity.h"


#include "Scene.h"
#include "EntityManager.h"
#include "SceneManager.h"

BossRapidFireWeapon::BossRapidFireWeapon() :
numWaves(0),
numBullets(1),
currentWave(0),
timeBetweenWaves(1.f),
currentTimeBetweenWave(0.f)
{
}

BossRapidFireWeapon::~BossRapidFireWeapon()
{
}

void BossRapidFireWeapon::Update(double dt)
{
    WeaponBase::Update(dt);
    UpdateDischarge(dt);
}

void BossRapidFireWeapon::UpdateDischarge(double dt)
{
    if (currentTimeBetweenWave > timeBetweenWaves)
    {
        if (currentWave < numWaves)
        {
            std::cout << "curr wave : " << currentWave << "max wave : " << numWaves << std::endl;
            DischargeWave(dt, front);
            ++currentWave;
        }
        currentTimeBetweenWave = 0;
    }
    else
    {
        currentTimeBetweenWave += dt;
    }
}

void BossRapidFireWeapon::DischargeWave(double dt, Vector3 direction)
{
    float angle = 180 / numBullets;

    Mtx44 rotate;
    for (int i = 0; i < numBullets; ++i)
    {
        rotate.SetToZero();
        rotate.SetToRotation(angle * i - 90, 0, 0, 1);

        Create::enemyProjectileEntity(EntityManager::GetInstance(),
            "projectile_enemy",
            position,
            (rotate * direction) * 30.0f,
            Vector3(2.0f, 2.0f, 1.0f),
            weaponDamage
            );
    }
}

void BossRapidFireWeapon::Discharge(Vector3 direction)
{
    if (!b_canFire)
        return;
    else
        b_canFire = false;

    if (direction.LengthSquared() > Math::EPSILON)
    {
        this->front = direction.Normalized();
        currentWave = 0;
        currentTimeBetweenWave = 0;
    }
}

BossRapidFireWeapon* Create::bossRapidFireWeapon(
    const unsigned int& _numWaves,
    const unsigned int& _numBullets,
    const float& _timeBetweenWaves
    )
{
    BossRapidFireWeapon* weapon = new BossRapidFireWeapon();
    weapon->SetNumberOfWaves(_numWaves);
    weapon->SetCurrentWaveNumber(_numWaves);
    weapon->SetNumberOfBulletsPerWave(_numBullets);
    weapon->SetTimeBetweenWave(_timeBetweenWaves);

    return weapon;
}
