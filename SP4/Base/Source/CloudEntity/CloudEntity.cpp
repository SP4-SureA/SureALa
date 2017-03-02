#include "CloudEntity.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CloudEntity::CloudEntity(Mesh* _modelMesh) :
GenericEntity(_modelMesh)
{
}

CloudEntity::~CloudEntity()
{
}

void CloudEntity::Update(double dt)
{
	GenericEntity::Update(dt);

	if (this->position.y < 0 - scale.y)
	{
		this->SetIsDead(true);
	}

}

void CloudEntity::Render()
{
    GenericEntity::Render();
}

CloudEntity* Create::cloudEntity(EntityManager* em,
    const std::string& _meshName,
    const Vector3& _position,
	const Vector3& _velocity,
    const Vector3& _scale
    )
{
    if (em == NULL)
        return NULL;
	
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    CloudEntity* result = new CloudEntity(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
	result->SetVelocity(_velocity);

    em->AddEntity(result, true);
    return result;
}

CloudEntity* Create::cloudAsset(const std::string& _meshName,
    const Vector3& _position,
	const Vector3& _velocity,
    const Vector3& _scale
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    CloudEntity* result = new CloudEntity(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
	result->SetVelocity(_velocity);

    return result;
}
