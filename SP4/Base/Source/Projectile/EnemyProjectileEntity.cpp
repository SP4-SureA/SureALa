#include "EnemyProjectileEntity.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "..\PlayerInfo\PlayerEntityBase.h"
#include "..\Particle\ParticleScale.h"

EnemyProjectileEntity::EnemyProjectileEntity(Mesh* _modelMesh) :
ProjectileBase(_modelMesh)
{
}

EnemyProjectileEntity::~EnemyProjectileEntity()
{
}

void EnemyProjectileEntity::Update(double dt)
{
	EntityBase::Update(dt);
    this->position += velocity * dt;
}

void EnemyProjectileEntity::CollisionResponse(EntityBase* other, double dt)
{
	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);
	if (!player)
		return;

	//sound effect
	//display effect
	//ParticleBase* particle = Create::particleScale(EntityManager::GetInstance(), "enemy_hit_effect", this->position, Vector3(3, 3, 1), 0.2f);

	SetIsDead(true);

	player->TakeDamage(1);
}

void EnemyProjectileEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	//modelStack.PushMatrix();
	//modelStack.Translate(position.x, position.y, position.z);
	//modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	//modelStack.Scale(colliderSize.x, colliderSize.y, colliderSize.z);
	//RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("circle"));
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z+0.1f);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

EnemyProjectileEntity* Create::enemyProjectileEntity(EntityManager* em,
    const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _veloctiy,
    const Vector3& _scale,
    const float& _damage
    )
{
    if (em == NULL)
        return NULL;

    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    EnemyProjectileEntity* result = new EnemyProjectileEntity(modelMesh);

    result->SetPosition(_position);
    result->SetVelocity(_veloctiy);
    result->SetScale(_scale);
    result->SetProjectileDamage(_damage);
	result->SetAnimation(_meshName, "default");

	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale * 0.5f);

    em->AddEntity(result, true);
    return result;
}

EnemyProjectileEntity* Create::enemyProjectileAsset(const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _veloctiy,
    const Vector3& _scale,
    const float& _damage
    )
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	EnemyProjectileEntity* result = new EnemyProjectileEntity(modelMesh);

	result->SetPosition(_position);
	result->SetVelocity(_veloctiy);
	result->SetScale(_scale);
	result->SetProjectileDamage(_damage);
	result->SetAnimation(_meshName, "default");

	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale * 0.5f);

	return result;
}
