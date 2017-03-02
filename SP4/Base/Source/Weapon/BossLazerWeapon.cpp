#include "BossLazerWeapon.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Projectile/BossLazerStart.h"

#include "Scene.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "..\SoundManager.h"

BossLazerWeapon::BossLazerWeapon() :
chargeTime(0),
lazerLength(1),
lazerLifeTime(1)
{
}

BossLazerWeapon::~BossLazerWeapon()
{
}

void BossLazerWeapon::Update(double dt)
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

	if (!v_LazerEntities.empty())
    {
		vector<ProjectileBase*>::iterator it = v_LazerEntities.begin();
		vector<ProjectileBase*>::iterator end = v_LazerEntities.end();

		for (it = v_LazerEntities.begin(); it != v_LazerEntities.end();)
		{
			EntityBase* temp = (*it);

			if (temp->GetIsDead() || temp->GetIsDone())
			{
				it = v_LazerEntities.erase(it);
			}
			else
			{
				temp->SetPosition(this->position);
				++it;
			}
		}
    }
}

void BossLazerWeapon::Discharge(Vector3 Direction)
{
	if (!b_canFire)
		return;
	else
		b_canFire = false;

	SoundManager::GetInstance()->PlaySound2D("skywhale_chargeLaser");

	Mtx44 rotate;
	rotate.SetToRotation(45, 0, 0, 1);

	v_LazerEntities.push_back(
		Create::bossLazerStartEntity(
		EntityManager::GetInstance(),
		"boss_lazer",
		"charging",
		this->position,
		Vector3(10.f, 10.f, 10.f),
		Direction,
		lazerLifeTime,
		chargeTime,
		lazerLength
		)
		);

	v_LazerEntities.push_back(
		Create::bossLazerStartEntity(
		EntityManager::GetInstance(),
		"circle",
		"anim",
		this->position,
		Vector3(10.f, 10.f, 10.f),
		rotate * Direction,
		lazerLifeTime,
		chargeTime,
		lazerLength
		)
		);

	rotate.SetToZero();
	rotate.SetToRotation(-45, 0, 0, 1);
	
	v_LazerEntities.push_back(
		Create::bossLazerStartEntity(
		EntityManager::GetInstance(),
		"circle",
		"anim",
		this->position,
		Vector3(10.f, 10.f, 10.f),
		rotate * Direction,
		lazerLifeTime,
		chargeTime,
		lazerLength
		)
		);
}

BossLazerWeapon* Create::bossLazerWeapon(
    const float& _fireRate,
    const float& _chargeTime,
    const float& _lazerLength,
    const float& _lazerLifeTime

    )
{
    BossLazerWeapon* weapon = new BossLazerWeapon();

    weapon->SetFireRate(_fireRate);
    weapon->SetChargeTime(_chargeTime);
    weapon->SetLazerLength(_lazerLength);
    weapon->SetLazerLifetime(_lazerLifeTime);

    return weapon;
}
