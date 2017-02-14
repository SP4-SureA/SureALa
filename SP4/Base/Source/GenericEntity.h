#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"
#include "LevelOfDetails.h"

class Mesh;
class EntityManager;

class GenericEntity : public EntityBase, public CCollider, public LevelOfDetails
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);

protected:
	Mesh* modelMesh;
};

namespace Create
{
	GenericEntity* Entity(EntityManager* em,
							const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
	GenericEntity* Asset(EntityManager* em,
							const std::string& _meshName,
							const Vector3& _position,
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H