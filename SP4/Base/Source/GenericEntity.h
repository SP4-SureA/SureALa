#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"
#include "LevelOfDetails.h"

class Mesh;
class EntityManager;

enum COLLIDER_TYPE
{
    COLLIDER_NONE = 0,
    COLLIDER_BOX,
    COLLDER_SPHERE,

    NUM_COLLIDER_TYPE,
};

class GenericEntity : public EntityBase
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

    virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();

    inline void SetFront(const Vector3& _value){ this->front = _value; };
    inline Vector3 GetFront(){ return this->front; };

    inline void SetMaxSpeed(const float _value){ this->maxSpeed = _value; };
    inline float GetMaxSpeed(){ return this->maxSpeed; };

    void ClampSpeed();

protected:
    float maxSpeed;

    Vector3 front;

	Mesh* modelMesh;

    COLLIDER_TYPE collider_Type;
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