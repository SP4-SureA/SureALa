#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"

class Scene
{
public:
	Scene() : entityManager(NULL) {}
	virtual ~Scene(){}// { if (entityManager){ delete entityManager; } }

	virtual void Init() = 0;
	virtual void UpdateInputs(double dt){}
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	inline EntityManager* GetEntityManager(){ return this->entityManager; }

	EntityManager* entityManager;
};

#endif // SCENE_H