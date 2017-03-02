#include "PlayerShieldWeapon.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Projectile/PlayerShieldEntity.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "..\SoundManager.h"

PlayerShieldWeapon::PlayerShieldWeapon() :
shieldDuration(0),
shield(NULL)
{
}

PlayerShieldWeapon::~PlayerShieldWeapon()
{
}

void PlayerShieldWeapon::Update(double dt)
{
	WeaponBase::Update(dt);

	if (shield)
	{
		shield->SetPosition(position);

		if (shield->GetIsDead() || shield->GetIsDone())
			this->shield = NULL;	
	}
}

void PlayerShieldWeapon::Discharge(Vector3 Direction)
{
	if (!b_canFire)
		return;
	else
		b_canFire = false;

	SoundManager::GetInstance()->PlaySound2D("player_melee_shield");

	if (Direction.LengthSquared() >  Math::EPSILON)
	{
		Direction.Normalize();
	}
	else
	{
		return;
	}

	shield = Create::playerShieldEntity(EntityManager::GetInstance(), 
		"player_shield_guard",
		this->position + (Direction * 5.0f),
		Direction,
		Vector3(5, 5, 1),
		shieldDuration);
}

PlayerShieldWeapon* Create::playerShieldWeapon(
	const float& _fireRate,
	const float& _damage,
	const float& _shieldDuration
	)
{
    PlayerShieldWeapon* weapon = new PlayerShieldWeapon();
    weapon->SetDamage(_damage);
    weapon->SetFireRate(_fireRate);
	weapon->SetShieldDuration(_shieldDuration);

    return weapon;
}
