#include "PlayerRangeWeapon.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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

void PlayerRangeWeapon::Discharge()
{

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
