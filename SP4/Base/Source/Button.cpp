#include "Button.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

Button::Button(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

Button::~Button()
{
}

void Button::Update(double dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

bool Button::CheckCollision(Vector3 pos)
{
	if (pos.x < position.x + scale.x * 0.5f &&
		pos.x > position.x - scale.x * 0.5f &&
		pos.y < position.y + scale.y * 0.5f &&
		pos.y > position.y - scale.y * 0.5f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Button::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);

	modelStack.PopMatrix();
}

Button* Create::button(EntityManager* em, const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	if (em == NULL)
		return NULL;
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Button* result = new Button(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	em->AddEntity(result, true);
	return result;
}

Button* Create::AssetButton(EntityManager* em, const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Button* result = new Button(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	return result;
}
