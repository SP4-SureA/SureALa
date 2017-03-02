#include "PlayerShieldEntity.h"

#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "EnemyProjectileEntity.h"
#include "..\Particle\ParticleScale.h"

PlayerShieldEntity::PlayerShieldEntity(Mesh* _modelMesh) :
ProjectileBase(_modelMesh)
{
}

PlayerShieldEntity::~PlayerShieldEntity()
{
}

void PlayerShieldEntity::Update(double dt)
{
	ProjectileBase::Update(dt);
}

void PlayerShieldEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	//modelStack.PushMatrix();
	//modelStack.Translate(position.x, position.y, position.z);
	//modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	//modelStack.Scale(colliderSize.x, colliderSize.x, colliderSize.z);
	//RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("circle"));
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z+0.1f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void PlayerShieldEntity::CollisionResponse(EntityBase* other, double dt)
{
	EnemyProjectileEntity* projectile = dynamic_cast<EnemyProjectileEntity*>(other);

	if (projectile)
	{
		ParticleBase* particle = Create::particleScale(EntityManager::GetInstance(), "player_shield_block_effect", projectile->GetPosition(), Vector3(4, 4, 1), 0.1f);
		Vector3 dir = projectile->GetPosition() - this->position;
		particle->SetRotate(Math::RadianToDegree(atan2(dir.y, dir.x)) - 90);
		other->SetIsDead(true);
	}
}

PlayerShieldEntity* Create::playerShieldEntity(EntityManager* em,
    const std::string& _meshName,
    const Vector3& _position,
	const Vector3& _front,
    const Vector3& _scale,
	const float& _lifespan,
    const float& _damage
    )
{
    if (em == NULL)
        return NULL;

    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    PlayerShieldEntity* result = new PlayerShieldEntity(modelMesh);

    result->SetPosition(_position);
	result->SetFront(_front);
	result->SetScale(_scale);
    result->SetProjectileDamage(_damage);
	result->SetLifespan(_lifespan);

    result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale);

    em->AddEntity(result, true);
    return result;
}

PlayerShieldEntity* Create::playerShieldAsset(const std::string& _meshName,
    const Vector3& _position,
	const Vector3& _front,
    const Vector3& _scale,
	const float& _lifespan,
    const float& _damage
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    PlayerShieldEntity* result = new PlayerShieldEntity(modelMesh);

    result->SetPosition(_position);
	result->SetFront(_front);
    result->SetScale(_scale);
    result->SetProjectileDamage(_damage);
	result->SetLifespan(_lifespan);

	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale);

    return result;
}
