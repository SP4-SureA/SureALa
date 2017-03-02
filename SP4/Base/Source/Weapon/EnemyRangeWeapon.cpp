#include "EnemyRangeWeapon.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Projectile/EnemyProjectileEntity.h"

#include "EntityManager.h"
#include "SceneManager.h"
#include "..\SoundManager.h"

EnemyRangeWeapon::EnemyRangeWeapon()
{
}

EnemyRangeWeapon::~EnemyRangeWeapon()
{
}

void EnemyRangeWeapon::Update(double dt)
{
	if (!b_canFire)
	{
		bulletDelay -= dt;

		if (bulletDelay < 0)
		{
			bulletDelay = fireRate;
			b_canFire = true;
		}
	}
}

void EnemyRangeWeapon::Discharge(Vector3 Direction)
{
	if (!b_canFire)
		return;
	else
		b_canFire = false;

	SoundManager::GetInstance()->PlaySound2D("floopy_attack");
	EnemyProjectileEntity* proj = Create::enemyProjectileEntity(EntityManager::GetInstance(),
		"projectile_enemy",
		position,
		(Direction * 30.0f),
		Vector3(2.0f, 2.0f, 1.0f),
		weaponDamage
		);
}

EnemyRangeWeapon* enemyRangeWeapon(
    const float& _fireRate,
    const float& _damage
    )
{
    EnemyRangeWeapon* weapon = new EnemyRangeWeapon();
    weapon->SetDamage(_damage);
    weapon->SetFireRate(_fireRate);

    return weapon;
}
