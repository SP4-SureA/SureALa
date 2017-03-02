#include "EnvironmentObject.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

EnvironmentObject::EnvironmentObject(Mesh* _modelMesh):
modelMesh(_modelMesh)
{
}

EnvironmentObject::~EnvironmentObject()
{
}

void EnvironmentObject::Update(double dt)
{
	animationPlayer.Update(dt);
}

void EnvironmentObject::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

EnvironmentObject* Create::environmentObject(
	EntityManager* em,
	const std::string& _meshName,
	const std::string& animation,
	const Vector3& _position,
	const Vector3& _scale
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	EnvironmentObject* result = new EnvironmentObject(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetAnimation(_meshName, animation);
	em->AddEntity(result, true);

	return result;
}