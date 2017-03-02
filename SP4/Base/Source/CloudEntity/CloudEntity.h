#ifndef _CLOUD_ENTITY_H
#define _CLOUD_ENTITY_H

#include "Vector3.h"
#include "../GenericEntity.h"

class CloudEntity : public GenericEntity
{
public:
    CloudEntity(Mesh* _modelMesh);
    virtual ~CloudEntity();

    virtual void Update(double dt);
    virtual void Render();

protected:	

};

namespace Create
{
	CloudEntity* cloudEntity(
		EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _velocity = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );

	CloudEntity* cloudAsset(
		const std::string& _meshName,
        const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _velocity = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
        );
};

#endif // _CLOUD_ENTITY_H