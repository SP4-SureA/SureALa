#ifndef BUTTON_ENTITY_H
#define BUTTON_ENTITY_H

#include "EntityBase.h"
#include "Collider\Collider.h"
#include <string>

class Mesh;
class EntityManager;

class Button : public EntityBase, public CCollider
{
public:
	Button(Mesh* _modelMesh);
	virtual ~Button();

	virtual void Update(double dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);

	bool CheckCollision(Vector3 pos);

protected:
	Mesh* modelMesh;
};

namespace Create
{
	Button* button(EntityManager* em, const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
	Button* AssetButton(EntityManager* em, const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H