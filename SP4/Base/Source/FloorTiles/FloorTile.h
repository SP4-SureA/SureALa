#ifndef FLOOR_TILE_H
#define FLOOR_TILE_H

#include "EntityBase.h"
#include "Vector3.h"

class Mesh;
class EntityManager;

class FloorTile : public EntityBase
{
public:
	FloorTile(Mesh* _modelMesh);
	~FloorTile();

	virtual void Render();

	inline void SetFront(Vector3 _front){ this->front = _front; }
	inline Vector3 GetFront(){ return this->front; }

protected:
	Mesh* modelMesh;
	Vector3 front;

};

namespace Create
{
	FloorTile* floorTileEntity(EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const Vector3& _front = Vector3(1, 0, 0)
		);

	FloorTile* floorTileAsset(
		const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const Vector3& _front = Vector3(1, 0, 0)
		);
};

#endif