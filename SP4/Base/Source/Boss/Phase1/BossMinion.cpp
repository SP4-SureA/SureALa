#include "BossMinion.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../../Projectile/PlayerMeleeHitboxEntity.h"
#include "../../Projectile/PlayerProjectileEntity.h"
#include "EntityManager.h"
#include "AnimationManager.h"
#include "../../SP4/GameInfo.h"
#include "../../PlayerInfo/PlayerEntityBase.h"
#include "../../Physics/Physics.h"

BossMinion::BossMinion(Mesh* _modelMesh) :
EnemyBase(_modelMesh),
damage(0)
{
}

BossMinion::~BossMinion()
{
}

void BossMinion::Update(double dt)
{
	EntityBase::Update(dt);
	animationPlayer.Update(dt);

	Vector3 direction = GameInfo::GetInstance()->GetNearestPlayer(this->position)->GetPosition() - this->position;

	if (direction.LengthSquared() > Math::EPSILON)
	{
		direction.Normalize();
		this->velocity += dt *this->moveSpeed * direction;

		{//Max speed
			if (!velocity.IsZero())//if zero speed, skip checks
			{
				if (velocity.LengthSquared() > maxSpeed * maxSpeed)
				{
					velocity -= velocity.Normalized() * (2 * moveSpeed) * dt;
					if (velocity.LengthSquared() < maxSpeed * maxSpeed)
						velocity = velocity.Normalized() * maxSpeed;
				}
			}
		}
	}

	position += velocity * dt;
}

void BossMinion::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

bool BossMinion::TakeDamage(float amount, EntityBase* other)
{
	return EnemyBase::TakeDamage(amount, other);
}

void BossMinion::CollisionResponse(EntityBase* other, double dt)
{
	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);

	if (player)
	{
		Physics::CollisionResponse(this, other);
		player->TakeDamage(1);
		return;
	}
}

BossMinion* Create::bossMinionEntity(
	EntityManager* em,
	const std::string& _meshName,
	const std::string& _animationName,
	const Vector3& _position,
	const Vector3& _scale,
    const float& _health,
    const float& _moveSpeed,
    const float& _maxSpeed,
    const float& _damage
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	BossMinion* minion = new BossMinion(modelMesh);

    minion->SetPosition(_position);
    minion->SetScale(_scale);
    minion->SetColliderSize(_scale);
    minion->SetColliderType(Collider::COLLIDER_SPHERE);

    minion->SetHealth(_health);
    minion->SetMoveSpeed(_moveSpeed);
    minion->SetMaxSpeed(_maxSpeed);
    minion->SetDamage(_damage);

	minion->SetAnimation(_meshName, _animationName);

	em->AddEntity(minion, true);

	return minion;
}

BossMinion* Create::bossMinionAsset(
	const std::string& _meshName,
	const std::string& _animationName,
	const Vector3& _position,
	const Vector3& _scale,
    const float& _health,
    const float& _moveSpeed,
    const float& _maxSpeed,
    const float& _damage
	)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	BossMinion* minion = new BossMinion(modelMesh);

	minion->SetPosition(_position);
	minion->SetScale(_scale);
    minion->SetColliderSize(_scale);
	minion->SetColliderType(Collider::COLLIDER_SPHERE);

	minion->SetHealth(_health);
	minion->SetMoveSpeed(_moveSpeed);
    minion->SetMaxSpeed(_maxSpeed);
	minion->SetDamage(_damage);

	minion->SetAnimation(_meshName, _animationName);

	return minion;
}