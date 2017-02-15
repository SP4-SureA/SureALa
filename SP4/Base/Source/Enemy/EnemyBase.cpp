#include "EnemyBase.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

EnemyBase::EnemyBase(Mesh* _modelMesh) :
NetworkEntity(_modelMesh),
moveSpeed(0),
weapon(NULL)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update(double dt)
{
}

void EnemyBase::Render()
{
    GenericEntity::Render();
}

void EnemyBase::Read(RakNet::BitStream &bs){}

void EnemyBase::Write(RakNet::BitStream &bs){}

void EnemyBase::ReadInit(RakNet::BitStream &bs)
{

}

void EnemyBase::WriteInit(RakNet::BitStream &bs)
{
}

EnemyBase* Create::EnemyEntity(EntityManager* em,
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

    EnemyBase* result = new EnemyBase(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetHasCollider(false);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    em->AddEntity(result, true);
    return result;
}

EnemyBase* Create::EnemyAsset(const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    EnemyBase* result = new EnemyBase(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    result->SetHasCollider(false);
    return result;
}
