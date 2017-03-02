#include "PlayerRangeWeapon.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Projectile/PlayerProjectileEntity.h"

#include "Scene.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "..\SoundManager.h"

PlayerRangeWeapon::PlayerRangeWeapon()
{
}

PlayerRangeWeapon::~PlayerRangeWeapon()
{
}

void PlayerRangeWeapon::Update(double dt)
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

void PlayerRangeWeapon::Discharge(Vector3 Direction)
{
	if (!b_canFire)
		return;
	else
		b_canFire = false;

	SoundManager::GetInstance()->PlaySound2D("player_range_attack");
	PlayerProjectileEntity* proj = Create::playerProjectileEntity(EntityManager::GetInstance(), "player_staff_projectile",
		position, (Direction * 30.0f), Vector3(1.0f, 1.0f, 1.0f), weaponDamage, 2);
}

PlayerRangeWeapon* playerRangeWeapon(
    const float& _fireRate,
    const float& _damage
    )
{
    PlayerRangeWeapon* weapon = new PlayerRangeWeapon();
    weapon->SetDamage(_damage);
    weapon->SetFireRate(_fireRate);

    return weapon;
}
