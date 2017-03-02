#include "FloorTile.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "FloorTilesManager.h"

FloorTile::FloorTile(Mesh* _modelMesh):
modelMesh(_modelMesh),
front(Vector3(1, 0, 0))
{
}

FloorTile::~FloorTile()
{
}

void FloorTile::Render()
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

FloorTile* Create::floorTileEntity(EntityManager* em,
	const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale,
	const Vector3& _front
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	FloorTile* result = new FloorTile(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetFront(_front);
	em->AddEntity(result, true);
	FloorTilesManager::GetInstance()->AddFloor(result);
	return result;
}

FloorTile* Create::floorTileAsset(
	const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale,
	const Vector3& _front
	)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	FloorTile* result = new FloorTile(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetFront(_front);
	FloorTilesManager::GetInstance()->AddFloor(result);
	return result;
}
