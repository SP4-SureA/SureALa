#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
	//for testing only
	position += velocity * dt;
}

void GenericEntity::Render()
{
	if (detailLevel == NO_DETAILS)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		RenderHelper::RenderMesh(GetLODMesh());                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	}
	else
		RenderHelper::RenderMesh(modelMesh);

	modelStack.PopMatrix();
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
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
	result->SetHasCollider(false);
	em->AddEntity(result, true);
	return result;
}

GenericEntity* Create::Asset(EntityManager* em, 
							 const std::string& _meshName,
							 const Vector3& _position,
							 const Vector3& _scale)
{
	//if (em == NULL)
	//	return NULL;
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetHasCollider(false);
	//em->AddEntity(result);
	return result;
}