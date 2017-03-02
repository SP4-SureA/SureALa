#include "CloudFactory.h"
#include "MeshBuilder.h"
#include "EntityManager.h"

CloudFactory::CloudFactory() :
maxSpeed(0),
minPos(Vector3(0, 0, 0)),
maxPos(Vector3(0, 0, 0)),
spawnRate(0),
currentTime(0)
{

}

CloudFactory::~CloudFactory()
{
}

void CloudFactory::Update(double dt)
{
	if (currentTime > spawnRate)
	{
		SpawnCloud();
		spawnRate += Math::RandFloatMinMax(-1, 1);
		spawnRate = Math::Clamp(spawnRate, 1.0, 3.0);
		currentTime = 0;
	}

	currentTime += dt;
}

void CloudFactory::SpawnCloud()
{
	int cloudType = Math::RandIntMinMax(1, 3);

	CloudEntity* cloud = NULL;

	float zPos;
	if (rand() % 2)
		zPos = 0.5f;
	else
		zPos = -1.0f;

	switch (cloudType)
	{
	case 1:
		if (zPos < 0)
			cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_1_opaque", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(50, 50, 100));
		else
			cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_1_translucent", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(50, 50, 100));
		break;

	case 2:
		if (zPos < 0)
			cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_3_opaque", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(10, 5, 100));
		else
			cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_3_translucent", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(10, 5, 100));
		break;

	case 3:
		if (zPos < 0)
			cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_4_opaque", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(30, 20, 100));
		else
			cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_4_translucent", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(30, 20, 100));
		break;

	default:
		cloud = Create::cloudEntity(EntityManager::GetInstance(), "cloud_1", Vector3(Math::RandFloatMinMax(minPos.x, maxPos.x), maxPos.y, zPos), 0, Vector3(50, 50, 100));
		break;
	}

	cloud->SetVelocity(Vector3(0, -Math::RandFloatMinMax(maxSpeed * 0.5, maxSpeed * 2), 0));
	cloud->SetPosition(cloud->GetPosition() + Vector3(0, cloud->GetScale().y, 0));
}

CloudFactory* Create::cloudFactoryEntity(EntityManager* em,
	const float& _maxSpeed,
	const double& _spawnRate,
	const Vector3& _minPos,
	const Vector3& _maxPos
	)
{
	if (em == NULL)
		return NULL;

	CloudFactory* result = new CloudFactory();
	result->SetMaxSpeed(_maxSpeed);
	result->SetSpawnRate(_spawnRate);
	result->SetMinPos(_minPos);
	result->SetMaxPos(_maxPos);

	em->AddEntity(result, true);
	return result;
}

CloudFactory* Create::cloudFactoryAsset(const float& _maxSpeed,
	const double& _spawnRate,
	const Vector3& _minPos,
	const Vector3& _maxPos
	)
{
	CloudFactory* result = new CloudFactory();

	return result;
}
