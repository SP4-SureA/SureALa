#ifndef WALL_H
#define WALL_H

#include "EntityBase.h"

class Mesh;
class EntityManager;

class Wall : public EntityBase
{
public:
	Wall(Mesh* _modelMesh);
	~Wall();

	virtual void Update(double dt);
	virtual void CollisionResponse(EntityBase* other, double dt);
	virtual void Render();

private:
	Mesh* modelMesh;
};

namespace Create
{
	Wall* squareWall(
		EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const Vector3& _front = Vector3(1, 0, 0)
		);
	Wall* roundWall(
		EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
		);
}

#endif