#include "PlayerMeleeEntity.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "AnimationManager.h"

#include "../Weapon/PlayerMeleeWeapon.h"
#include "../Weapon/PlayerShieldWeapon.h"

PlayerMeleeEntity::PlayerMeleeEntity(Mesh* _modelMesh) :
PlayerEntityBase(_modelMesh),
lastDirection(Vector3(1, 0, 0)),
shield(nullptr)
{
}

PlayerMeleeEntity::~PlayerMeleeEntity()
{
	if (weapon)
		delete weapon;
}

void PlayerMeleeEntity::UpdateInputs(double dt)
{
	//MovementInputUpdate(dt);
	//WeaponInputUpdate(dt);
}

void PlayerMeleeEntity::Update(double dt)
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
		if (weapon->GetCanFire())
		{
			moveDirection.Normalize();
			lastDirection = moveDirection;
			AddVelocity(moveDirection * moveSpeed * dt);
			{//Max speed
				if (!velocity.IsZero())//if zero speed, skip checks
				{
					if (velocity.LengthSquared() > maxSpeed * maxSpeed)
					{
						velocity -= velocity.Normalized() * (moveSpeed) * 1.5f * dt;
						if (velocity.LengthSquared() < maxSpeed * maxSpeed)
							velocity = velocity.Normalized() * maxSpeed;
					}
				}
			}
		}
		else
		{
			velocity.SetZero();
		}
	}

	if (weapon)
	{
		if (!shootDirection.IsZero())
		{
			lastDirection = shootDirection.Normalized();
			weapon->Discharge(shootDirection.Normalize());
		}

		weapon->SetPosition(this->position + this->colliderOffset);
		weapon->Update(dt);
	}
	
	if (shield)
	{
		shield->SetPosition(this->position + this->colliderOffset);
		shield->Update(dt);
	}

	PlayerEntityBase::Update(dt);
}

void PlayerMeleeEntity::UseSpecial()
{
	if (shield)
		shield->Discharge(lastDirection.Normalize());
}

void PlayerMeleeEntity::MovementInputUpdate(double dt)
{
	moveDirection.SetZero();

	if (!weapon)
		return;

	if (!weapon->GetCanFire())
		return;

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

	//if (!moveDirection.IsZero())
	//{
	//	lastDirection = moveDirection.Normalized();
	//}
}

void PlayerMeleeEntity::WeaponInputUpdate(double dt)
{
	if (!weapon)
		return;

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

	if (!shield)
		return;

	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
	{
		shield->Discharge(lastDirection.Normalize());
	}
}

void PlayerMeleeEntity::UpdateAnimation(double dt)
{
	if (!shootDirection.IsZero() || (shield && ((PlayerShieldWeapon*)shield)->GetShield()))
	{
		if (shootDirection.IsZero())
		{
			if (!moveDirection.IsZero())
				shootDirection = moveDirection;
			else
				shootDirection = lastDirection;
		}

		animationPlayer.m_pause = false;
		if (abs(shootDirection.x) > abs(shootDirection.y))//horizontal direction
		{
			if (shootDirection.x > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_right");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_left");
		}
		else if (abs(shootDirection.y) > abs(shootDirection.x)) //vertical direction
		{
			if (shootDirection.y > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down");
		}
		else//diagonal direction
		{
			if (shootDirection.y > 0)//top-diagonal
			{
				if (shootDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up_left");
			}
			else
			{
				if (shootDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down_left");
			}
		}
	}
	else
	{
		PlayerEntityBase::UpdateAnimation(dt);
	}
	//if (shootDirection.IsZero())
	//{
	//	PlayerEntityBase::UpdateAnimation(dt);
	//}
	//else
	//{
	//	animationPlayer.m_pause = false;
	//	if (abs(shootDirection.x) > abs(shootDirection.y))//horizontal direction
	//	{
	//		if (shootDirection.x > 0)
	//			animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_right");
	//		else
	//			animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_left");
	//	}
	//	else if (abs(shootDirection.y) > abs(shootDirection.x)) //vertical direction
	//	{
	//		if (shootDirection.y > 0)
	//			animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up");
	//		else
	//			animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down");
	//	}
	//	else//diagonal direction
	//	{
	//		if (shootDirection.y > 0)//top-diagonal
	//		{
	//			if (shootDirection.x > 0)
	//				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up_right");
	//			else
	//				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up_left");
	//		}
	//		else
	//		{
	//			if (shootDirection.x > 0)
	//				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down_right");
	//			else
	//				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down_left");
	//		}
	//	}
	//}

	animationPlayer.Update(dt);
}

/*
	if (!shootDirection.IsZero() || (shield && ((PlayerShieldWeapon*)shield)->GetShield()))
	{
		if (shootDirection.IsZero())
		{
			if (!moveDirection.IsZero())
				shootDirection = moveDirection;
			else
				shootDirection = lastDirection;
		}

		animationPlayer.m_pause = false;
		if (abs(shootDirection.x) > abs(shootDirection.y))//horizontal direction
		{
			if (shootDirection.x > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_right");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_left");
		}
		else if (abs(shootDirection.y) > abs(shootDirection.x)) //vertical direction
		{
			if (shootDirection.y > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down");
		}
		else//diagonal direction
		{
			if (shootDirection.y > 0)//top-diagonal
			{
				if (shootDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_up_left");
			}
			else
			{
				if (shootDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("shield_down_left");
			}
		}
	}
	else
	{
		PlayerEntityBase::UpdateAnimation(dt);
	}
*/

void PlayerMeleeEntity::Render()
{
    PlayerEntityBase::Render();
}

PlayerMeleeEntity* Create::playerMeleeEntity(EntityManager* em,
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

    PlayerMeleeEntity* result = new PlayerMeleeEntity(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
	result->SetDefaultScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(Vector3(_scale.y* 0.5f, _scale.y * 0.5f, 1));
	result->SetColliderOffset(Vector3(0, -_scale.y * 0.92f, 0));
    em->AddEntity(result, true);

	WeaponBase* _weapon = Create::playerMeleeWeapon(_fireRate, _damage, 0.3f);
	WeaponBase* _shield = Create::playerShieldWeapon(5, _damage, 3);

	result->SetWeapon(_weapon);
	result->SetShield(_shield);
    return result;
}

PlayerMeleeEntity* Create::playerMeleeAsset(const std::string& _meshName,
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

    PlayerMeleeEntity* result = new PlayerMeleeEntity(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
	result->SetDefaultScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(Vector3(_scale.y* 0.5f, _scale.y * 0.5f, 1));
	result->SetColliderOffset(Vector3(0, -_scale.y * 0.92f, 0));

    WeaponBase* _weapon = Create::playerMeleeWeapon(_fireRate, _damage, 0.3f);
    WeaponBase* _shield = Create::playerShieldWeapon(5, _damage, 3);

	result->SetWeapon(_weapon);
	result->SetShield(_shield);

    return result;
}
