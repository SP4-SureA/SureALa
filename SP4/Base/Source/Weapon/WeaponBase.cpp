#include "WeaponBase.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

WeaponBase::WeaponBase() : 
b_canFire(false),
weaponDamage(0),
fireRate(0),
bulletDelay(0)
{
}

WeaponBase::~WeaponBase()
{
}

void WeaponBase::Update(double dt)
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