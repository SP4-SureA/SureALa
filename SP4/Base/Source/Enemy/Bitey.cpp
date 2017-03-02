#include "Bitey.h"

#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "AnimationManager.h"
#include "RenderHelper.h"
#include "../Weapon/EnemyRangeWeapon.h"

#include "..\State\StateManager.h"
#include "EnemyBaseState_Patrol.h"
#include "BiteyState_Attack.h"

#include "..\SP4\GameInfo.h"
#include "..\PlayerInfo\PlayerInfo.h"
#include "..\PlayerInfo\PlayerEntityBase.h"
#include "..\Projectile\PlayerShieldEntity.h"
#include "..\Physics\Physics.h"
#include "..\Particle\ParticleScale.h"
#include "..\SoundManager.h"

Bitey::Bitey(Mesh* _modelMesh) :
EnemyBase(_modelMesh)
{
	animationPlayer.m_anim = AnimationManager::GetInstance("bitey")->GetAnimation("move");
	stateManager = new StateManager();
	stateManager->AttachEntity(this);
	stateManager->SetCurrentState(EnemyBaseState_Patrol::GetInstance());

	weight = 80.0f;
	attackPhaseMaxSpeed = 30.0f;
	attackCooldown = 1.5f;
	attackCooldownTimer = 0.0f;
	isCharging = false;
}

Bitey::~Bitey()
{
	if (stateManager)
		delete stateManager;
}

void Bitey::Update(double dt)
{
	EntityBase::Update(dt);

	{//Max speed
		float tempMaxSpeed = maxSpeed;
		if (stateManager->GetCurrentState() == BiteyState_Attack::GetInstance())
			tempMaxSpeed = attackPhaseMaxSpeed;
		if (!velocity.IsZero())//if zero speed, skip checks
		{
			if (velocity.LengthSquared() > tempMaxSpeed * tempMaxSpeed)
			{
				velocity -= velocity.Normalized() * (moveSpeed*1.1f) * dt;
				if (velocity.LengthSquared() < tempMaxSpeed * tempMaxSpeed)
					velocity = velocity.Normalized() * tempMaxSpeed;
			}
		}
	}

	position += velocity * dt;

	{//Update states for AI
		if (stateManager)
		{
			HandleState(dt);
			stateManager->Update(dt);
		}
	}

	animationPlayer.Update(dt); 
}

void Bitey::CollisionResponse(EntityBase* other, double dt)
{
	if (stateManager->GetCurrentState() != BiteyState_Attack::GetInstance())
		return;
	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);
	if (player)
	{
		Physics::CollisionResponse(this, other);
		//Create::particleScale(EntityManager::GetInstance(), "enemy_hit_effect", other->GetPosition(), Vector3(5, 5, 1), 0.2f);
		player->TakeDamage(1);
		stateManager->SetCurrentState(EnemyBaseState_Patrol::GetInstance());
		
		return;
	}
	PlayerShieldEntity* shield = dynamic_cast<PlayerShieldEntity*>(other);
	if (shield)
	{
		Physics::CollisionResponse(this, shield);
		shield->SetVelocity(Vector3());
		stateManager->SetCurrentState(EnemyBaseState_Patrol::GetInstance());
		
		return;
	}
}

void Bitey::HandleState(double dt)
{
	if (stateManager->GetCurrentState() == EnemyBaseState_Patrol::GetInstance())
	{
		if (attackCooldownTimer > 0.0f)
			attackCooldownTimer -= dt;
		PlayerEntityBase* target = GetPlayerInSight();
		if (target != NULL && attackCooldownTimer <= 0.0f)
		{
			Attack();
		}
	}
	else if (stateManager->GetCurrentState() == BiteyState_Attack::GetInstance())
	{
		if (isExitingBoundary())
		{
			Vector3 possy = GetNearestWaypoint();
			possy.y = maxBoundary.y + scale.y;
			this->position = possy;
			stateManager->SetCurrentState(EnemyBaseState_Patrol::GetInstance());
		}
	}
}

