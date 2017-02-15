#include "PlayerProjectileEntity.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

PlayerProjectileEntity::PlayerProjectileEntity(Mesh* _modelMesh) :
ProjectileBase(_modelMesh)
{
}

PlayerProjectileEntity::~PlayerProjectileEntity()
{
}

void PlayerProjectileEntity::Update(double dt)
{
    this->position += velocity * dt;
}

void PlayerProjectileEntity::Render()
{
    GenericEntity::Render();
}

PlayerProjectileEntity* Create::playerProjectileEntity(EntityManager* em,
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

    PlayerProjectileEntity* result = new PlayerProjectileEntity(modelMesh);

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

PlayerProjectileEntity* playerProjectileAsset(
    const std::string& _meshName,
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

    PlayerProjectileEntity* result = new PlayerProjectileEntity(modelMesh);

    result->SetPosition(_position);
    result->SetVelocity(_veloctiy);
    result->SetScale(_scale);
    result->SetID(ID);
    result->SetProjectileDamage(_damage);

    result->SetColliderType(COLLIDER_SPHERE);
    result->SetHasCollider(true);

    return result;
}
