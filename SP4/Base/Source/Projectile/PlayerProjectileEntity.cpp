#include "PlayerProjectileEntity.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "..\PlayerInfo\PlayerInfo.h"
#include "..\PlayerInfo\PlayerEntityBase.h"
#include "..\Enemy\EnemyBase.h"
#include "..\Particle\ParticleScale.h"

#include "..\Physics\Physics.h"

PlayerProjectileEntity::PlayerProjectileEntity(Mesh* _modelMesh) :
ProjectileBase(_modelMesh)
{
}

PlayerProjectileEntity::~PlayerProjectileEntity()
{
    Create::particleScaleSprite(EntityManager::GetInstance(), "player_staff_hit_effect", "default", this->position, Vector3(3, 3, 1), 0.0f, 0.2f);
}

void PlayerProjectileEntity::Update(double dt)
{
    ProjectileBase::Update(dt);
}

void PlayerProjectileEntity::CollisionResponse(EntityBase* other, double dt)
{
	EnemyBase* enemy = dynamic_cast<EnemyBase*>(other);
	if (!enemy)
		return;

	//sound effect
	//display effect

	SetIsDead(true);
	enemy->TakeDamage(projectile_Damage, this);
}

void PlayerProjectileEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//modelStack.PushMatrix();
	//modelStack.Translate(position.x, position.y, position.z-0.2f);
	//modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	//modelStack.Scale(colliderSize.x, colliderSize.y, colliderSize.z);
	//RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("circle"));
	//modelStack.PopMatrix();

	float angle = Math::RadianToDegree(atan2(velocity.y, velocity.x));
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z - 0.1f);
	modelStack.Rotate(angle, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

PlayerProjectileEntity* Create::playerProjectileEntity(EntityManager* em,
    const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _veloctiy,
    const Vector3& _scale,
    const float& _damage,
    const float& _lifespan
    )
{
    if (em == NULL)
        return NULL;

    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    PlayerProjectileEntity* result = new PlayerProjectileEntity(modelMesh);

    result->SetPosition(_position);
    result->SetVelocity(_veloctiy);
    result->SetLifespan(_lifespan);
    result->SetScale(_scale * 3.0f);
    result->SetProjectileDamage(_damage);

    result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale);

    em->AddEntity(result, true);
    return result;
}

PlayerProjectileEntity* Create::playerProjectileAsset(
    const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _veloctiy,
    const Vector3& _scale,
    const float& _damage,
    const float& _lifespan
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    PlayerProjectileEntity* result = new PlayerProjectileEntity(modelMesh);

    result->SetPosition(_position);
    result->SetVelocity(_veloctiy);
    result->SetScale(_scale);
    result->SetProjectileDamage(_damage);
    result->SetLifespan(_lifespan);

	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(_scale);

    return result;
}