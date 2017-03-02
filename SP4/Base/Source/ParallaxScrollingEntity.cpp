//#include "ParallaxScrollingEntity.h"
//#include "MeshBuilder.h"
//#include "EntityManager.h"
//#include "GraphicsManager.h"
//#include "RenderHelper.h"
//
//ParallaxScrollingEntity::ParallaxScrollingEntity():
//scrollValue(0.0f),
//scrollSpeed(1.0f),
//scrollDir(Vector3(1, 0, 0))
//{
//}
//
//ParallaxScrollingEntity::~ParallaxScrollingEntity()
//{
//}
//
//void ParallaxScrollingEntity::Update(double dt)
//{
//
//}
//
//void ParallaxScrollingEntity::Render()
//{
//	for (int i = 0; i < meshList.size(); ++i)
//	{
//		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//
//		Vector3 pos = position + scrollDir * (scrollValue / (meshList.size() + 0.1f - i));
//
//		modelStack.PushMatrix();
//		modelStack.Translate(pos.x, pos.y, 0);
//		modelStack.Scale(scale.x, scale.y, scale.z);
//
//		modelStack.PushMatrix();
//		modelStack.Translate(0, 0.5f, 0);
//		RenderHelper::RenderMesh(meshList[i]);
//		modelStack.PopMatrix();
//
//		modelStack.PushMatrix();
//		modelStack.Translate(0, -0.5f, 0);
//		RenderHelper::RenderMesh(meshList[i]);
//		modelStack.PopMatrix();
//
//		modelStack.PopMatrix();
//	}
//	//for (int i = 0; i < backgroundList.size(); ++i)
//	//{
//	//	float xOffset = -mainCamera->target.x / ((backgroundList.size()) + 0.1f - i);
//	//	float yOffset = -mainCamera->target.y / ((backgroundList.size()) + 0.1f - i);
//
//	//	if (abs(xOffset) > 1)
//	//	{
//	//		xOffset -= Math::GetSign(xOffset);
//	//	}
//	//	if (abs(yOffset) > 1)
//	//	{
//	//		yOffset -= Math::GetSign(yOffset);
//	//	}
//
//	//	modelStack.PushMatrix();
//	//	modelStack.Translate(xOffset / m_worldWidth, yOffset / m_worldHeight, 0);
//	//	RenderMesh(backgroundList[i], false);
//
//	//	modelStack.PushMatrix();
//	//	modelStack.Translate(1, 0, 0);
//	//	RenderMesh(backgroundList[i], false);
//	//	modelStack.PopMatrix();
//
//	//	modelStack.PushMatrix();
//	//	modelStack.Translate(0, 1, 0);
//	//	RenderMesh(backgroundList[i], false);
//	//	modelStack.PopMatrix();
//
//	//	modelStack.PushMatrix();
//	//	modelStack.Translate(1, 1, 0);
//	//	RenderMesh(backgroundList[i], false);
//	//	modelStack.PopMatrix();
//
//	//	modelStack.PopMatrix();
//	//}
//	//MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//	//modelStack.PushMatrix();
//	//modelStack.Translate(position.x, position.y, position.z);
//	//modelStack.Scale(scale.x, scale.y, scale.z);
//	//RenderHelper::RenderMesh(modelMesh);
//	//modelStack.PopMatrix();
//}
//
//ParallaxScrollingEntity* Create::parallaxScrollingEntity(EntityManager* em,
//	const Vector3& _position,
//	const Vector3& _scrollDirection,
//	const float& _scrollSpeed,
//	const Vector3& _scrollBoundaryMin,
//	const Vector3& _scrollBoundaryMax,
//	const Vector3& _scale)
//{
//	if (em == NULL)
//		return NULL;
//
//	ParallaxScrollingEntity* result = new ParallaxScrollingEntity();
//	result->SetPosition(_position);
//	if (!_scrollDirection.IsZero())
//		result->SetScrollDirection(_scrollDirection.Normalized());
//	result->SetScrollSpeed(_scrollSpeed);
//	result->SetMinBoundary(_scrollBoundaryMin);
//	result->SetMaxBoundary(_scrollBoundaryMax);
//	result->SetScale(_scale);
//	result->SetHasCollider(false);
//	em->AddEntity(result, true);
//	return result;
//}
