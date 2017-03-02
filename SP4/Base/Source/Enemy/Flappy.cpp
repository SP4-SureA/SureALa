#include "Flappy.h"

#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "AnimationManager.h"
#include "RenderHelper.h"
#include "../Weapon/EnemyRangeWeapon.h"

#include "..\State\StateManager.h"
#include "EnemyBaseState_Patrol.h"
#include "EnemyBaseState_Attack.h"

#include "..\SP4\GameInfo.h"
#include "..\PlayerInfo\PlayerEntityBase.h"

Flappy::Flappy(Mesh* _modelMesh) :
EnemyBase(_modelMesh)
{
	animationPlayer.m_anim = AnimationManager::GetInstance("flappy")->GetAnimation("move");
	stateManager = new StateManager();
	stateManager->AttachEntity(this);
	stateManager->SetCurrentState(EnemyBaseState_Patrol::GetInstance());
}

Flappy::~Flappy()
{
	if (stateManager)
		delete stateManager;
}

void Flappy::Update(double dt)
{
	EnemyBase::Update(dt);

	animationPlayer.Update(dt);
}

void Flappy::HandleState(double dt)
{
	if (stateManager->GetCurrentState() == EnemyBaseState_Patrol::GetInstance())
	{
		PlayerEntityBase* target = GetPlayerInSight();

		if (weapon && weapon->GetCanFire() && target != NULL)
		{
			velocity.SetZero();
			stateManager->SetCurrentState(EnemyBaseState_Attack::GetInstance());
		}
	}
	else if (stateManager->GetCurrentState() == EnemyBaseState_Attack::GetInstance())
	{
		PlayerEntityBase* target = GetPlayerInSight();
		
		if (!weapon || target == NULL)//if no weapon or no target in sight, change state to patrol
			stateManager->SetCurrentState(EnemyBaseState_Patrol::GetInstance());
	}
}

void Flappy::UpdateAnimation(double dt)
{
	if (stateManager)
	{
		if (stateManager->GetCurrentState() == EnemyBaseState_Attack::GetInstance())
		{
			animationPlayer.m_anim = AnimationManager::GetInstance("flappy")->GetAnimation("attack");
		}
		else
		{
			animationPlayer.m_anim = AnimationManager::GetInstance("flappy")->GetAnimation("move");
		}
	}
}

void Flappy::Attack()
{
	weapon->Discharge(Vector3(0, -1, 0));
}

PlayerEntityBase* Flappy::GetPlayerInSight()
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

void Flappy::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	
	//modelStack.PushMatrix();
	//modelStack.Translate(position.x, position.y, position.z-0.1f);
	//modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	//modelStack.Scale(colliderSize.x, colliderSize.y, colliderSize.z);
	//RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("circle"));
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

Flappy* Create::flappyEntity(
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

    Flappy* result = new Flappy(modelMesh);
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

Flappy* Create::flappyAsset(const std::string& _meshName,
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

    Flappy* result = new Flappy(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale*0.5f);
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