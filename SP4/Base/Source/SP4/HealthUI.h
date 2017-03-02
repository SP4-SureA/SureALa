#ifndef HEALTHUI_H
#define HEALTHUI_H

#include "EntityBase.h"

class Mesh;
class EntityManager;

class HealthUI : public EntityBase
{
public:
	HealthUI(Mesh* _modelMesh);
	virtual ~HealthUI();

	virtual void RenderUI();

	inline void SetMesh(Mesh* mesh){ if (mesh){ this->modelMesh = mesh; } }

	inline void SetCanvasSize(Vector3 size){ this->canvasSize = size; }
	inline Vector3 GetCanvasSize(){ return this->canvasSize; }

protected:
	Mesh* modelMesh;
	Vector3 canvasSize;
};

namespace Create
{
	HealthUI* healthUIEntity(EntityManager* em,
		const std::string& _meshName,
		const Vector3& _scale,
		const Vector3& _canvasSize
		);

	HealthUI* healthUIAsset(
		const std::string& _meshName,
		const Vector3& _scale,
		const Vector3& _canvasSize
		);
};

#endif