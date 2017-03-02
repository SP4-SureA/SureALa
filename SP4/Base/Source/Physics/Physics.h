#ifndef PHYSICS_H
#define PHYSICS_H

#include "SingletonTemplate.h"

class EntityBase;

class Physics : public Singleton<Physics>
{
	friend Singleton<Physics>;
public:
	virtual ~Physics();

	static bool CheckCollision(EntityBase* a, EntityBase* b, double dt);
	static void CollisionResponse(EntityBase* a, EntityBase* b);

protected:
	static bool CheckSphereSphere(EntityBase* a, EntityBase* b, double dt);
	static bool CheckSphereBox(EntityBase* a, EntityBase* b, double dt);
	static void ResponseSphereSphere(EntityBase* a, EntityBase* b);
	static void ResponseSphereBox(EntityBase* a, EntityBase* b);

	Physics();
};

#endif