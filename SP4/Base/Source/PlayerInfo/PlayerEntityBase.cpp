#include "PlayerEntityBase.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

PlayerEntityBase::PlayerEntityBase(Mesh* _modelMesh) :
NetworkEntity(_modelMesh),
moveSpeed(0),
moveDirection(Vector3(0, 0, 0)),
shootDirection(Vector3(0, 0, 0)),
weapon(NULL)
{
}

PlayerEntityBase::~PlayerEntityBase()
{
}

void PlayerEntityBase::UpdateInputs(double dt)
{
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

void PlayerEntityBase::Update(double dt)
{
    if (moveDirection.IsZero())
    {
        this->velocity.Lerp(0, dt * 10);
    }
    else
    {
        SetVelocity(moveSpeed * moveDirection * dt);
        moveDirection.SetZero();
    }
    ClampSpeed();
    this->position += this->velocity;
}

void PlayerEntityBase::Render()
{
    GenericEntity::Render();
}

void PlayerEntityBase::Read(RakNet::BitStream &bs){}

void PlayerEntityBase::Write(RakNet::BitStream &bs){}

void PlayerEntityBase::ReadInit(RakNet::BitStream &bs)
{

}

void PlayerEntityBase::WriteInit(RakNet::BitStream &bs)
{
}

PlayerEntityBase* Create::PlayerEntity(EntityManager* em,
    const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    if (em == NULL)
        return NULL;
	
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    PlayerEntityBase* result = new PlayerEntityBase(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetHasCollider(false);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    em->AddEntity(result, true);
    return result;
}

PlayerEntityBase* Create::PlayerAsset(const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    PlayerEntityBase* result = new PlayerEntityBase(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    result->SetHasCollider(false);
    return result;
}
