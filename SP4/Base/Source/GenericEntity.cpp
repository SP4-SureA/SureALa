#include "GenericEntity.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

GenericEntity::GenericEntity(Mesh* _modelMesh): 
modelMesh(_modelMesh),
maxSpeed(0)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double dt)
{
	EntityBase::Update(dt);
	position += velocity * dt;
}

void GenericEntity::UpdateInputs(double dt)
{
}

void GenericEntity::Render()
{
    if (b_DoRender)
    {
        MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
        modelStack.PushMatrix();
        modelStack.Translate(position.x, position.y, position.z);
        modelStack.Scale(scale.x, scale.y, scale.z);
        RenderHelper::RenderMesh(modelMesh);
        modelStack.PopMatrix();
    }
}

void GenericEntity::ClampSpeed()
{
    if (!velocity.IsZero())
    {
        if (velocity.LengthSquared() > maxSpeed * maxSpeed)
        {
            velocity.Normalize();
            velocity *= maxSpeed;
        }
    }
}

GenericEntity* Create::Entity(EntityManager* em, 
								const std::string& _meshName,
								const Vector3& _position,
								const Vector3& _scale)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	em->AddEntity(result, true);
	return result;
}

GenericEntity* Create::Asset(EntityManager* em, 
							 const std::string& _meshName,
							 const Vector3& _position,
							 const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	return result;
}
