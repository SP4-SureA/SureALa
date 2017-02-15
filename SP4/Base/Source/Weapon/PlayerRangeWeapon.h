#ifndef _PLAYER_RANGE_WEAPON_H
#define _PLAYER_RANGE_WEAPON_H

#include "Vector3.h"
#include "WeaponBase.h"

// Forward declaration
namespace RakNet
{
	class BitStream;
};

class PlayerRangeWeapon : public WeaponBase
{
public:
    PlayerRangeWeapon();
    virtual ~PlayerRangeWeapon();

    virtual void Update(double dt);
    virtual void Discharge(Vector3 Direction);

protected:
};

namespace Create
{
    PlayerRangeWeapon* playerRangeWeapon(
        const float& _fireRate = 0,
        const float& _damage = 0
        );
};


#endif // _PLAYER_RANGE_WEAPON_H