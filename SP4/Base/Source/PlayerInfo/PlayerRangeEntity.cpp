#include "PlayerRangeEntity.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Weapon/PlayerRangeWeapon.h"

PlayerRangeEntity::PlayerRangeEntity(Mesh* _modelMesh) :
PlayerEntityBase(_modelMesh)
{
}

PlayerRangeEntity::~PlayerRangeEntity()
{
	if (weapon)
		delete weapon;
}

void PlayerRangeEntity::UpdateInputs(double dt)
{
	MovementInputUpdate(dt);
	WeaponInputUpdate(dt);
}

void PlayerRangeEntity::Update(double dt)
{
	if (moveDirection.IsZero())
	{
		this->velocity.Lerp(0, dt * 10);
	}
	else
	{
		SetVelocity(moveSpeed * moveDirection * dt);
		moveDirection.SetZero();
	}

	ClampSpeed();

	this->position += this->velocity;

	if (weapon)
	{
		weapon->Update(dt);
	}
}

void PlayerRangeEntity::MovementInputUpdate(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown('W'))
	{
		AddMoveDir(Vector3(0, 1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown('A'))
	{
		AddMoveDir(Vector3(-1, 0, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown('S'))
	{
		AddMoveDir(Vector3(0, -1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown('D'))
	{
		AddMoveDir(Vector3(1, 0, 0));
	}
}

void PlayerRangeEntity::WeaponInputUpdate(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
	{
		AddShootDir(Vector3(0, 1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
	{
		AddShootDir(Vector3(-1, 0, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	{
		AddShootDir(Vector3(0, -1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		AddShootDir(Vector3(1, 0, 0));
	}

	if (!shootDirection.IsZero())
		weapon->Discharge(shootDirection.Normalize());

	shootDirection.SetZero();
}

void PlayerRangeEntity::Render()
{
    GenericEntity::Render();
}

void PlayerRangeEntity::Read(RakNet::BitStream &bs){}

void PlayerRangeEntity::Write(RakNet::BitStream &bs){}

void PlayerRangeEntity::ReadInit(RakNet::BitStream &bs)
{

}

void PlayerRangeEntity::WriteInit(RakNet::BitStream &bs)
{
}

PlayerRangeEntity* Create::playerRangeEntity(EntityManager* em,
    const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
	float _fireRate,
	float _damage,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    if (em == NULL)
        return NULL;
	
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    PlayerRangeEntity* result = new PlayerRangeEntity(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetHasCollider(false);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    em->AddEntity(result, true);

	PlayerRangeWeapon* _weapon = new PlayerRangeWeapon();
	_weapon->SetDamage(_damage);
	_weapon->SetFireRate(_fireRate);

	result->SetWeapon(_weapon);

    return result;
}

PlayerRangeEntity* Create::playerRangeAsset(const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
	float _fireRate,
	float _damage,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    PlayerRangeEntity* result = new PlayerRangeEntity(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    result->SetHasCollider(false);

	PlayerRangeWeapon* _weapon = new PlayerRangeWeapon();
	_weapon->SetDamage(_damage);
	_weapon->SetFireRate(_fireRate);

	result->SetWeapon(_weapon);

    return result;
}