void Bitey::UpdateAnimation(double dt)
{
	if (stateManager->GetCurrentState() == EnemyBaseState_Patrol::GetInstance())
	{
		animationPlayer.m_anim = AnimationManager::GetInstance("bitey")->GetAnimation("move");
	}
	else if (stateManager->GetCurrentState() == BiteyState_Attack::GetInstance())
	{
		if (velocity.y < 0.0f)
		{
			animationPlayer.m_anim = AnimationManager::GetInstance("bitey")->GetAnimation("attack_downwards");
		}
		else
		{
			if (velocity.x > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("bitey")->GetAnimation("attack_right");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("bitey")->GetAnimation("attack_left");
		}
	}
}

void Bitey::Attack()
{
	attackCooldownTimer = attackCooldown;
	Charge(Vector3(0, -1, 0));
	stateManager->SetCurrentState(BiteyState_Attack::GetInstance());
}

void Bitey::ChargeFromDirection(CHARGE_DRECTION dir)
{
	Vector3 spawnPos = GameInfo::GetInstance()->GetRandomPlayer()->GetPosition();
	switch (dir)
	{
	case CHARGE_FROMTOP:
		spawnPos.y = maxBoundary.y + scale.y * 0.9f;
		Charge(Vector3(0, -1, 0));
		break;
	case CHARGE_FROMLEFT:
		spawnPos.x = minBoundary.x - scale.x * 0.9f;
		Charge(Vector3(-1, 0, 0));
		break;
	case CHARGE_FROMRIGHT:
		spawnPos.x = maxBoundary.x + scale.x * 0.9f;
		Charge(Vector3(1, 0, 0));
		break;
	default:break;
	}
	this->position = spawnPos;
}

void Bitey::Charge(Vector3 dir)
{
	if (dir.IsZero())
		return;

	SoundManager::GetInstance()->PlaySound2D("bitey_attack");
	isCharging = true;
	velocity = dir.Normalized() * attackPhaseMaxSpeed;
}

bool Bitey::isExitingBoundary()
{
	if ((position.x + scale.x <= minBoundary.x) && velocity.x < 0 ||
		(position.x - scale.x >= maxBoundary.x) && velocity.x > 0  ||
		(position.y + scale.y <= minBoundary.y) && velocity.y < 0  ||
		(position.y - scale.y >= maxBoundary.y) && velocity.y > 0
		)
	{
		return true;
	}
	else
		return false;
}

PlayerEntityBase* Bitey::GetPlayerInSight()
{
	PlayerEntityBase* nearestPlayer = NULL;
	float closestDistance = FLT_MAX;

	for (auto q : GameInfo::GetInstance()->GetPlayersList())
	{
		float xDistSquared = abs(q->GetCharacter()->GetPosition().x - this->position.x);
		if (xDistSquared <= sightWidth && xDistSquared < closestDistance)
		{
			nearestPlayer = q->GetCharacter();
			closestDistance = xDistSquared;
		}
	}

	return nearestPlayer;
}

void Bitey::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	
	//modelStack.PushMatrix();
	//modelStack.Translate(position.x, position.y, position.z - 0.1f);
	//modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	//modelStack.Scale(colliderSize.x, colliderSize.x, 1);
	//RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shadow"));
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

Bitey* Create::biteyEntity(
	EntityManager* em,
	const std::string& _meshName,
	float _moveSpeed,
	float _maxSpeed,
	float _damage,
	float _fireRate,
	float _health,
	const Vector3& _position,
	const Vector3& _scale
	)
{
    if (em == NULL)
        return NULL;
	
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    Bitey* result = new Bitey(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
	result->SetMaxSpeed(_maxSpeed);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale*0.5f);
	result->SetHealth(_health);
    em->AddEntity(result, true);

	result->SetSightWidth(_scale.y * 0.5f);

	EnemyRangeWeapon* _weapon = new EnemyRangeWeapon();
	_weapon->SetDamage(_damage);
	_weapon->SetFireRate(_fireRate);

	result->SetWeapon(_weapon);

    return result;
}

Bitey* Create::biteyAsset(const std::string& _meshName,
	float _moveSpeed,
	float _maxSpeed,
	float _damage,
	float _fireRate,
	float _health,
	const Vector3& _position,
	const Vector3& _scale
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    Bitey* result = new Bitey(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderOffset(Vector3(0, -_scale.y * 0.1f, 0));
	result->SetColliderSize(_scale*0.6f);
    result->SetMoveSpeed(_moveSpeed);
	result->SetMaxSpeed(_maxSpeed);
	result->SetHealth(_health);

	result->SetSightWidth(_scale.y * 0.5f);

	EnemyRangeWeapon* _weapon = new EnemyRangeWeapon();
	_weapon->SetDamage(_damage);
	_weapon->SetFireRate(_fireRate);

	result->SetWeapon(_weapon);
    return result;
}