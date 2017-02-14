//#include "Enemy.h"
//
//#include "EntityManager.h"
//#include "GraphicsManager.h"
//#include "RenderHelper.h"
//
//Enemy::Enemy() :
//GenericEntity(NULL),
//defaultPosition(Vector3(0.0f, 0.0f, 0.0f)),
//defaultTarget(Vector3(0.0f, 0.0f, 0.0f)),
//defaultUp(Vector3(0.0f, 0.0f, 0.0f)),
//target(Vector3(0.0f, 0.0f, 0.0f)),
//up(Vector3(0.0f, 0.0f, 0.0f)),
//maxBoundary(Vector3(0.0f, 0.0f, 0.0f)),
//minBoundary(Vector3(0.0f, 0.0f, 0.0f)),
//terrain(NULL)
//{
//}
//
//Enemy::~Enemy()
//{
//}
//
//void Enemy::Init()
//{
//	defaultPosition.Set(0, 0, 10);
//	defaultTarget.Set(0, 0, 0);
//	defaultUp.Set(0, 1, 0);
//
//	position.Set(10.0f, 0.0f, 0.0f);
//	target.Set(10.0f, 0.0f, 450.0f);
//	up.Set(0.0f, 1.0f, 0.0f);
//
//	maxBoundary.Set(1, 1, 1);
//	minBoundary.Set(-1, -1, -1);
//
//	speed = 1.0f;
//
//	InitLOD("cube", "sphere", "cube");
//
//	EntityManager::GetInstance()->AddEntity(this, true);
//}
//
//void Enemy::Update(double dt)
//{
//	Vector3 viewVector = (target - position).Normalized();
//	position += viewVector * speed * dt;
//
//	Constrain();
//
//	if (position.z > 400.0f)
//		target.z = position.z * -1;
//	else if (position.z < -400.0f)
//		target.z = position.z * -1;
//}
//
//void Enemy::Render()
//{
//	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
//	modelStack.PushMatrix();
//	modelStack.Translate(position.x, position.y, position.z);
//	modelStack.Scale(scale.x, scale.y, scale.z);
//	if (GetLODStatus() == true)
//	{
//		if (detailLevel != NO_DETAILS)
//		{
//			RenderHelper::RenderMesh(GetLODMesh());
//		}
//	}
//	modelStack.PopMatrix();
//}
//
//void Enemy::Constrain()
//{
//	if (position.x > maxBoundary.x - 1.0f)
//		position.x = maxBoundary.x - 1.0f;
//	if (position.z > maxBoundary.z - 1.0f)
//		position.z = maxBoundary.z - 1.0f;
//	if (position.x < minBoundary.x - 1.0f)
//		position.x = minBoundary.x - 1.0f;
//	if (position.z < minBoundary.z - 1.0f)
//		position.z = minBoundary.z - 1.0f;
//
//	if (position.y != terrain->GetTerrainHeight(position))
//		position.y = terrain->GetTerrainHeight(position);
//}
//
//void Enemy::Reset()
//{
//	position = defaultPosition;
//	target = defaultTarget;
//	up = defaultUp;
//}
//
//void Enemy::SetPosition(const Vector3& pos)
//{
//	this->position = pos;
//}
//void Enemy::SetTarget(const Vector3& target)
//{
//	this->target = target;
//}
//void Enemy::SetUp(const Vector3& up)
//{
//	this->up = up;
//}
//void Enemy::SetBoundary(Vector3 min, Vector3 max)
//{
//	minBoundary = min;
//	maxBoundary = max;
//}
//
//void Enemy::SetTerrain(GroundEntity* terrain)
//{
//	if (this->terrain != NULL)
//	{
//		this->terrain = terrain;
//		SetBoundary(this->terrain->GetMinBoundary(), this->terrain->GetMinBoundary());
//	}
//}
//
//Vector3 Enemy::GetPosition() const
//{
//	return position;
//}
//Vector3 Enemy::GetTarget() const
//{
//	return target;
//}
//Vector3 Enemy::GetUp() const
//{
//	return up;
//}
//GroundEntity* Enemy::GetTerrain()
//{
//	return terrain;
//}