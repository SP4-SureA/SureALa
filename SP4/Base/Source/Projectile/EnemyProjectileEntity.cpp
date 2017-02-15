#include "EnemyProjectileEntity.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

EnemyProjectileEntity::EnemyProjectileEntity(Mesh* _modelMesh) :
ProjectileBase(_modelMesh)
{
}

EnemyProjectileEntity::~EnemyProjectileEntity()
{
}

void EnemyProjectileEntity::Update(double dt)
{
    this->position += velocity * dt;
}

void EnemyProjectileEntity::Render()
{
    GenericEntity::Render();
}

EnemyProjectileEntity* Create::enemyProjectileEntity(EntityManager* em,
    const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _veloctiy,
    const Vector3& _scale,
    const int& ID,
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
    result->SetID(ID);
    result->SetProjectileDamage(_damage);

    result->SetColliderType(COLLIDER_SPHERE);
    result->SetHasCollider(true);

    em->AddEntity(result, true);
    return result;
}

EnemyProjectileEntity* Create::playerProjectileAsset(const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _veloctiy,
    const Vector3& _scale,
    const int& ID,
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
    result->SetID(ID);
    result->SetProjectileDamage(_damage);

    result->SetColliderType(COLLIDER_SPHERE);
    result->SetHasCollider(true);

    return result;
}
