#include "Wall.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "..\Physics\Physics.h"
#include "..\PlayerInfo\PlayerEntityBase.h"
#include "..\Projectile\PlayerProjectileEntity.h"
#include "..\Projectile\EnemyProjectileEntity.h"

Wall::Wall(Mesh* _modelMesh) :
modelMesh(_modelMesh)
{
}

Wall::~Wall()
{
}

void Wall::Update(double dt)
{
	velocity.SetZero();
}

void Wall::CollisionResponse(EntityBase* other, double dt)
{
	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);
	PlayerProjectileEntity* playerProj = dynamic_cast<PlayerProjectileEntity*>(other);
	EnemyProjectileEntity* enemyProj = dynamic_cast<EnemyProjectileEntity*>(other);

	if (player)
	{
		Physics::CollisionResponse(other, this);
	}
	else if (playerProj)
	{
		playerProj->SetIsDead(true);
	}
	else if (enemyProj)
	{
		enemyProj->SetIsDead(true);
	}
}

void Wall::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	float degree = Math::RadianToDegree(atan2(front.y, front.x));

	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
	modelStack.Rotate(degree, 0, 0, 1);
	modelStack.Scale(colliderSize.x, colliderSize.y, colliderSize.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

Wall* Create::squareWall(
	EntityManager* em,
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

	Wall* result = new Wall(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetFront(_front);
	result->SetColliderType(Collider::COLLIDER_BOX);
	result->SetColliderSize(_scale);
	em->AddEntity(result, true);

	return result;
}

Wall* Create::roundWall(
	EntityManager* em,
	const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	Wall* result = new Wall(modelMesh);
	result->SetPosition(_position);
	result->SetScale(Vector3());
	result->SetScale(Vector3(_scale.x, _scale.x, 1));
	result->SetColliderType(Collider::COLLIDER_SPHERE);
	result->SetColliderSize(Vector3(_scale.x, _scale.x, 1));
	em->AddEntity(result, true);
	
	return result;
}