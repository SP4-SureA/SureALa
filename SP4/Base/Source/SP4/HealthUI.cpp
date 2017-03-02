#include "HealthUI.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "GameInfo.h"

HealthUI::HealthUI(Mesh* _modelMesh) :
modelMesh(_modelMesh)
{
}

HealthUI::~HealthUI()
{
}

void HealthUI::RenderUI()
{
	if (!b_DoRender)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	float halfScaleX = scale.x * 0.5f;
	float halfScaleY = scale.y * 0.5f;

	for (int i = 0; i < GameInfo::GetInstance()->GetHealth(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(halfScaleX + (scale.x * i), canvasSize.y - halfScaleY, position.z + 1);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
}

HealthUI* Create::healthUIEntity(EntityManager* em,
	const std::string& _meshName,
	const Vector3& _scale,
	const Vector3& _canvasSize
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	HealthUI* result = new HealthUI(modelMesh);
	result->SetCanvasSize(_canvasSize);
	result->SetScale(_scale);
	em->AddEntity(result, true);
	return result;
}

HealthUI* Create::healthUIAsset(
	const std::string& _meshName,
	const Vector3& _scale,
	const Vector3& _canvasSize
	)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	HealthUI* result = new HealthUI(modelMesh);
	result->SetCanvasSize(_canvasSize);
	result->SetScale(_scale);
	return result;
}
