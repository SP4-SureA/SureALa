#include "ScrollingEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

ScrollingEntity::ScrollingEntity(Mesh* _modelMesh) :
modelMesh(_modelMesh),
scrollSpeed(1.0f),
scrollDir(Vector3(1, 0, 0))
{
}

ScrollingEntity::~ScrollingEntity()
{
}

void ScrollingEntity::Update(double dt)
{
	position += scrollDir * scrollSpeed * dt;

	if (position.x < minBoundary.x)
	{
		position.x += (maxBoundary.x - minBoundary.x);
	}
	else if (position.x > maxBoundary.x)
	{
		position.x -= (maxBoundary.x - minBoundary.x);
	}

	if (position.y < minBoundary.y)
	{
		position.y += (maxBoundary.y - minBoundary.y);
	}
	else if (position.y > maxBoundary.y)
	{
		position.y -= (maxBoundary.y - minBoundary.y);
	}
}

void ScrollingEntity::RenderUI()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

ScrollingEntity* Create::scrollingEntity(EntityManager* em,
	const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scrollDirection,
	const float& _scrollSpeed,
	const Vector3& _scrollBoundaryMin,
	const Vector3& _scrollBoundaryMax,
	const Vector3& _scale)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ScrollingEntity* result = new ScrollingEntity(modelMesh);
	result->SetPosition(_position);

	if (!_scrollDirection.IsZero())
		result->SetScrollDirection(_scrollDirection.Normalized());

	result->SetScrollSpeed(_scrollSpeed);
	result->SetMinBoundary(_scrollBoundaryMin);
	result->SetMaxBoundary(_scrollBoundaryMax);
	result->SetScale(_scale);

	em->AddEntity(result, true);
	return result;
}
