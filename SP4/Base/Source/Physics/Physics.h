#ifndef PHYSICS_H
#define PHYSICS_H

#include "SingletonTemplate.h"

class GenericEntity;

class Physics : public Singleton<Physics>
{
	friend Singleton<Physics>;
public:
	virtual ~Physics();

	static bool CheckCollision(GenericEntity* a, GenericEntity* b, double dt);
	static void CollisionResponse(GenericEntity* a, GenericEntity* b);
	static bool CheckCollision2(GenericEntity* a, GenericEntity* b, double dt);
	static void CollisionResponse2(GenericEntity* a, GenericEntity* b);

protected:
	Physics();
};

#endif