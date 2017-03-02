#include "PlayerRangeEntity.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "AnimationManager.h"

#include "..\Weapon\PlayerRangeWeapon.h"

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
	//MovementInputUpdate(dt);
	//WeaponInputUpdate(dt);
}

void PlayerRangeEntity::Update(double dt)
{
	if (moveDirection.IsZero())
	{
		//Apply friction
		if (!velocity.IsZero())
		{
			velocity -= velocity.Normalized() * moveSpeed * 0.5f * dt;
			if (velocity.LengthSquared() < (moveSpeed * moveSpeed) * dt)
				velocity.SetZero();
		}
	}
	else
	{
		moveDirection.Normalize();
		AddVelocity(moveDirection * moveSpeed * dt);
		{//Max speed
			if (!velocity.IsZero())//if zero speed, skip checks
			{
				if (velocity.LengthSquared() > maxSpeed * maxSpeed)
				{
					velocity -= velocity.Normalized() * (moveSpeed * 1.1f) * dt;
					if (velocity.LengthSquared() < maxSpeed * maxSpeed)
						velocity = velocity.Normalized() * maxSpeed;
				}
			}
		}
	}

	if (weapon)
	{
		if (!shootDirection.IsZero())
			weapon->Discharge(shootDirection.Normalize());

		weapon->SetPosition(this->position);
		weapon->Update(dt);
	}

	PlayerEntityBase::Update(dt);
}

void PlayerRangeEntity::MovementInputUpdate(double dt)
{
	moveDirection.SetZero();

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
	shootDirection.SetZero();

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
}

void PlayerRangeEntity::UpdateAnimation(double dt)
{
	if (shootDirection.IsZero())
	{
		//PlayerEntityBase::UpdateAnimation(dt);
		if (moveDirection.IsZero())//no moving
		{
			animationPlayer.m_pause = true;
		}
		else//yes moving
		{
			animationPlayer.m_pause = false;
			if (abs(moveDirection.x) > abs(moveDirection.y))//horizontal direction
			{
				if (moveDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("left");
			}
			else if (abs(moveDirection.y) > abs(moveDirection.x)) //vertical direction
			{
				if (moveDirection.y > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("up");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("down");
			}
			else//diagonal direction
			{
				if (moveDirection.y > 0)//top-diagonal
				{
					if (moveDirection.x > 0)
						animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("up_right");
					else
						animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("up_left");
				}
				else
				{
					if (moveDirection.x > 0)
						animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("down_right");
					else
						animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("down_left");
				}
			}
		}
	}
	else
	{
		animationPlayer.m_pause = false;
		if (abs(shootDirection.x) > abs(shootDirection.y))//horizontal direction
		{
			if (shootDirection.x > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_right");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_left");
		}
		else if (abs(shootDirection.y) > abs(shootDirection.x)) //vertical direction
		{
			if (shootDirection.y > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_up");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_down");
		}
		else//diagonal direction
		{
			if (shootDirection.y > 0)//top-diagonal
			{
				if (shootDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_up_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_up_left");
			}
			else
			{
				if (shootDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_down_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_staff")->GetAnimation("staff_down_left");
			}
		}
	}

	animationPlayer.Update(dt);
}

void PlayerRangeEntity::Render()
{
    PlayerEntityBase::Render();
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
	result->SetDefaultScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(Vector3(_scale.y* 0.5f, _scale.y * 0.5f, 1));
	result->SetColliderOffset(Vector3(0, -_scale.y * 0.92f, 0));
    em->AddEntity(result, true);

	PlayerRangeWeapon* _weapon = new PlayerRangeWeapon();
	_weapon->SetDamage(_damage);
	_weapon->SetFireRate(_fireRate);

	result->SetWeapon(_weapon);
    std::cout << _damage << std::endl;
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
	result->SetDefaultScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(Vector3(_scale.y* 0.5f, _scale.y * 0.5f, 1));
	result->SetColliderOffset(Vector3(0, -_scale.y * 0.92f, 0));

	PlayerRangeWeapon* _weapon = new PlayerRangeWeapon();
	_weapon->SetDamage(_damage);
	_weapon->SetFireRate(_fireRate);

	result->SetWeapon(_weapon);

    return result;
}
