#ifndef _CLOUD_FACTORY_H
#define _CLOUD_FACTORY_H

#include "Vector3.h"
#include "CloudEntity.h"
#include "EntityBase.h"

class CloudFactory : public EntityBase
{
public:
	CloudFactory();
	virtual ~CloudFactory();

    virtual void Update(double dt);

	inline void SetSpawnRate(const double&  _spawnRate){ this->spawnRate = _spawnRate; };
	inline double GetSpawnRate(void){ return this->spawnRate; };

	inline void SetMaxSpeed(const float&  _maxSpeed){ this->maxSpeed = _maxSpeed; };
	inline float GetMaxSpeed(void){ return this->maxSpeed; };

	inline void SetMinPos(const Vector3&  _minPos){ this->minPos = _minPos; };
	inline Vector3 GetMinPos(void){ return this->minPos; };

	inline void SetMaxPos(const Vector3&  _maxPos){ this->maxPos = _maxPos; };
	inline Vector3 GetMaxPos(void){ return this->maxPos; };

	void SpawnCloud();

protected:	
	float maxSpeed;

	Vector3 minPos;
	Vector3 maxPos;

	double spawnRate;
	double currentTime;
};

namespace Create
{
	CloudFactory* cloudFactoryEntity(
		EntityManager* em,
		const float& _maxSpeed = 1,
		const double& _spawnRate = 0,
		const Vector3& _minPos = Vector3(0, 0, 0),
		const Vector3& _maxPos = Vector3(1.0f, 1.0f, 1.0f)
		);

	CloudFactory* cloudFactoryAsset(
		const float& _maxSpeed = 1,
		const double& _spawnRate = 0,
		const Vector3& _minPos = Vector3(0, 0, 0),
		const Vector3& _maxPos = Vector3(1.0f, 1.0f, 1.0f)
		);
};

#endif // _CLOUD_FACTORY_H