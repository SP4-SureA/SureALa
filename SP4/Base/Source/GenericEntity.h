#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "LevelOfDetails.h"

class Mesh;
class EntityManager;

class GenericEntity : public EntityBase
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

    virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();

	inline void SetMesh(Mesh* mesh){ if (mesh){ this->modelMesh = mesh; } }

    inline void SetMaxSpeed(const float _value){ this->maxSpeed = _value; };
    inline float GetMaxSpeed(){ return this->maxSpeed; };

    void ClampSpeed();

protected:
    float maxSpeed;

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